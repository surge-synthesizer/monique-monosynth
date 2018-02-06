//------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auwrapper/aucocoaview.mm
// Created by  : Steinberg, 12/2007
// Description : VST 3 -> AU Wrapper
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2013, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// This Software Development Kit may not be distributed in parts or its entirety  
// without prior written agreement by Steinberg Media Technologies GmbH. 
// This SDK must not be used to re-engineer or manipulate any technology used  
// in any Steinberg or Third-party application or software module, 
// unless permitted by law.
// Neither the name of the Steinberg Media Technologies nor the names of its
// contributors may be used to endorse or promote products derived from this 
// software without specific prior written permission.
// 
// THIS SDK IS PROVIDED BY STEINBERG MEDIA TECHNOLOGIES GMBH "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL STEINBERG MEDIA TECHNOLOGIES GMBH BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------------------------

/// \cond ignore

#import "aucocoaview.h"
#import "auwrapper.h"
#import "pluginterfaces/vst/ivsteditcontroller.h"
#import "pluginterfaces/gui/iplugview.h"
#import "base/source/fobject.h"


namespace Steinberg {
DEF_CLASS_IID(IPlugFrame)

//--------------------------------------------------------------------------------------------------------------
class AUPlugFrame : public FObject, public IPlugFrame
//--------------------------------------------------------------------------------------------------------------
{
public:
	AUPlugFrame (NSView* parent) : parent (parent) {}
	tresult PLUGIN_API resizeView (IPlugView* view, ViewRect* vr)
	{
		NSRect newSize = NSMakeRect ([parent frame].origin.x, [parent frame].origin.y, vr->right - vr->left, vr->bottom - vr->top);
		[parent setFrame:newSize];
		return kResultTrue;
	}
	
	OBJ_METHODS(AUPlugFrame, FObject)
	DEF_INTERFACES_1(IPlugFrame, FObject)
	REFCOUNT_METHODS(FObject)
protected:
	NSView* parent;
};
	
} // namespace Steinberg

using namespace Steinberg;

#if DYNAMIC_AUWRAPPER_CLASS_CREATION
//------------------------------------------------------------------------------------
// Fun Stuff for stupid objective-c non namespace feature
//------------------------------------------------------------------------------------

#import <objc/runtime.h>
#import <objc/message.h>
#import <Cocoa/Cocoa.h>
#import <dlfcn.h>

#define HIDDEN __attribute__((__visibility__("hidden")))

//------------------------------------------------------------------------------------
inline HIDDEN id get_Objc_Value (id obj, const char* name)
{
	Ivar ivar = class_getInstanceVariable ([obj class], name);
	if (ivar)
	{
		id value = object_getIvar (obj, ivar);
		return value;
	}
	return nil;
}

//------------------------------------------------------------------------------------
inline HIDDEN void set_Objc_Value (id obj, const char* name, id value)
{
	Ivar ivar = class_getInstanceVariable ([obj class], name);
	if (ivar)
	{
		object_setIvar (obj, ivar, value);
	}
}

//------------------------------------------------------------------------------------
static Class generateUniqueClass (NSMutableString* className, Class baseClass)
{
	NSString* _className = [NSString stringWithString:className];
	NSInteger iteration = 0;
	id cl = nil;
	while ((cl = objc_lookUpClass ([className UTF8String])) != nil)
	{
		iteration++;
		[className setString:[NSString stringWithFormat:@"%@_%d", _className, iteration]];
	}
	Class resClass = objc_allocateClassPair (baseClass, [className UTF8String], 0);
	return resClass;
}

#define __OBJC_SUPER(x) objc_super __os; __os.receiver = x; __os.super_class = class_getSuperclass ([x class]);
#define SUPER	&__os
#define OBJC_GET_VALUE(x,y) get_Objc_Value (x, #y)
#define OBJC_SET_VALUE(x,y,z) set_Objc_Value (x, #y, (id)z)

//------------------------------------------------------------------------------------
// SMTGCocoa_NSViewWrapperForAU
//------------------------------------------------------------------------------------
static Class NSViewWrapperForAU = 0;
struct NSViewWrapperForAUVar {
	IPlugView* plugView;
	Vst::IEditController* editController;
	AudioUnit audioUnit;
	FObject* dynlib;
	AUPlugFrame* plugFrame;
	
