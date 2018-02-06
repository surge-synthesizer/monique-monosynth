//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/logevents.h
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

#ifndef __logevents__
#define __logevents__

#include "pluginterfaces/base/ftypes.h"
#include <map>
#include "base/source/fstring.h"

//------------------------------------------------------------------------
struct LogEvent
{
	LogEvent () 
	: id (-1)
	, count (0) {}

	Steinberg::int64 id;
	Steinberg::int64 count;
};

//------------------------------------------------------------------------
// Categories
#define AUDIO_BUFFER "AudioBuffer"
#define EVENT_LIST "EventList"
#define PARAM_CHANGE "ParameterChanges"
#define PROCESS_DATA "ProcessData"
#define PROCESS_CONTEXT "ProcessContext"

// Severity
#define LOG_ERR "Error"
#define LOG_WARN "Warn"
#define LOG_INFO "Info"

#define LOG_EVENT_LIST(LOG_DEF) \
	LOG_DEF(kLogIdInvalidSymbolicSampleSize,		LOG_ERR, PROCESS_DATA, "Symbolic sample size does not match the one in ProcessSetup"),\
	LOG_DEF(kLogIdInvalidProcessMode,				LOG_ERR, PROCESS_DATA, "Process mode does not match the one in ProcessSetup."),\
	LOG_DEF(kLogIdInvalidBlockSize,					LOG_ERR, PROCESS_DATA, "Block size is either < 1 or >= max block size."),\
	LOG_DEF(kLogIdProcessContextPointerNull,		LOG_WARN,	PROCESS_DATA, "Pointer to ProcessContext struct is null."),\
	LOG_DEF(kLogIdNullPointerToChannelBuf,			LOG_ERR, AUDIO_BUFFER, "A pointer to a channel buffer is null although the index is valid."),\
	LOG_DEF(kLogIdNullPointerToAudioBusBuffer,		LOG_ERR, AUDIO_BUFFER, "A pointer to an audio bus buffer is null although the index is valid."),\
	LOG_DEF(kLogIdAudioBufNotMatchComponentBusCount, LOG_ERR, AUDIO_BUFFER, "Number of Audio Buffers does not match the number of buses defined by IComponent."),\
	LOG_DEF(kLogIdInvalidAudioBufNumOfChannels,		LOG_ERR, AUDIO_BUFFER, "An audio bus has a different number of channels than specified by IComponent."),\
	LOG_DEF(kLogIdUnknownEventType,					LOG_ERR, EVENT_LIST, "Event has a type which is not specified."),\
	LOG_DEF(kLogIdInvalidEventVelocityValue,		LOG_ERR, EVENT_LIST, "Event velocity is either < 0.0 or > 1.0."),\
	LOG_DEF(kLogIdInvalidEventPitchValue,			LOG_ERR, EVENT_LIST, "Event pitch is either < 0 or > 127."),\
	LOG_DEF(kLogIdInvalidEventSampleOffset,			LOG_ERR, EVENT_LIST, "Event sample offset either < 0 or >= max block size."),\
	LOG_DEF(kLogIdInvalidEventBusIndex,				LOG_ERR, EVENT_LIST, "Event has a bus index which is different than specified by IComponent."),\
	LOG_DEF(kLogIdInvalidNoteOnChannelIndex,		LOG_ERR, EVENT_LIST, "Note on event has a channel index which was not specified by IComponent."),\
	LOG_DEF(kLogIdInvalidNoteOffChannelIndex,		LOG_ERR, EVENT_LIST, "Note off event has a channel index which was not specified by IComponent."),\
	LOG_DEF(kLogIdInvalidPolyPressChannelIndex,		LOG_ERR, EVENT_LIST, "Poly pressure event has a channel index which was not specified by IComponent."),\
	LOG_DEF(kLogIdNumInputEventExceedsLimit,		LOG_WARN, EVENT_LIST, "List contains more than 2048 events."),\
	LOG_DEF(kLogIdCouldNotGetAnInputEvent,			LOG_WARN, EVENT_LIST, "Getting an event returned an error code."),\
	LOG_DEF(kLogIdEventsAreNotSortedBySampleOffset,	LOG_WARN, EVENT_LIST, "Events are not sorted by sample offset."),\
	LOG_DEF(kLogIdEventsAreNotSortedByPpqPosition,	LOG_WARN, EVENT_LIST, "Events are not sorted by PPQ position."),\
	LOG_DEF(kLogIdNoteOnWithPitchAlreadyTriggered,	LOG_INFO, EVENT_LIST, "An event occurred with a pitch currently already triggered."),\
	LOG_DEF(kLogIdNoteOnWithIdAlreadyTriggered,		LOG_WARN, EVENT_LIST, "An event occurred with an ID currently already triggered."),\
	LOG_DEF(kLogIdNoteOffWithIdNeverTriggered,		LOG_WARN, EVENT_LIST, "A note off event with no matching note on (ID)"),\
	LOG_DEF(kLogIdNoteOffWithPitchNeverTriggered,	LOG_WARN, EVENT_LIST, "A note off event with no matching note on (pitch)."),\
	LOG_DEF(kLogIdNoteExpressValNotNormalized,		LOG_ERR, EVENT_LIST,  "A note expression event value is either < 0.0 or > 1.0."),\
	LOG_DEF(kLogIdInvalidProcessContextSampleRate,	LOG_ERR, PROCESS_CONTEXT, "The sampleRate does not match the one in ProcessSetup."),\
	LOG_DEF(kLogIdInvalidParamValue,				LOG_ERR, PARAM_CHANGE, "Parameter value is < 0.0 or > 1.0"),\
	LOG_DEF(kLogIdInvalidParameterCount,			LOG_ERR, PARAM_CHANGE, "The number of changes is bigger than the number of parameters specified by IEditController."),\
	LOG_DEF(kLogIdInvalidParameterID,				LOG_ERR, PARAM_CHANGE, "A parameter change queue has a parameter ID which was not specified by IEditController."),\
	LOG_DEF(kLogIdParameterChangesPointerIsNull,	LOG_WARN, PARAM_CHANGE, "Pointer to parameter changes interface is null."),\
	LOG_DEF(kLogIdParameterQueueIsNullForValidIndex, LOG_ERR, PARAM_CHANGE, "Pointer to parameter value queue interface is null, although index is valid."),\
	LOG_DEF(kLogIdParametersAreNotSortedBySampleOffset, LOG_ERR, PARAM_CHANGE, "Parameter changes (for a ID) are not sorted by sample offset."),\
	LOG_DEF(kLogIdParametersHaveSameSampleOffset, LOG_WARN, PARAM_CHANGE, "Parameter changes (for a ID) have more than one time the same sample offset.")
#define LOG_ID(a, b, c, d) a
#define LOG_SEVER(a, b, c, d) b
#define LOG_DESC(a, b, c, d) ("["c"] "d) // "[category] description"

// enum of all IDs
enum eLogIds
{
	LOG_EVENT_LIST(LOG_ID),
	kNumLogEvents
};

// array of log descriptions
static const char *logEventDescriptions[] =
{
	LOG_EVENT_LIST(LOG_DESC)
};

// array of string 'error' or 'warning'
static const char* logEventSeverity[] =
{
	LOG_EVENT_LIST(LOG_SEVER)
};

#endif //__logevents__
