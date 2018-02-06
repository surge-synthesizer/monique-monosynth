//-----------------------------------------------------------------------------
// Project     : SDK Core
//
// Category    : SDK Core Interfaces
// Filename    : pluginterfaces/base/funknown.h
// Created by  : Steinberg, 01/2004
// Description : Basic Interface
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
//------------------------------------------------------------------------------

#ifndef __funknown__
#define __funknown__

#include <string.h>
#include "ftypes.h"


//------------------------------------------------------------------------
#if WINDOWS
	#define COM_COMPATIBLE 1
#else
	#define COM_COMPATIBLE 0
#endif

//------------------------------------------------------------------------
/*! \defgroup pluginBase Basic Interfaces
*/

//------------------------------------------------------------------------
#undef PLUGIN_API
#if COM_COMPATIBLE
	#define PLUGIN_API __stdcall
#else
	#define PLUGIN_API
#endif


//------------------------------------------------------------------------
//  Unique Identifier macros
//------------------------------------------------------------------------

#if COM_COMPATIBLE
#define INLINE_UID(l1, l2, l3, l4) \
{ \
	(::Steinberg::int8)((l1 & 0x000000FF)      ), (::Steinberg::int8)((l1 & 0x0000FF00) >>  8), \
	(::Steinberg::int8)((l1 & 0x00FF0000) >> 16), (::Steinberg::int8)((l1 & 0xFF000000) >> 24), \
	(::Steinberg::int8)((l2 & 0x00FF0000) >> 16), (::Steinberg::int8)((l2 & 0xFF000000) >> 24), \
	(::Steinberg::int8)((l2 & 0x000000FF)      ), (::Steinberg::int8)((l2 & 0x0000FF00) >>  8), \
	(::Steinberg::int8)((l3 & 0xFF000000) >> 24), (::Steinberg::int8)((l3 & 0x00FF0000) >> 16), \
	(::Steinberg::int8)((l3 & 0x0000FF00) >>  8), (::Steinberg::int8)((l3 & 0x000000FF)      ), \
	(::Steinberg::int8)((l4 & 0xFF000000) >> 24), (::Steinberg::int8)((l4 & 0x00FF0000) >> 16), \
	(::Steinberg::int8)((l4 & 0x0000FF00) >>  8), (::Steinberg::int8)((l4 & 0x000000FF)      )  \
}
#else
#define INLINE_UID(l1, l2, l3, l4) \
{ \
	(::Steinberg::int8)((l1 & 0xFF000000) >> 24), (::Steinberg::int8)((l1 & 0x00FF0000) >> 16), \
	(::Steinberg::int8)((l1 & 0x0000FF00) >>  8), (::Steinberg::int8)((l1 & 0x000000FF)      ), \
	(::Steinberg::int8)((l2 & 0xFF000000) >> 24), (::Steinberg::int8)((l2 & 0x00FF0000) >> 16), \
	(::Steinberg::int8)((l2 & 0x0000FF00) >>  8), (::Steinberg::int8)((l2 & 0x000000FF)      ), \
	(::Steinberg::int8)((l3 & 0xFF000000) >> 24), (::Steinberg::int8)((l3 & 0x00FF0000) >> 16), \
	(::Steinberg::int8)((l3 & 0x0000FF00) >>  8), (::Steinberg::int8)((l3 & 0x000000FF)      ), \
	(::Steinberg::int8)((l4 & 0xFF000000) >> 24), (::Steinberg::int8)((l4 & 0x00FF0000) >> 16), \
	(::Steinberg::int8)((l4 & 0x0000FF00) >>  8), (::Steinberg::int8)((l4 & 0x000000FF)      )  \
}
#endif

//------------------------------------------------------------------------
#define DECLARE_UID(name, l1, l2, l3, l4) \
	::Steinberg::TUID name = INLINE_UID (l1, l2, l3, l4);

//------------------------------------------------------------------------
#define EXTERN_UID(name) \
	extern const ::Steinberg::TUID name;