	BOOL isAttached;
	
	NSViewWrapperForAUVar () : plugView(0), editController(0), audioUnit(0), dynlib(0), plugFrame (0), isAttached (NO) {}
};

//--------------------------------------------------------------------------------------------------------------
@interface NSObject (SMTGCocoa_NSViewWrapperForAU)
- (id) initWithEditController: (Vst::IEditController*) editController audioUnit: (AudioUnit) au preferredSize: (NSSize) size;
@end

//------------------------------------------------------------------------------------
static id SMTGCocoa_NSViewWrapperForAU_initWithEditController (id self, SEL _cmd, Vst::IEditController* editController, AudioUnit au, NSSize size)
{
	__OBJC_SUPER(self);
	self = objc_msgSendSuper (SUPER, @selector(initWithFrame:), NSMakeRect (0, 0, size.width, size.height));
	if (self)
	{
		NSViewWrapperForAUVar* var = new NSViewWrapperForAUVar ();
		OBJC_SET_VALUE (self, _var, var);

		var->editController = editController;
		var->editController->addRef ();
		var->audioUnit = au;
		var->plugView = editController->createView (Vst::ViewType::kEditor);
		if (!var->plugView || var->plugView->isPlatformTypeSupported (kPlatformTypeNSView) != kResultTrue)
		{
			[self dealloc];
			return nil;
		}
		
		var->plugFrame = NEW AUPlugFrame (self);
		var->plugView->setFrame (var->plugFrame);
		
		if (var->plugView->attached (self, kPlatformTypeNSView) != kResultTrue)
		{
			[self dealloc];
			return nil;
		}
		ViewRect vr;
		if (var->plugView->getSize (&vr) == kResultTrue)
		{
			NSRect newSize = NSMakeRect (0, 0, vr.right - vr.left, vr.bottom - vr.top);
			[self setFrame:newSize];
		}
		
		var->isAttached = YES;
		UInt32 size = sizeof (FObject*);
		if (AudioUnitGetProperty (var->audioUnit, 64001, kAudioUnitScope_Global, 0, &var->dynlib, &size) == noErr)
			var->dynlib->addRef ();
	}
	return self;
}

//------------------------------------------------------------------------------------
static BOOL SMTGCocoa_NSViewWrapperForAU_isFlipped (id self, SEL _cmd) { return YES; }

//------------------------------------------------------------------------------------
static void SMTGCocoa_NSViewWrapperForAU_setFrame (id self, SEL _cmd, NSRect newSize)
{
	__OBJC_SUPER(self);
	objc_msgSendSuper(SUPER, @selector(setFrame:), newSize);

	NSViewWrapperForAUVar* var = (NSViewWrapperForAUVar*)OBJC_GET_VALUE (self, _var);
	if (var)
	{
		ViewRect viewRect (0, 0, newSize.size.width, newSize.size.height);
		
		if (var->plugView)
			var->plugView->onSize (&viewRect);
	}
}

//------------------------------------------------------------------------------------
static void SMTGCocoa_NSViewWrapperForAU_Dealloc (id self, SEL _cmd)
{
	NSViewWrapperForAUVar* var = (NSViewWrapperForAUVar*)OBJC_GET_VALUE (self, _var);
	if (var)
	{
		if (var->plugView)
		{
			if (var->isAttached)
			{
				var->plugView->setFrame (0);
				var->plugView->removed ();
			}
			var->plugView->release ();
			if (var->plugFrame)
				var->plugFrame->release ();
			
			if (var->editController)
			{
				Steinberg::uint32 refCount= var->editController->addRef ();
				if (refCount == 2)
					var->editController->terminate ();
				
				var->editController->release ();
				var->editController->release ();
				var->editController = 0;
			}		
		}
		if (var->dynlib)
			var->dynlib->release ();
		delete var;
	}
	__OBJC_SUPER(self);
	objc_msgSendSuper(SUPER, @selector(dealloc));
}

//------------------------------------------------------------------------------------
static void SMTGCocoa_NSViewWrapperForAU_ViewDidMoveToSuperview (id self, SEL _cmd)
{
	NSViewWrapperForAUVar* var = (NSViewWrapperForAUVar*)OBJC_GET_VALUE (self, _var);
	if (var)
	{
		if (var->plugView)
		{
			if ([self superview])
			{
				if (!var->isAttached)
				{
					var->isAttached = var->plugView->attached (self, kPlatformTypeNSView) == kResultTrue;
				}
			}
			else
			{
				if (var->isAttached)
				{
					var->plugView->removed ();
					var->isAttached = NO;
				}
			}
		}
	}
}

