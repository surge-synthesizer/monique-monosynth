/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_15EBFFC85DA080CA__
#define __JUCE_HEADER_15EBFFC85DA080CA__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"
#include "monique_core_Datastructures.h"

//==============================================================================
//==============================================================================
//==============================================================================
class EndlessBuffer : public RuntimeListener
{
protected:
    mono_AudioSampleBuffer<1> sample_buffer;

    int current_size;
    int reader_position;

public:
    inline void write( const float* samples_, int num_samples_ ) noexcept;
    inline void write( const float* samples_, const float* samples_2_, int num_samples_ ) noexcept;

public:
    // TODO you can improve it by coounting the pos one time by a master and not for each painting: also you have it in sync
    inline float get( int pos_ ) const noexcept;
    inline float get_size() const noexcept
    {
        return current_size;
    }
    void clear() noexcept
    {
        sample_buffer.clear();
    }
    int get_reader_start() const noexcept
    {
        return reader_position;
    }
    void set_reader_start( int reader_position_ )  noexcept
    {
        reader_position = reader_position_;
    }

private:
    COLD virtual void sample_rate_changed( double /* old_sr_ */ ) noexcept override;
    COLD virtual void block_size_changed() noexcept override;

public:
    COLD EndlessBuffer( RuntimeNotifyer*const notifyer_ ) noexcept;
    COLD virtual ~EndlessBuffer() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EndlessBuffer)
};
//==============================================================================
inline void EndlessBuffer::write( const float* samples_, int num_samples_ ) noexcept
{
    float*const tmp_sample_buffer = sample_buffer.getWritePointer(0);
    int tmp_position = reader_position;

    if( tmp_position + num_samples_ < current_size )
    {
        FloatVectorOperations::copy( &tmp_sample_buffer[tmp_position], samples_, num_samples_ );
        tmp_position+=num_samples_;
    }
    else
    {
        for( int sid = 0 ; sid != num_samples_ ; ++sid )
        {
            if( ++tmp_position >= current_size )
            {
                tmp_position = 0;
            }

            tmp_sample_buffer[tmp_position] = samples_[sid];
        }
    }

    reader_position = tmp_position;
}
static inline float sample_mix_ui( float s1_, float s2_ ) noexcept
{
    if ((s1_ > 0) && (s2_ > 0))
    {
        s1_ = s1_ + s2_ - (s1_ * s2_);
    }
    else if ((s1_ < 0) && (s2_ < 0))
    {
        s1_ = s1_ + s2_ + (s1_ * s2_);
    }
    else
    {
        s1_ = s1_ + s2_;
    }

    return s1_;
}
inline void EndlessBuffer::write( const float* samples_, const float* samples_2_, int num_samples_ ) noexcept
{
    float*const tmp_sample_buffer = sample_buffer.getWritePointer(0);
    int tmp_position = reader_position;

    {
        for( int sid = 0 ; sid != num_samples_ ; ++sid )
        {
            if( ++tmp_position >= current_size )
            {
                tmp_position = 0;
            }

            tmp_sample_buffer[tmp_position] = sample_mix_ui(samples_[sid],samples_2_[sid]);
        }
    }

    reader_position = tmp_position;
}

//==============================================================================
//==============================================================================
//==============================================================================
class EndlessSwitchBuffer : public EndlessBuffer
{
    mono_AudioSampleBuffer<1> switch_buffer;

public:
    inline void write( const float* samples_, const float* switchs_, int num_samples_ ) noexcept;
    inline int get_new_reader_start_position( int samples_to_paint_ ) const noexcept;
    void clear() noexcept
    {
        FloatVectorOperations::fill(switch_buffer.getWritePointer(),1,switch_buffer.get_size());
        EndlessBuffer::clear();
    }

private:
    COLD void sample_rate_changed( double /* old_sr_ */ ) noexcept override;
    COLD void block_size_changed() noexcept override;

public:
    COLD EndlessSwitchBuffer( RuntimeNotifyer*const notifyer_ ) noexcept;
    COLD ~EndlessSwitchBuffer() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EndlessSwitchBuffer)
};

