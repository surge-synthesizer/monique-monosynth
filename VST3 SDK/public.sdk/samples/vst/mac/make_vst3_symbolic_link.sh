#!/bin/sh
#------------------------------------------------------------------------
# Project     : VST SDK
# Version     : 3.0
#
# Category    : Examples
# Filename    : public.sdk/samples/vst/mac/make_vst3_symbolic_link.sh
# Created by  : Steinberg, 06/2009
# Description : places a symbolic link for the VST3 Plug-In into the users VST3 folder. To be used as "Run Script" build phase in Xcode
#-----------------------------------------------------------------------------
# LICENSE
# (c) 2012, Steinberg Media Technologies GmbH, All Rights Reserved
#-----------------------------------------------------------------------------
# This Software Development Kit may not be distributed in parts or its entirety  
# without prior written agreement by Steinberg Media Technologies GmbH. 
# This SDK must not be used to re-engineer or manipulate any technology used  
# in any Steinberg or Third-party application or software module, 
# unless permitted by law.
# Neither the name of the Steinberg Media Technologies nor the names of its
# contributors may be used to endorse or promote products derived from this 
# software without specific prior written permission.
# 
# THIS SDK IS PROVIDED BY STEINBERG MEDIA TECHNOLOGIES GMBH "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL STEINBERG MEDIA TECHNOLOGIES GMBH BE LIABLE FOR ANY DIRECT, 
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.
#-----------------------------------------------------------------------------

if [[ "$CONFIGURATION" == "Debug" ]]
then
	mkdir -p ~/Library/Audio/Plug-Ins/VST3/
	cd ~/Library/Audio/Plug-Ins/VST3/
	ln -f -s "$BUILT_PRODUCTS_DIR/$WRAPPER_NAME" .
fi
