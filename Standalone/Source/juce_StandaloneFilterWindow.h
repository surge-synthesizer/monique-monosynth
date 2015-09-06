#ifndef JUCE_STANDALONEFILTERWINDOW_H_INCLUDED
#define JUCE_STANDALONEFILTERWINDOW_H_INCLUDED

#include "../../Source/monique_core_Processor.h"
#include "../../Source/monique_core_Synth.h"
#include "../../Source/monique_ui_MainWindow.h"

extern AudioProcessor* JUCE_CALLTYPE createPluginFilter();

//==============================================================================
class StandaloneFilterWindow : public DocumentWindow
{
public:
    StandaloneFilterWindow(const String& title) noexcept
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
        setContentOwned(reinterpret_cast<Monique_Ui_Mainwindow*>( filter->createEditorIfNeeded()), true );
#endif
    }

    ~StandaloneFilterWindow() noexcept
    {
        deleteFilter();
    }

    //==============================================================================
    AudioProcessor* getAudioProcessor() const noexcept
    {
        return filter;
    }

    void createFilter() noexcept
    {
        AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
        filter = reinterpret_cast<MoniqueAudioProcessor*>( createPluginFilter() );
    }

    void resetFilter() noexcept
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

    void suspended() noexcept
    {
        Monique_Ui_Refresher::getInstance()->stopTimer();
    }
    void resumed() noexcept
    {
        Monique_Ui_Refresher::getInstance()->startTimer( UI_REFRESH_RATE );
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

    void minimisationStateChanged( bool isNowMinimised ) override
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

    void deleteFilter() noexcept
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