//------------------------------------------------------------------------------------
// SMTGCocoa_NSViewWrapperForAU
//------------------------------------------------------------------------------------
static Class AUViewFactory = 0;
NSString* AUViewFactoryClassName = 0;

//------------------------------------------------------------------------------------
static unsigned SMTGCocoa_NSViewWrapperForAU_InterfaceVersion (id self, SEL _cmd)
{
	return 0;
}

//------------------------------------------------------------------------------------
static NSString* SMTGCocoa_NSViewWrapperForAU_Description (id self, SEL _cmd)
{
	return @"Cocoa View";
}

//------------------------------------------------------------------------------------
static NSView* SMTGCocoa_NSViewWrapperForAU_UiViewForAudioUnit (id self, SEL _cmd, AudioUnit inAU, NSSize inPreferredSize)
{
	Vst::IEditController* editController = 0;
	UInt32 size = sizeof (Vst::IEditController*);
	if (AudioUnitGetProperty (inAU, 64000, kAudioUnitScope_Global, 0, &editController, &size) != noErr)
		return nil;
	return [[[NSViewWrapperForAU alloc] initWithEditController:editController audioUnit:inAU preferredSize:inPreferredSize] autorelease];
}

//------------------------------------------------------------------------------------
// NSBundle bundleForClass patch
//------------------------------------------------------------------------------------
static NSURL* GetBundleURLFromExecutable (const char* filepath)
{
	NSString* execStr = [NSString stringWithCString:filepath encoding:NSUTF8StringEncoding];
	NSString* macOSStr = [execStr stringByDeletingLastPathComponent];
	NSString* contentsStr = [macOSStr stringByDeletingLastPathComponent];
	NSString* bundleStr = [contentsStr stringByDeletingLastPathComponent];
	return [NSURL fileURLWithPath:bundleStr isDirectory:YES];
	
}

//--------------------------------------------------------------------------------------------------------------
static NSURL* GetCurrentBundleURL ()
{
	Dl_info info;
	if (dladdr ((const void*)GetCurrentBundleURL, &info))
	{
		if (info.dli_fname)
		{
			return GetBundleURLFromExecutable (info.dli_fname);
		}
	}
	return nil;
}

//--------------------------------------------------------------------------------------------------------------
static IMP originalNSBundleForClassImplementation = 0;
static NSBundle* SMTGCocoa_NSBundle_BundleForClass (id self, SEL _cmd, Class aClass)
{
	if (aClass == AUViewFactory)
	{
		NSURL* bundleUrl = GetCurrentBundleURL ();
		NSArray* allBundles = [NSBundle allBundles];
		for (NSBundle* bundle in allBundles)
		{
			if ([[bundle bundleURL] isEqualTo:bundleUrl])
				return bundle;
		}
	}
	return originalNSBundleForClassImplementation (self, _cmd, aClass);
}

//------------------------------------------------------------------------------------
// NSBundle classNamed patch
//------------------------------------------------------------------------------------
static IMP originalNSBundleClassNamed = 0;
static Class SMTGCocoa_NSBundle_ClassNamed (id self, SEL _cmd, NSString* className)
{
	if ([className isEqualToString:AUViewFactoryClassName])
		return AUViewFactory;
	return originalNSBundleClassNamed (self, _cmd, className);
}

