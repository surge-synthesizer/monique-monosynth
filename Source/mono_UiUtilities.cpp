/*
  ==============================================================================

    mono_UiUtilities.cpp
    Created: 24 Apr 2015 5:13:35pm
    Author:  monotomy

  ==============================================================================
*/

#include "mono_UiUtilities.h"
#include "UiLookAndFeel.h"

// ==============================================================================
ParameterReference::ParameterReference( mono_ParameterCompatibilityBase*const param_compatibility_base_ ) : _base( param_compatibility_base_ ) {}

ParameterReference::operator bool() const {
    return _base;
}

void ParameterReference::init_value_slider( Slider*const slider_, float min_value_override ) const {
    if( min_value_override == DONT_OVERRIDE_SLIDER_VALUE )
        slider_->setRange( _base->min_unscaled(), _base->max_unscaled(), _base->slider_interval() );
    else
        slider_->setRange( min_value_override, _base->max_unscaled(), _base->slider_interval() );

    slider_->setDoubleClickReturnValue( true, _base->reset_unscaled() );
    slider_->setPopupMenuEnabled( true );
    write_value_to( slider_ );
}
void ParameterReference::write_value_to( Slider*const slider_ ) const {
    slider_->setValue( _base->get_scaled_value()*_base->scale() ,dontSendNotification );
}
void ParameterReference::read_value_from( const Slider*const slider_ ) {
    _base->set_scaled_value( slider_->getValue()/_base->scale() );
}
float ParameterReference::get_value() const {
    return _base->get_scaled_value()/_base->scale();
}
void ParameterReference::invert () {
    _base->set_scaled_value( !_base->get_scaled_value() );
}
void ParameterReference::init_modulation_slider( Slider*const slider_, bool is_modulation_slider_centered_ ) const {
    slider_->setRange( -100, MODULATION_AMOUNT_MAX, 0.1 );
    slider_->setDoubleClickReturnValue( true, 0 );
    slider_->setPopupMenuEnabled( true );
    write_modulation_to( slider_, is_modulation_slider_centered_ );
}
void ParameterReference::write_modulation_to( Slider*const slider_, bool is_modulation_slider_centered_ ) const {
    const float modulation_value = _base->get_modulation_amount();
    float scaled_value = _base->get_scaled_value();
    const float scale = _base->scale();
    if( scale != 1000 )
    {
        if( scaled_value >= 0 )
            scaled_value /= _base->max_unscaled();
        else
            scaled_value /= _base->min_unscaled();
    }

    int modulation_slider_style;
    if( is_modulation_slider_centered_ )
        modulation_slider_style = MODULATION_SLIDER_IS_FIXED_CENTERED;
    else if( _base->min_unscaled() < 0 )
        modulation_slider_style = MODULATION_SLIDER_MOVES_WITH_MASTER;
    else
        modulation_slider_style = MODULATION_SLIDER_MOVES_WITH_MASTER_FROM_ZERO;

    slider_->setRotaryParameters( scaled_value, modulation_slider_style, true );
    slider_->setValue( modulation_value*MODULATION_AMOUNT_MAX ,dontSendNotification );
}
void ParameterReference::read_modulation_from ( const Slider*const slider_ ) {
    _base->set_modulation_amount( slider_->getValue() / MODULATION_AMOUNT_MAX );
}

float ParameterReference::get_modulation() const {
    return _base->get_modulation_amount();
}
float ParameterReference::get_last_modulation() const {
    return _base->get_last_modulation_amount();
}
bool ParameterReference::has_modulation() const {
    return _base->has_modulation();
}
