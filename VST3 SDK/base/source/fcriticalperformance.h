//------------------------------------------------------------------------
// Project     : SDK Base
// Version     : 1.0
//
// Category    : Helpers
// Filename    : base/source/fcriticalperformance.h
// Created by  : Steinberg, 2013
// Description : Critical Performance Mode for Realtime Threads
//
//               Note: When using this file, add it to your own project and 
//               not to "base" because you'll also need to include the Core 
//               Audio library on Mac.
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

//----------------------------------------------------------------------------------
/** @file base/source/fcriticalperformance.h
	Critical Performance Mode for Realtime Threads. */
//----------------------------------------------------------------------------------
#ifndef __fcriticalperformance__
#define __fcriticalperformance__

#include "pluginterfaces/base/ftypes.h"

namespace Steinberg {

//------------------------------------------------------------------------
/** FCriticalPerformanceEnabler
 *
 * Singleton for using Critical Performance Mode. */
//------------------------------------------------------------------------
class FCriticalPerformanceEnabler
{
public:
	//--------------------------------------------------------------------
	/** Call this to get Singleton access. */
	static FCriticalPerformanceEnabler& getInstance ();

	/** On Windows systems, this function associates the calling thread with the Windows Vista Pro Audio task.
	 *  On Mac systems, it uses the THREAD_TIME_CONSTRAINT_POLICY for the calling thread.
	 *  Use "criticalPriority" for maximum priority.
	 *  Windows: It returns the handle to the associated pro audio task. This handle must be used for subsequent calls - 
	 *  otherwise the thread priority might not be set correctly!
	 *  Mac: Disabling is not supported. You'll need to terminate the tread (and create a new one if wanted).
	 *  Note: this method must be called within the thread that should be associated, 
	 *  because the priority of the calling thread will be changed!
	 *  WARNING: On Windows, calling this method may release the cpu affinity! If needed, set the affinity afterwards again.
	 *  @param enable enable state
	 *  @param criticalPriority critical priority state
	 *  @param timeConstraintNanoSeconds time constraint for THREAD_TIME_CONSTRAINT_POLICY (Mac only)
	 *  @param handle see description (Win only)
	 *  @return see description (Win only)
	 */
	void* useCriticalPerformanceMode (bool enable, bool criticalPriority, uint64 timeConstraintNanoSeconds, void* handle);
		
	/** Checks if Windows Vista Pro Audio task is supported. (win only)
	 *  @return true if supported (Mac always returns true)
	 */
	bool isCriticalPerformanceModeSupported ();
	//--------------------------------------------------------------------

protected:
	FCriticalPerformanceEnabler ();
	~FCriticalPerformanceEnabler ();
};

} // namespace Steinberg

#endif