//------------------------------------------------------------------------------------
// Create dynamic Objective-C classes
//------------------------------------------------------------------------------------
__attribute__((__constructor__)) void createCocoaClasses ()
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSMutableString* viewClassName = [[[NSMutableString alloc] initWithString:@"SMTGCocoa_NSViewWrapperForAU"] autorelease];
	NSViewWrapperForAU = generateUniqueClass (viewClassName, [NSView class]);

	BOOL res;
	const char* nsSizeEncoded = @encode(NSSize);
	const char* nsRectEncoded = @encode(NSRect);
	char funcSig[100];

	sprintf (funcSig, "@@:@:^:^:%s:", nsSizeEncoded);
	res = class_addMethod (NSViewWrapperForAU, @selector(initWithEditController:audioUnit:preferredSize:), IMP (SMTGCocoa_NSViewWrapperForAU_initWithEditController), funcSig);
	res = class_addMethod (NSViewWrapperForAU, @selector(isFlipped), IMP (SMTGCocoa_NSViewWrapperForAU_isFlipped), "B@:@:");
	res = class_addMethod (NSViewWrapperForAU, @selector(dealloc), IMP (SMTGCocoa_NSViewWrapperForAU_Dealloc), "v@:@:");
	sprintf (funcSig, "v@:@:%s:", nsRectEncoded);
	res = class_addMethod (NSViewWrapperForAU, @selector(setFrame:), IMP (SMTGCocoa_NSViewWrapperForAU_setFrame), funcSig);
	res = class_addMethod (NSViewWrapperForAU, @selector(viewDidMoveToSuperview), IMP (SMTGCocoa_NSViewWrapperForAU_ViewDidMoveToSuperview), "v@:@:");

	res = class_addIvar (NSViewWrapperForAU, "_var", sizeof (void*), (uint8_t)log2(sizeof(void*)), @encode(void*));

	viewClassName = [[[NSMutableString alloc] initWithString:@"SMTGCocoa_NSViewWrapperForAU_Dynamic"] autorelease];
	AUViewFactory = generateUniqueClass (viewClassName, [NSObject class]);
	AUViewFactoryClassName = [viewClassName retain];

	Protocol* protocol = objc_getProtocol ("AUCocoaUIBase");
	res = class_addProtocol (AUViewFactory, protocol);
	
	sprintf (funcSig, "%s@:@:", @encode(unsigned));
	res = class_addMethod (AUViewFactory, @selector(interfaceVersion), IMP (SMTGCocoa_NSViewWrapperForAU_InterfaceVersion), funcSig);
	res = class_addMethod (AUViewFactory, @selector(description), IMP (SMTGCocoa_NSViewWrapperForAU_Description), "@@:@:");
	sprintf (funcSig, "@@:@:@:", nsSizeEncoded);
	res = class_addMethod (AUViewFactory, @selector(uiViewForAudioUnit:withSize:), IMP (SMTGCocoa_NSViewWrapperForAU_UiViewForAudioUnit), funcSig);

	objc_registerClassPair (NSViewWrapperForAU);
	objc_registerClassPair (AUViewFactory);

	Method m = class_getClassMethod ([NSBundle class], @selector(bundleForClass:));
	if (m)
	{
		originalNSBundleForClassImplementation = method_getImplementation (m);
		method_setImplementation (m, IMP (SMTGCocoa_NSBundle_BundleForClass));
	}
	m = class_getInstanceMethod([NSBundle class], @selector(classNamed:));
	if (m)
	{
		originalNSBundleClassNamed = method_getImplementation (m);
		method_setImplementation (m, IMP (SMTGCocoa_NSBundle_ClassNamed));
	}


	ASSERT ([AUViewFactory conformsToProtocol:@protocol(AUCocoaUIBase)] &&
		    [AUViewFactory instancesRespondToSelector:@selector(interfaceVersion)] &&
			[AUViewFactory instancesRespondToSelector:@selector(uiViewForAudioUnit:withSize:)]);

	[pool drain];
	
}

//--------------------------------------------------------------------------------------------------------------
__attribute__((__destructor__)) void unregisterCocoaClasses ()
{
	if (AUViewFactory)
		objc_disposeClassPair (AUViewFactory);
	if (NSViewWrapperForAU)
		objc_disposeClassPair (NSViewWrapperForAU);
	if (AUViewFactoryClassName)
		[AUViewFactoryClassName release];
	if (originalNSBundleForClassImplementation)
	{
		Method m = class_getClassMethod ([NSBundle class], @selector(bundleForClass:));
		if (method_getImplementation (m) == IMP (SMTGCocoa_NSBundle_BundleForClass))
		{
			method_setImplementation (m, originalNSBundleForClassImplementation);
		}
	}
	if (originalNSBundleClassNamed)
	{
		Method m = class_getInstanceMethod ([NSBundle class], @selector(classNamed:));
		if (method_getImplementation (m) == IMP (SMTGCocoa_NSBundle_ClassNamed))
		{
			method_setImplementation (m, originalNSBundleClassNamed);
		}
	}

}
#else

