/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef JUCE_STANDALONEFILTERWINDOW_H_INCLUDED
#define JUCE_STANDALONEFILTERWINDOW_H_INCLUDED

#include "../../Source/PluginProcessor.h"
#include "../../Source/monique_core_Synth.h"
#include "../../Source/UiEditorSynthLite.h"

extern AudioProcessor* JUCE_CALLTYPE createPluginFilter();

//==============================================================================
class StandaloneFilterWindow : public DocumentWindow
{
public:
    StandaloneFilterWindow(const String& title)
        :
        DocumentWindow(title, Colour(0xff000000), DocumentWindow::allButtons, false )
    {
        setOpaque(true);
        createFilter();

        if (filter == nullptr)
        {
            JUCEApplicationBase::quit();
        }

#ifndef PROFILE
        setContentOwned(reinterpret_cast<UiEditorSynthLite*>( filter->createEditorIfNeeded()), true );
#endif
    }

    ~StandaloneFilterWindow()
    {
        deleteFilter();
    }

    //==============================================================================
    AudioProcessor* getAudioProcessor() const noexcept
    {
        return filter;
    }

    void createFilter()
    {
        AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
        filter = reinterpret_cast<MoniqueAudioProcessor*>( createPluginFilter() );
    }

    void resetFilter()
    {
        deleteFilter();
        createFilter();

        if (filter != nullptr)
        {
            setContentOwned (filter->createEditorIfNeeded(), true);
        }
    }


    //==============================================================================
    void closeButtonPressed() override
    {
        JUCEApplicationBase::quit();
    }

    void maximiseButtonPressed() override
    {
        if( isResizable() )
        {
            setResizable(false,false);
            DocumentWindow::maximiseButtonPressed();
        }
        else
        {
            setResizable(true,true);
            DocumentWindow::maximiseButtonPressed();
        }
    }

    void resized() override
    {
        DocumentWindow::resized();
    }

    void suspended()
    {
        mono_UiRefresher::getInstance()->stopTimer();
    }
    void resumed()
    {
        mono_UiRefresher::getInstance()->startTimer( UI_REFRESH_RATE );
    }

    void visibilityChanged() override
    {
        if( isVisible() )
        {
            resumed();
        }
        else
        {
            suspended();
        }
    }

    void minimisationStateChanged( bool isNowMinimised) override
    {
        if( isNowMinimised )
        {
            suspended();
        }
        else
        {
            resumed();
        }
    }

private:
    //==============================================================================
    ScopedPointer<MoniqueAudioProcessor> filter;

    void deleteFilter()
    {
        if (filter != nullptr && getContentComponent() != nullptr)
        {
            filter->editorBeingDeleted (dynamic_cast <AudioProcessorEditor*> (getContentComponent()));
            clearContentComponent();
        }

        filter = nullptr;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow)
};

#endif   // JUCE_STANDALONEFILTERWINDOW_H_INCLUDED