#ifdef INIT_CLASS_IID
#define DECLARE_CLASS_IID(ClassName, l1, l2, l3, l4) static const ::Steinberg::TUID ClassName##_iid = INLINE_UID (l1, l2, l3, l4); \
const ::Steinberg::FUID ClassName::iid (ClassName##_iid);
#else
#define DECLARE_CLASS_IID(ClassName, l1, l2, l3, l4) static const ::Steinberg::TUID ClassName##_iid = INLINE_UID (l1, l2, l3, l4);
#endif
#define DEF_CLASS_IID(ClassName) const ::Steinberg::FUID ClassName::iid (ClassName##_iid);

#define INLINE_UID_OF(ClassName) ClassName##_iid

#define INLINE_UID_FROM_FUID(x) INLINE_UID(x.getLong1 (), x.getLong2 (), x.getLong3 (), x.getLong4 ())

//------------------------------------------------------------------------
//  FUnknown implementation macros
//------------------------------------------------------------------------

#define DECLARE_FUNKNOWN_METHODS                                                                      \
public:	                                                                                              \
	virtual ::Steinberg::tresult PLUGIN_API queryInterface (const ::Steinberg::TUID iid, void** obj); \
	virtual ::Steinberg::uint32 PLUGIN_API addRef ();                                                 \
	virtual ::Steinberg::uint32 PLUGIN_API release ();                                                \
protected :                                                                                           \
	::Steinberg::int32 __funknownRefCount;                                                            \
public:

//------------------------------------------------------------------------

#define DELEGATE_REFCOUNT(ClassName)											        \
public:																			        \
	virtual ::Steinberg::uint32 PLUGIN_API addRef ()  { return ClassName::addRef ();  }	\
	virtual ::Steinberg::uint32 PLUGIN_API release () { return ClassName::release (); }	\

//------------------------------------------------------------------------
#define IMPLEMENT_REFCOUNT(ClassName)                                   \
::Steinberg::uint32 PLUGIN_API ClassName::addRef ()                     \
{                                                                       \
	return ::Steinberg::FUnknown::atomicAdd (__funknownRefCount, 1);    \
}                                                                       \
::Steinberg::uint32 PLUGIN_API ClassName::release ()                    \
{                                                                       \
	if (::Steinberg::FUnknown::atomicAdd  (__funknownRefCount, -1) == 0)\
	{                                                                   \
		delete this;                                                    \
		return 0;                                                       \
	}                                                                   \
	return __funknownRefCount;                                          \
}

//------------------------------------------------------------------------
#define FUNKNOWN_CTOR	{ __funknownRefCount = 1; }
#define FUNKNOWN_DTOR

//------------------------------------------------------------------------
#define QUERY_INTERFACE(iid, obj, InterfaceIID, InterfaceName)  \
if (memcmp (iid, InterfaceIID, sizeof (::Steinberg::TUID)) == 0)\
{                                                               \
	addRef ();                                                  \
	*obj = static_cast< InterfaceName* >(this);                 \
	return ::Steinberg::kResultOk;                              \
}

//------------------------------------------------------------------------
#define IMPLEMENT_QUERYINTERFACE(ClassName, InterfaceName, ClassIID)                                \
::Steinberg::tresult PLUGIN_API ClassName::queryInterface (const ::Steinberg::TUID iid, void** obj) \
{                                                                                                   \
	QUERY_INTERFACE (iid, obj, ::Steinberg::FUnknown::iid, InterfaceName)                           \
	QUERY_INTERFACE (iid, obj, ClassIID, InterfaceName)                                             \
	*obj = 0;                                                                                       \
	return ::Steinberg::kNoInterface;                                                               \
}

//------------------------------------------------------------------------
#define IMPLEMENT_FUNKNOWN_METHODS(ClassName,InterfaceName,ClassIID) \
	IMPLEMENT_REFCOUNT (ClassName)                                   \
	IMPLEMENT_QUERYINTERFACE (ClassName, InterfaceName, ClassIID)

//------------------------------------------------------------------------
//  Result Codes
//------------------------------------------------------------------------

namespace Steinberg {

//------------------------------------------------------------------------
#if COM_COMPATIBLE
#if WINDOWS
enum
{
	kNoInterface		= static_cast<tresult>(0x80004002L),	// E_NOINTERFACE
	kResultOk			= static_cast<tresult>(0x00000000L),	// S_OK
	kResultTrue			= kResultOk,
	kResultFalse		= static_cast<tresult>(0x00000001L),	// S_FALSE
	kInvalidArgument	= static_cast<tresult>(0x80070057L),	// E_INVALIDARG
	kNotImplemented		= static_cast<tresult>(0x80004001L),	// E_NOTIMPL
	kInternalError		= static_cast<tresult>(0x80004005L),	// E_FAIL
	kNotInitialized		= static_cast<tresult>(0x8000FFFFL),	// E_UNEXPECTED
	kOutOfMemory		= static_cast<tresult>(0x8007000EL)		// E_OUTOFMEMORY
};
#else
enum
{
	kNoInterface		= static_cast<tresult>(0x80000004L),	// E_NOINTERFACE
	kResultOk			= static_cast<tresult>(0x00000000L),	// S_OK
	kResultTrue			= kResultOk,
	kResultFalse		= static_cast<tresult>(0x00000001L),	// S_FALSE
	kInvalidArgument	= static_cast<tresult>(0x80000003L),	// E_INVALIDARG
	kNotImplemented		= static_cast<tresult>(0x80000001L),	// E_NOTIMPL
	kInternalError		= static_cast<tresult>(0x80000008L),	// E_FAIL
	kNotInitialized		= static_cast<tresult>(0x8000FFFFL),	// E_UNEXPECTED
	kOutOfMemory		= static_cast<tresult>(0x80000002L)		// E_OUTOFMEMORY
};
#endif
#else
enum
{
	kNoInterface = -1,
	kResultOk,
	kResultTrue = kResultOk,
	kResultFalse,
	kInvalidArgument,
	kNotImplemented,
	kInternalError,
	kNotInitialized,
	kOutOfMemory
};
#endif

//------------------------------------------------------------------------
typedef int64 LARGE_INT; // obsolete

//------------------------------------------------------------------------
//	FUID class declaration
//------------------------------------------------------------------------
typedef int8 TUID[16]; ///< plain UID type

//------------------------------------------------------------------------
/** Handling 16 Byte Globaly Unique Identifiers.
\ingroup pluginBase

Each interface declares its identifier as static member inside the interface
namespace (e.g. FUnknown::iid).
*/
//------------------------------------------------------------------------
class FUID
{
public:
//------------------------------------------------------------------------
	FUID ();
	FUID (const TUID uid);
	FUID (uint32 l1, uint32 l2, uint32 l3, uint32 l4);
	FUID (const FUID&);
	virtual ~FUID ();

	/** Generates a new Unique Identifier (UID).
	    Will return true for success. If the return value is false, either no
	    UID is generated or the UID is not guaranteed to be unique worldwide. */
	bool generate ();

	/** Checks if the UID data is valid.
		The default constructor initializes the memory with zeros. */
	bool isValid () const;

	FUID& operator = (const FUID& f);
	FUID& operator = (FIDString uid);
	FUID& operator = (TUID uid);

	bool operator == (const FUID& f) const { return memcmp (data, f.data, sizeof (TUID)) == 0; }
	bool operator == (FIDString uid) const { return memcmp (data, uid, sizeof (TUID)) == 0; }
	bool operator == (TUID uid) const { return memcmp (data, uid, sizeof (TUID)) == 0; }

	bool operator < (const FUID& f) const { return memcmp (data, f.data, sizeof (TUID)) < 0; }
	bool operator < (FIDString uid) const { return memcmp (data, uid, sizeof (TUID)) < 0; }
	bool operator < (TUID uid) const { return memcmp (data, uid, sizeof (TUID)) < 0; }

	bool operator != (const FUID& f) const   { return memcmp (data, f.data, sizeof (TUID)) != 0; }
	bool operator != (FIDString uid) const { return memcmp (data, uid, sizeof (TUID)) != 0; }
	bool operator != (TUID uid) const { return memcmp (data, uid, sizeof (TUID)) != 0; }

	operator FIDString () const { return data; }
	operator char* () { return data; }

 	uint32 getLong1 () const;
 	uint32 getLong2 () const;
 	uint32 getLong3 () const;
 	uint32 getLong4 () const;

	void from4Int (uint32 d1, uint32 d2, uint32 d3, uint32 d4);
	void to4Int (uint32& d1, uint32& d2, uint32& d3, uint32& d4) const;

	typedef char8 String [64];

	/** Converts UID to a string.
		The string will be 32 characters long, representing the hexadecimal values
		of each data byte (e.g. "9127BE30160E4BB69966670AA6087880"). */
	void toString (char8* string) const;

	/** Sets the UID data from a string.
		The string has to be 32 characters long, where each character-pair is
		the ASCII-encoded hexadecimal value of the corresponding data byte. */
	bool fromString (const char8* string);

	/** Converts UID to a string in Microsoft® OLE format.
	(e.g. "{c200e360-38c5-11ce-ae62-08002b2b79ef}") */
	void toRegistryString (char8* string) const;

	/** Sets the UID data from a string in Microsoft® OLE format. */
	bool fromRegistryString (const char8* string);

	enum UIDPrintStyle
	{
		kINLINE_UID,  ///< "INLINE_UID (0x00000000, 0x00000000, 0x00000000, 0x00000000)"
		kDECLARE_UID, ///< "DECLARE_UID (0x00000000, 0x00000000, 0x00000000, 0x00000000)"
		kFUID,        ///< "FUID (0x00000000, 0x00000000, 0x00000000, 0x00000000)"
		kCLASS_UID    ///< "DECLARE_CLASS_IID (Interface, 0x00000000, 0x00000000, 0x00000000, 0x00000000)"
	};
	/** Prints the UID to a string (or debug output if string is NULL).
		\param string is the output string if not NULL.
		\param style can be chosen from the FUID::UIDPrintStyle enumeration. */
	void print (char8* string = 0, int32 style = kINLINE_UID) const;

	void toTUID (TUID result) const;
//------------------------------------------------------------------------
protected:
	TUID data;
};


//------------------------------------------------------------------------
// FUnknown
//------------------------------------------------------------------------
/**	The basic interface of all interfaces.
\ingroup pluginBase

- The FUnknown::queryInterface method is used to retrieve pointers to other
  interfaces of the object.
- FUnknown::addRef and FUnknown::release manage the lifetime of the object.
  If no more references exist, the object is destroyed in memory.

Interfaces are identified by 16 byte Globally Unique Identifiers.
The SDK provides a class called FUID for this purpose.

\sa \ref howtoClass */
//------------------------------------------------------------------------
class FUnknown
{
public:
//------------------------------------------------------------------------
	/** Query for a pointer to the specified interface.
	Returns kResultOk on success or kNoInterface if the object does not implement the interface.
	The object has to call addRef when returning an interface.
	\param iid : (in) 16 Byte interface identifier (-> FUID)
	\param obj : (out) On return, *obj points to the requested interface */
	virtual tresult PLUGIN_API queryInterface (const TUID iid, void** obj) = 0;

	/** Adds a reference and return the new reference count.
	\par Remarks:
		The initial reference count after creating an object is 1. */
	virtual uint32 PLUGIN_API addRef () = 0;

	/** Releases a reference and return the new reference count.
	If the reference count reaches zero, the object will be destroyed in memory. */
	virtual uint32 PLUGIN_API release () = 0;

//------------------------------------------------------------------------
	static const FUID iid;

	static int32 atomicAdd (int32& value, int32 amount);

//------------------------------------------------------------------------
};

DECLARE_CLASS_IID (FUnknown, 0x00000000, 0x00000000, 0xC0000000, 0x00000046)


//------------------------------------------------------------------------
// IPtr
//------------------------------------------------------------------------
/**	IPtr - Smart pointer template class.
\ingroup pluginBase

- can be used as an I* pointer
- handles refCount of the interface
- Usage example:
										\code
		IPtr<IPath> path (sharedPath);
		if (path)
			path->ascend ();
										\endcode
*/
//------------------------------------------------------------------------
template <class I>
class IPtr
{
public:
//------------------------------------------------------------------------
	inline IPtr (I* ptr, bool addRef = true);
	inline IPtr (const IPtr&);
	inline IPtr ();
	inline ~IPtr ();

	inline I* operator=(I* ptr);
	inline IPtr& operator=(const IPtr& );

	inline operator I* ()  const { return ptr; }      // act as I*
	inline I* operator->() const { return ptr; }      // act as I*
//------------------------------------------------------------------------
protected:
	I* ptr;
};

//------------------------------------------------------------------------
template <class I>
inline IPtr<I>::IPtr (I* _ptr, bool addRef)
: ptr (_ptr)
{
	if (ptr && addRef)
		ptr->addRef ();
}

//------------------------------------------------------------------------
template <class I>
inline IPtr<I>::IPtr (const IPtr<I>& other)
: ptr (other.ptr)
{
	if (ptr)
		ptr->addRef ();
}

//------------------------------------------------------------------------
template <class I>
inline IPtr<I>::IPtr ()
: ptr (0)
{}

//------------------------------------------------------------------------
template <class I>
inline IPtr<I>::~IPtr ()
{
	if (ptr)
		ptr->release ();
}

//------------------------------------------------------------------------
template <class I>
inline I* IPtr<I>::operator=(I* _ptr)
{
	if (_ptr != ptr)
	{
		if (ptr)
			ptr->release ();
		ptr = _ptr;
		if (ptr)
			ptr->addRef ();
	}
	return ptr;
}

//------------------------------------------------------------------------
template <class I>
inline IPtr<I>& IPtr<I>::operator=(const IPtr<I>& ptr)
{
	operator= (ptr.ptr);
	return *this;
}


//------------------------------------------------------------------------
/** OPtr - "owning" smart pointer used for newly created FObjects.
\ingroup pluginBase

	FUnknown implementations are supposed to have a refCount of 1 right after creation.
	So using an IPtr on newly created objects would lead to a leak.
	Instead the OPtr can be used in this case. \n
	Example:
								\code
	OPtr<IPath> path = FHostCreate (IPath, hostClasses);
	// no release is needed...
								\endcode
	The assignment operator takes ownership of a new object and releases the old.
	So its safe to write:
								\code
	OPtr<IPath> path = FHostCreate (IPath, hostClasses);
	path = FHostCreate (IPath, hostClasses);
	path = 0;
								\endcode
	This is the difference to using an IPtr with addRef=false.
								\code
	// DONT DO THIS:
	IPtr<IPath> path (FHostCreate (IPath, hostClasses), false);
	path = FHostCreate (IPath, hostClasses);
	path = 0;
								\endcode
	This will lead to a leak!
*/
//------------------------------------------------------------------------
template <class I>
class OPtr : public IPtr<I>
{
public:
//------------------------------------------------------------------------
	inline OPtr (I* p) : IPtr<I> (p, false) {}
	inline OPtr (const IPtr<I>& p) : IPtr<I> (p) {}
	inline OPtr (const OPtr<I>& p) : IPtr<I> (p) {}
	inline OPtr () {}
	inline I* operator=(I* _ptr)
	{
		if (_ptr != this->ptr)
		{
			if (this->ptr)
				this->ptr->release ();
			this->ptr = _ptr;
		}
		return this->ptr;
	}
};

//------------------------------------------------------------------------
/** Assigning newly created object to an IPtr.
	Example:
								\code
	IPtr<IPath> path = owned (FHostCreate (IPath, hostClasses));
								\endcode
	which is a slightly shorter form of writing:
								\code
	IPtr<IPath> path = OPtr<IPath> (FHostCreate (IPath, hostClasses));
								\endcode
*/
template <class I>
IPtr<I> owned (I* p) { return OPtr<I> (p); }


//------------------------------------------------------------------------
// FUnknownPtr
//------------------------------------------------------------------------
/** FUnknownPtr - automatic interface conversion and smart pointer in one.
    This template class can be used for interface conversion like this:
					\code
	OPtr<IPath> path = FHostCreate (IPath, hostClasses);
	FUnknownPtr<IPath2> path2 (path); // does a query interface for IPath2
	if (path2)
		...
					\endcode
*/
//------------------------------------------------------------------------
template <class I>
class FUnknownPtr : public IPtr<I>
{
public:
//------------------------------------------------------------------------
	inline FUnknownPtr (FUnknown* unknown); // query interface
	inline FUnknownPtr (const FUnknownPtr& p) : IPtr<I> (p) {}
	inline FUnknownPtr () {}

	inline FUnknownPtr& operator=(const FUnknownPtr& p) {IPtr<I>::operator=(p); return *this;}
	inline I* operator=(FUnknown* unknown);
	inline I* getInterface () { return this->ptr; }
};

//------------------------------------------------------------------------
template <class I>
inline FUnknownPtr<I>::FUnknownPtr (FUnknown* unknown)
{
	if (unknown && unknown->queryInterface (I::iid, (void**)&this->ptr) != kResultOk)
		this->ptr = 0;
}

//------------------------------------------------------------------------
template <class I>
inline I* FUnknownPtr<I>::operator=(FUnknown* unknown)
{
	I* newPtr = 0;
	if (unknown && unknown->queryInterface (I::iid, (void**)&newPtr) == kResultOk)
	{
		OPtr<I> rel (newPtr);
		return IPtr<I>::operator=(newPtr);
	}

	return IPtr<I>::operator=(0);
}


//------------------------------------------------------------------------
//  FVariant struct declaration
//------------------------------------------------------------------------
/** A Value of variable type.
\ingroup pluginBase
*/
class FVariant
{
//------------------------------------------------------------------------
public:
	enum
	{
		kEmpty   = 0,
		kInteger = 1<<0,
		kFloat   = 1<<1,
		kString8  = 1<<2,
		kObject  = 1<<3,
		kOwner   = 1<<4,
		kString16 = 1<<5
	};

//------------------------------------------------------------------------
	// ctors
	FVariant () { memset (this, 0, sizeof (FVariant)); }
	FVariant (const FVariant& variant);

	FVariant (int64 v)     : type (kInteger), intValue (v)   {}
	FVariant (double v)        : type (kFloat),   floatValue (v) {}
	FVariant (const char8* str) : type (kString8),  string8 (str)   {}
	FVariant (const char16* str) : type (kString16),  string16 (str)   {}
	FVariant (FUnknown* obj, bool owner = false)   : type (kObject),  object (obj)   { setOwner (owner); }
	~FVariant () { empty (); }

//------------------------------------------------------------------------
	FVariant& operator= (const FVariant& variant);

	inline void setInt   (int64 v)    { empty (); type = kInteger; intValue = v;   }
	inline void setFloat (double v)       { empty (); type = kFloat;   floatValue = v; }
	inline void setString8 (const char8* v) { empty (); type = kString8;  string8 = v;     }
	inline void setString16 (const char16* v) { empty (); type = kString16;  string16 = v;    }

	inline void setObject (FUnknown* obj) { empty (); type = kObject;  object = obj;   }

	inline int64 getInt () const		{ return (type & kInteger) ? intValue : 0; }
	inline double getFloat () const	        { return (type & kFloat) ? floatValue : 0.; }
	inline double getNumber () const		{ return (type & kInteger) ? (double)intValue : (type & kFloat) ? floatValue : 0.; }
	inline const char8* getString8 () const	{ return (type & kString8) ? string8 : 0; }
	inline const char16* getString16 () const	{ return (type & kString16) ? string16 : 0; }

	inline FUnknown* getObject () const	    { return (type & kObject) ? object : 0; }

	inline uint16 getType () const { return (uint16)(type & ~(kOwner)); }
	inline bool isEmpty () const           { return getType () == kEmpty; }
	inline bool isOwner () const           { return (type & kOwner) != 0; }
	inline bool isString () const          { return (type & (kString8|kString16)) != 0;}
	inline void setOwner (bool state)      { if (state) type |= kOwner; else type &= ~kOwner; }

	void empty ();
//------------------------------------------------------------------------
	uint16 type;
	union
	{
		int64   intValue;
		double      floatValue;
		const char8* string8;
		const char16* string16;
		FUnknown*   object;
	};
};



//------------------------------------------------------------------------
// FReleaser (obsolete)
//------------------------------------------------------------------------
/** Release an interface using automatic object (obsolete).
This class is obsolete and is only kept for compatibility.
The replacement for FReleaser is OPtr.

Usage example with FReleaser:
										\code
	void someFunction ()
	{
		IPath* path = pathCreateMethod ();
		FReleaser releaser (path);
		.... do something with path...
		.... path not used anymore, releaser will destroy it when leaving function scope
	}
										\endcode
Usage example with OPtr:
										\code
	void someFunction ()
	{
		OPtr<IPath> path = pathCreateMethod ();
		.... do something with path...
		.... path not used anymore, OPtr will destroy it when leaving function scope
	}
										\endcode
*/
//------------------------------------------------------------------------
struct FReleaser
{
	FReleaser (FUnknown* u): u (u) {}
	~FReleaser () { if (u) u->release (); }

	FUnknown* u;
};

}

#endif
