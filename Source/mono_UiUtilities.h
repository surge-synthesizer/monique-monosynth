/*
  ==============================================================================

    mono_UiUtilities.h
    Created: 24 Apr 2015 5:13:35pm
    Author:  monotomy

  ==============================================================================
*/

#ifndef MONO_UIUTILITIES_H_INCLUDED
#define MONO_UIUTILITIES_H_INCLUDED

#include "App.h"

// ==============================================================================
// NOTE: this class is not designed for usage in time critical code.
// It's designed for UI usage and other low important stuff
#define DONT_OVERRIDE_SLIDER_VALUE -99999

class ParameterReference {
    mono_ParameterCompatibilityBase*const _base;

public:
    operator bool() const;
    bool operator !=( const ParameterReference& other_ ) const {
      return other_._base != _base;
    }
    mono_ParameterCompatibilityBase* get_param() const {
     return _base;
    }
  
    void init_value_slider( Slider*const slider_, float = DONT_OVERRIDE_SLIDER_VALUE ) const;
    void write_value_to( Slider*const slider_ ) const;
    void read_value_from ( const Slider*const slider_ );
    float get_value() const;
    void invert();
    
    void init_modulation_slider( Slider*const slider_, bool is_modulation_slider_centered_ ) const;
    void write_modulation_to( Slider*const slider_, bool is_modulation_slider_centered_ ) const;
    void read_modulation_from ( const Slider*const slider_ );
    float get_modulation() const;
    float get_last_modulation() const;
    bool has_modulation() const;

    ParameterReference( mono_ParameterCompatibilityBase*const param_compatibility_base_ );
};



#endif  // MONO_UIUTILITIES_H_INCLUDED
