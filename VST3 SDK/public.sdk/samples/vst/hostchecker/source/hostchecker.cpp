//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/hostchecker.cpp
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

#include "hostchecker.h"
#include "eventlogdatabrowsersource.h"

#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/ibstream.h"
#include "public.sdk/source/vst/vstcomponentbase.h"

#include "logevents.h"

using namespace VSTGUI;

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
FUID HostCheckerController::cid (0x35AC5652, 0xC7D24CB1, 0xB1427D38, 0xEB690DAF);
FUID HostCheckerProcessor::cid (0x23FC190E, 0x02DD4499, 0xA8D2230E, 0x50617DA3);

static ProgramListID kProgramListID = 0;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::initialize (FUnknown* context)
{
	tresult result = EditControllerEx1::initialize (context);
	if (result == kResultOk)
	{
		parameters.addParameter (STR16 ("Param1"), STR16 (""));
		parameters.addParameter (STR16 ("Param2"), STR16 (""));
		parameters.addParameter (STR16 ("Param3"), STR16 (""));
	
		parameters.addParameter (STR16 ("Bypass"), STR16 (""), 1, 0, ParameterInfo::kCanAutomate|ParameterInfo::kIsBypass);

		mDataBrowser = 0;
		mDataSource = 0;
	}
	return result;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::terminate ()
{
	tresult result = EditControllerEx1::terminate ();
	if (result == kResultOk)
	{
		if (mDataSource)
			mDataSource->forget ();

		if (mDataBrowser)
			mDataBrowser->forget ();
	}
	return result;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::setComponentState (IBStream* state)
{
	float saved = 0.f;
	if (state->read (&saved, sizeof (float)) != kResultOk)
	{
		return kResultFalse;
	}

#if BYTEORDER == kBigEndian
	SWAP_32 (toSave)
#endif
	if (saved != 12345.67f)
	{
		ASSERT(false)
	}
	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getUnitByBus (MediaType type, BusDirection dir, int32 busIndex, int32 channel, UnitID& unitId /*out*/)
{
	if (type == kEvent && dir == kInput)
	{
		if (busIndex == 0 && channel == 0)
		{
			unitId = kRootUnitId;
			return kResultTrue;
		}
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
IPlugView* PLUGIN_API HostCheckerController::createView (FIDString name)
{
	if (ConstString (name) == ViewType::kEditor)
	{
		return new VST3Editor (this, "HostCheckerEditor", "hostchecker.uidesc");
	}
	return 0;
}

//-----------------------------------------------------------------------------
CView* HostCheckerController::createCustomView (UTF8StringPtr name, const UIAttributes& attributes, IUIDescription* description, VST3Editor* editor)
{
	if (ConstString (name) == "HostCheckerDataBrowser")
	{
		if (!mDataSource)
			mDataSource = new EventLogDataBrowserSource (this);

		if (!mDataBrowser)
			mDataBrowser = new CDataBrowser (CRect (0, 0, 100, 100), 0, mDataSource, CDataBrowser::kDrawRowLines|CDataBrowser::kDrawColumnLines|CDataBrowser::kDrawHeader|CDataBrowser::kVerticalScrollbar);

		mDataBrowser->remember ();
		return mDataBrowser;
	}
	return 0;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::connect (IConnectionPoint* other)
{
	Steinberg::tresult tResult = ComponentBase::connect (other);
	if (peerConnection)
	{
		for (Steinberg::int32 paramIdx = 0; paramIdx < getParameterCount (); ++paramIdx)
		{
			Steinberg::Vst::ParameterInfo paramInfo = {0};
			if (getParameterInfo (paramIdx, paramInfo) == Steinberg::kResultOk)
			{
				IMessage* newMsg = allocateMessage ();
				if (newMsg)
				{
					newMsg->setMessageID ("Parameter");
					Steinberg::Vst::IAttributeList* attr = newMsg->getAttributes ();
					if (attr)
					{
						attr->setInt ("ID", paramInfo.id);
					}

					sendMessage (newMsg);
				}
			}
		}
	}
	
	return tResult;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::notify (IMessage* message)
{
	if (!message)
		return kInvalidArgument;

	if (!strcmp (message->getMessageID (), "LogEvent"))
	{
		LogEvent logEvt;
		if (message->getAttributes ()->getInt ("ID", logEvt.id) != kResultOk)
			return kResultFalse;
		if (message->getAttributes ()->getInt ("Count", logEvt.count) != kResultOk)
			return kResultFalse;

		if (mDataSource && mDataBrowser && mDataSource->updateLog (logEvt))
			mDataBrowser->invalidateRow (logEvt.id);
	}

	return ComponentBase::notify (message);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
HostCheckerProcessor::HostCheckerProcessor ()
{
	setControllerClass (HostCheckerController::cid);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::initialize (FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result == kResultOk)
	{
		addAudioOutput (USTRING("Audio Output"), SpeakerArr::kStereo);
		addEventInput (USTRING("Event Input"), 1);

		mHostCheck.setComponent (this);
	}
	return result;
}

//-----------------------------------------------------------------------------
void HostCheckerProcessor::sendLogEventMessage (const LogEvent& logEvent)
{
	IMessage* message = allocateMessage ();
	FReleaser msgReleaser (message);
	if (message)
	{
		message->setMessageID ("LogEvent");
		IAttributeList* attributes = message->getAttributes ();
		if (attributes)
		{
			ASSERT (logEvent.id >= 0);
			attributes->setInt ("ID", logEvent.id);
			attributes->setInt ("Count", logEvent.count);
			sendMessage (message);
		}
	}
}
//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::process (ProcessData& data)
{
	mHostCheck.validate (data);

	if (data.numSamples && data.numOutputs)
	{
		for (int32 i = 0; i < data.outputs[0].numChannels; i++)
			memset (data.outputs[0].channelBuffers32[i], 0, data.numSamples * sizeof (float));
		data.outputs[0].silenceFlags = 0x3;
	}

	const EventLogger::Codes& errors = mHostCheck.getEventLogs ();
	EventLogger::Codes::const_iterator iter = errors.begin ();
	while (iter != errors.end ())
	{
		sendLogEventMessage ((*iter));
		++iter;
	}
	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::setupProcessing (ProcessSetup& setup)
{
	mHostCheck.setProcessSetup (setup);
	return AudioEffect::setupProcessing (setup);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::notify (IMessage* message)
{
	if (!message)
		return kInvalidArgument;

	if (!strcmp (message->getMessageID (), "Parameter"))
	{
		int64 paramId = -1;
		if (message->getAttributes ()->getInt ("ID", paramId) == kResultOk)
		{
			mHostCheck.addParameter (paramId);
		}
	}

	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::setState (IBStream* state)
{
	float saved = 0.f;
	if (state->read (&saved, sizeof (float)) != kResultOk)
	{
		return kResultFalse;
	}

#if BYTEORDER == kBigEndian
	SWAP_32 (toSave)
#endif
	if (saved != 12345.67f)
	{
		ASSERT(false)
	}
	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::getState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	float toSave = 12345.67f;

#if BYTEORDER == kBigEndian
	SWAP_32 (toSave)
#endif

	state->write (&toSave, sizeof (float));

	return kResultOk;
}

//-----------------------------------------------------------------------------
}} // namespaces