//--------------------------------------------------------------------------------------------------------------
@interface SMTGCocoa_NSViewWrapperForAU : NSView {
//--------------------------------------------------------------------------------------------------------------
	IPlugView* plugView;
	Vst::IEditController* editController;
	AudioUnit audioUnit;
	FObject* dynlib;
	AUPlugFrame* plugFrame;
	
	BOOL isAttached;
}

- (id) initWithEditController: (Vst::IEditController*) editController audioUnit: (AudioUnit) au preferredSize: (NSSize) size;

@end

//--------------------------------------------------------------------------------------------------------------
@implementation SMTGCocoa_AUViewFactory
//--------------------------------------------------------------------------------------------------------------
- (unsigned) interfaceVersion 
{
	return 0;
}

//--------------------------------------------------------------------------------------------------------------
- (NSString *) description
{
	return @"Cocoa View";
}

//--------------------------------------------------------------------------------------------------------------
- (NSView *)uiViewForAudioUnit:(AudioUnit)inAU withSize:(NSSize)inPreferredSize
{
	Vst::IEditController* editController = 0;
	UInt32 size = sizeof (Vst::IEditController*);
	if (AudioUnitGetProperty (inAU, 64000, kAudioUnitScope_Global, 0, &editController, &size) != noErr)
		return nil;
	return [[[SMTGCocoa_NSViewWrapperForAU alloc] initWithEditController:editController audioUnit:inAU preferredSize:inPreferredSize] autorelease];
}

@end

//--------------------------------------------------------------------------------------------------------------
@implementation SMTGCocoa_NSViewWrapperForAU
//--------------------------------------------------------------------------------------------------------------
- (id) initWithEditController: (Vst::IEditController*) _editController audioUnit: (AudioUnit) au preferredSize: (NSSize) size
{
	self = [super initWithFrame: NSMakeRect (0, 0, size.width, size.height)];
	if (self)
	{
		editController = _editController;
		editController->addRef ();
		audioUnit = au;
		plugView = editController->createView (Vst::ViewType::kEditor);
		if (!plugView || plugView->isPlatformTypeSupported (kPlatformTypeNSView) != kResultTrue)
		{
			[self dealloc];
			return nil;
		}
		
		plugFrame = NEW AUPlugFrame (self);
		plugView->setFrame (plugFrame);
		
		if (plugView->attached (self, kPlatformTypeNSView) != kResultTrue)
		{
			[self dealloc];
			return nil;
		}
		ViewRect vr;
		if (plugView->getSize (&vr) == kResultTrue)
		{
			NSRect newSize = NSMakeRect (0, 0, vr.right - vr.left, vr.bottom - vr.top);
			[self setFrame:newSize];
		}

		isAttached = YES;
		UInt32 size = sizeof (FObject*);
		if (AudioUnitGetProperty (audioUnit, 64001, kAudioUnitScope_Global, 0, &dynlib, &size) == noErr)
			dynlib->addRef ();
	}
	return self;
}


//--------------------------------------------------------------------------------------------------------------
- (void) setFrame: (NSRect) newSize
{
	[super setFrame: newSize];
	ViewRect viewRect (0, 0, newSize.size.width, newSize.size.height);
	
	if (plugView)
		plugView->onSize (&viewRect);
}


//--------------------------------------------------------------------------------------------------------------
- (BOOL)isFlipped { return YES; }

//--------------------------------------------------------------------------------------------------------------
- (void)viewDidMoveToSuperview
{
	if (plugView)
	{
		if ([self superview])
		{
			if (!isAttached)
			{
				isAttached = plugView->attached (self, kPlatformTypeNSView) == kResultTrue;
			}
		}
		else
		{
			if (isAttached)
			{
				plugView->removed ();
				isAttached = NO;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
- (void) dealloc
{
	if (plugView)
	{
		if (isAttached)
		{
			plugView->setFrame (0);
			plugView->removed ();
		}
		plugView->release ();
		if (plugFrame)
			plugFrame->release ();
		
		if (editController)
		{
			Steinberg::uint32 refCount= editController->addRef ();
			if (refCount == 2)
				editController->terminate ();
			
			editController->release ();
			editController->release ();
			editController = 0;
		}		
	}
	if (dynlib)
		dynlib->release ();
	[super dealloc];
}

@end
#endif

/// \endcond
