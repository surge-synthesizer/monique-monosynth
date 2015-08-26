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
#include "../../Source/Synth.h"
#include "../../Source/UiEditorSynthLite.h"

extern AudioProcessor* JUCE_CALLTYPE createPluginFilter();

//==============================================================================
/**
    A class that can be used to run a simple standalone application containing your filter.

    Just create one of these objects in your JUCEApplicationBase::initialise() method, and
    let it do its work. It will create your filter object using the same createFilter() function
    that the other plugin wrappers use.
*/

class StandaloneFilterWindow : public DocumentWindow
{
public:
    //==============================================================================
    /** Creates a window with a given title and colour.
    	The settings object can be a PropertySet that the class should use to
    	store its settings - the object that is passed-in will be owned by this
    	class and deleted automatically when no longer needed. (It can also be null)
    	*/

    StandaloneFilterWindow(const String& title)
        :
        DocumentWindow(title, Colour(0xff000000), DocumentWindow::allButtons, false ),
        editor(nullptr)
    {
        setOpaque(true);
        createFilter();

        if (filter == nullptr)
        {
            //jassertfalse    // Your filter didn't create correctly! In a standalone app that's not too great.
            JUCEApplicationBase::quit();
        }

#ifdef IS_MOBILE_APP
        addToDesktop();
        setUsingNativeTitleBar(true);
        setResizable(false,false);
        setTitleBarHeight(0);
        setFullScreen(true);
#endif

        editor = dynamic_cast<UiEditorSynthLite*>(filter->createEditorIfNeeded());
        store = editor->_app_instance_store;
        setContentOwned(editor, true);
	
#if IS_MOBILE_APP
        Desktop::getInstance().setKioskModeComponent(this);
        editor->timerCallback();
        editor->startTimer(UI_REFRESH_RATE);
#endif
    }

    ~StandaloneFilterWindow() {
        deleteFilter();
    }

    //

    //==============================================================================
    AudioProcessor* getAudioProcessor() const noexcept      {
        return filter;
    }

    void createFilter()
    {
        AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
        filter = reinterpret_cast<MoniqueAudioProcessor*>( createPluginFilter() );

#ifdef USE_PLUGIN_PROCESS_BLOCK
        AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Undefined);
#endif
    }

    /** Deletes and re-creates the filter and its UI. */
    void resetFilter()
    {
        deleteFilter();
        createFilter();

        if (filter != nullptr) {
            setContentOwned (filter->createEditorIfNeeded(), true);
        }
    }


    //==============================================================================
    /** @internal */
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
    /** @internal */
    void resized() override
    {
        DocumentWindow::resized();
    }

    void suspended()
    {
        //sutdown_audio();
        //editor->stopTimer();
    }
    void resumed()
    {
        //editor->startTimer(1000.0f/30);
    }

    void visibilityChanged() override
    {
        if( isVisible() )
            resumed();
        else
            suspended();
    }

    void minimisationStateChanged( bool isNowMinimised) override
    {
        if( ! isNowMinimised )
            resumed();
        else
            suspended();
    }

private:
    //==============================================================================
    ScopedPointer<MoniqueAudioProcessor> filter;

public:
    UiEditorSynthLite* editor;
    AppInstanceStore*store;
private:
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



