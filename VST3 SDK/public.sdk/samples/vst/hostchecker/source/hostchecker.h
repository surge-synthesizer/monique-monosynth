//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/hostchecker.h
// Created by  : Steinberg, 04/2012
// Description : 
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2013, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef __hostchecker__
#define __hostchecker__

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "base/source/fstring.h"
#include "base/source/tdictionary.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "hostcheck.h"
#include "logevents.h"

namespace VSTGUI {
	class EventLogDataBrowserSource;
}

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
class HostCheckerController : public EditControllerEx1, public VSTGUI::VST3EditorDelegate
{
public:
	tresult PLUGIN_API initialize (FUnknown* context);
	tresult PLUGIN_API terminate ();

	virtual tresult PLUGIN_API setComponentState (IBStream* state);
	virtual tresult PLUGIN_API getUnitByBus (MediaType type, BusDirection dir, int32 busIndex, int32 channel, UnitID& unitId /*out*/);

	virtual IPlugView* PLUGIN_API createView (FIDString name);
	virtual tresult PLUGIN_API notify (IMessage* message);
	virtual tresult PLUGIN_API connect (IConnectionPoint* other);

	VSTGUI::CView* createCustomView (VSTGUI::UTF8StringPtr name, const VSTGUI::UIAttributes& attributes, VSTGUI::IUIDescription* description, VSTGUI::VST3Editor* editor);

	static FUnknown* createInstance (void*) { return (IEditController*)new HostCheckerController (); }
	static FUID cid;
protected:
	CDataBrowser* mDataBrowser;
	EventLogDataBrowserSource* mDataSource;
};

//-----------------------------------------------------------------------------
class HostCheckerProcessor : public AudioEffect
{
public:
	HostCheckerProcessor ();

	tresult PLUGIN_API initialize (FUnknown* context);
	tresult PLUGIN_API process (ProcessData& data);
	tresult PLUGIN_API setupProcessing (ProcessSetup& setup);
	tresult PLUGIN_API notify (IMessage* message);

	tresult PLUGIN_API setState (IBStream* state);
	tresult PLUGIN_API getState (IBStream* state);

	static FUnknown* createInstance(void*) { return (IAudioProcessor*)new HostCheckerProcessor (); }
	void sendLogEventMessage (const LogEvent& logEvent);
	static FUID cid;

protected:
	HostCheck mHostCheck;
};

}} // namespaces

#endif	// __hostchecker__