//==============================================================================
inline void EndlessSwitchBuffer::write( const float* samples_, const float* switchs_, int num_samples_ ) noexcept
{
    float*const tmp_sample_buffer = sample_buffer.getWritePointer();
    float*const tmp_switch_buffer = switch_buffer.getWritePointer();
    int tmp_position = reader_position;

    if( tmp_position + num_samples_ < current_size )
    {
        FloatVectorOperations::copy( &tmp_sample_buffer[tmp_position], samples_, num_samples_ );
        FloatVectorOperations::copy( &tmp_switch_buffer[tmp_position], switchs_, num_samples_ );
        tmp_position+=num_samples_;
    }
    else
    {
        for( int sid = 0 ; sid != num_samples_ ; ++sid )
        {
            if( ++tmp_position >= current_size )
            {
                tmp_position = 0;
            }

            tmp_sample_buffer[tmp_position] = samples_[sid];
            tmp_switch_buffer[tmp_position] = switchs_[sid];
        }
    }


    reader_position = tmp_position;
}
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_AmpPainter  : public Component,
    public Timer,
    public SliderListener,
    public ButtonListener
{
public:
    //==============================================================================
    Monique_Ui_AmpPainter (MoniqueSynthData* synth_data_, UiLookAndFeel*look_and_feel_);
    ~Monique_Ui_AmpPainter();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const float original_w;
    const float original_h;

private:
    int current_buffer_start_pos;
    UiLookAndFeel*const look_and_feel;
    MoniqueSynthData*const synth_data;
    OwnedArray<EndlessBuffer> filter_values;
    OwnedArray<EndlessBuffer> filter_env_values;
    ScopedPointer< EndlessBuffer > eq_values;
    ScopedPointer< EndlessBuffer > values_env;
    ScopedPointer< EndlessBuffer > values;
    ScopedPointer< EndlessSwitchBuffer > master_osc_values;
    OwnedArray<EndlessBuffer> osc_values;

    Array<EndlessBuffer*> buffers;

public:
    inline void calc_new_cycle() noexcept;
    inline void add_filter( int id_, const float* values_l_,const float* values_r_, int num_samples_ ) noexcept;
    inline void add_filter_env( int id_, const float* values_, int num_samples_ ) noexcept;
    inline void add_eq( const float* values_, int num_samples_ ) noexcept;
    inline void add_out_env( const float* values_, int num_samples_ ) noexcept;
    inline void add_out( const float* values_l_, const float* values_r_, int num_samples_ ) noexcept;
    inline void add_master_osc( const float* values_, const float* is_switch_values, int num_samples_ ) noexcept;
    inline void add_osc( int id_, const float* values_, int num_samples_ ) noexcept;
    inline void clear_and_keep_minimum() noexcept;

private:
    void timerCallback() override;

    void refresh_buttons();

    bool is_currently_painting;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> sl_show_range;
    ScopedPointer<TextButton> osc_1;
    ScopedPointer<TextButton> osc_2;
    ScopedPointer<TextButton> osc_3;
    ScopedPointer<TextButton> eq;
    ScopedPointer<TextButton> out;
    ScopedPointer<TextButton> f_1;
    ScopedPointer<TextButton> f_2;
    ScopedPointer<TextButton> f_3;
    ScopedPointer<TextButton> f_env_1;
    ScopedPointer<TextButton> f_env_2;
    ScopedPointer<TextButton> f_env_3;
    ScopedPointer<TextButton> out_env;
    ScopedPointer<Component> drawing_area;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_AmpPainter)
};

//[EndFile] You can add extra defines here...
inline void Monique_Ui_AmpPainter::calc_new_cycle() noexcept
{
    current_buffer_start_pos = master_osc_values->get_reader_start();
}

inline void Monique_Ui_AmpPainter::add_filter_env(int id_, const float* values_, int num_samples_) noexcept
{
    //if( id_ == 0 and synth_data->osci_show_flt_env_1 or id_ == 1 and synth_data->osci_show_flt_env_2 or id_ == 2 and synth_data->osci_show_flt_env_3 )
    {
        EndlessBuffer*const values = filter_env_values.getUnchecked(id_);
        values->set_reader_start(current_buffer_start_pos);
        values->write( values_, num_samples_ );
    }
}
inline void Monique_Ui_AmpPainter::add_filter(int id_, const float* values_l_, const float* values_r_, int num_samples_) noexcept
{
    //if( id_ == 0 and synth_data->osci_show_flt_1 or id_ == 1 and synth_data->osci_show_flt_2 or id_ == 2 and synth_data->osci_show_flt_3 )
    {
        EndlessBuffer*const values = filter_values.getUnchecked(id_);
        values->set_reader_start(current_buffer_start_pos);
        values->write( values_l_, values_r_, num_samples_ );
    }
}
inline void Monique_Ui_AmpPainter::add_eq( const float* values_, int num_samples_ ) noexcept
{
    //if( synth_data->osci_show_eq )
    {
        eq_values->set_reader_start(current_buffer_start_pos);
        eq_values->write( values_, num_samples_ );
    }
}
inline void Monique_Ui_AmpPainter::add_out_env( const float* values_, int num_samples_ ) noexcept
{
    //if( synth_data->osci_show_out_env )
    {
        values_env->set_reader_start(current_buffer_start_pos);
        values_env->write( values_, num_samples_ );
    }
}
inline void Monique_Ui_AmpPainter::add_out( const float* values_l_, const float* values_r_, int num_samples_ ) noexcept
{
    //if( synth_data->osci_show_out )
    {
        values->set_reader_start(current_buffer_start_pos);
        values->write( values_l_, values_r_, num_samples_ );
    }
}
inline void Monique_Ui_AmpPainter::add_master_osc( const float* values_, const float* is_switch_values, int num_samples_ ) noexcept
{
    //if( id_ == 0 or id_ == 1 and synth_data->osci_show_osc_2 or id_ == 2 and synth_data->osci_show_osc_3 )
    {
        master_osc_values->write( values_, is_switch_values, num_samples_ );
    }
}
inline void Monique_Ui_AmpPainter::add_osc( int id_, const float* values_, int num_samples_ ) noexcept
{
    //if( id_ == 0 or id_ == 1 and synth_data->osci_show_osc_2 or id_ == 2 and synth_data->osci_show_osc_3 )
    {
        EndlessBuffer*const osc_values_ = osc_values.getUnchecked(id_-1);
        osc_values_->set_reader_start(current_buffer_start_pos);
        osc_values_->write( values_, num_samples_ );
    }
}
inline void Monique_Ui_AmpPainter::clear_and_keep_minimum() noexcept
{
    values->clear();
    values_env->clear();
    eq_values->clear();
    filter_values.getUnchecked(0)->clear();
    filter_values.getUnchecked(1)->clear();
    filter_values.getUnchecked(2)->clear();
    filter_env_values.getUnchecked(0)->clear();
    filter_env_values.getUnchecked(1)->clear();
    filter_env_values.getUnchecked(2)->clear();
}
//[/EndFile]

#endif   // __JUCE_HEADER_15EBFFC85DA080CA__

