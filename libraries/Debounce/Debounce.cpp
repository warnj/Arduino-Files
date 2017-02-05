/*********************
*	Debounce.cpp
*	
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-APR-19
*
*	Purpose: Configurable debounce of digital inputs.
*
*/

/*********************
*	Revision History
*   Version|Date      |Who|Notes
*   -------|----------|---|------------------
*	 1.0.0  2016APR19  ND  Initial Development
*
*
*/

#include "debounce.h"

/** Assign a digital input pin to be debounced */
void Debounce::attach(uint8_t _inputPin)
{
	this->inputPin = _inputPin;
	this->state[DEBOUNCE_CURRENT_STATE ] = (bool)digitalRead(this->inputPin);
	this->state[DEBOUNCE_PREVIOUS_STATE] = this->state[DEBOUNCE_CURRENT_STATE];
	this->state[DEBOUNCED_STATE        ] = this->state[DEBOUNCE_CURRENT_STATE];
	this->state[DEBOUNCE_STATE_CHANGED ] = false;
	this->last_millis = millis(); // reset the timer
}

/** Assign a digital input pin to be debounced and set its mode */
void Debounce::attach(uint8_t _inputPin, uint8_t _mode)
{
	pinMode(_inputPin, _mode);
	this->attach(_inputPin);
}

/** Set the number of milliseconds required to debounce the input */
void Debounce::setInterval(uint16_t _debounce_millis)
{
	this->debounce_millis = _debounce_millis;
}

/** Reads the input and updates the debounce state
*   Returns true if the debounced state has changed
*   Returns false if the debounced state has NOT changed
*/
bool Debounce::update()
{
	this->state[DEBOUNCE_CURRENT_STATE] = (bool)digitalRead(this->inputPin);
	this->state[DEBOUNCE_STATE_CHANGED] = false;
	if (this->state[DEBOUNCE_CURRENT_STATE] != this->state[DEBOUNCE_PREVIOUS_STATE])
	{
		this->last_millis = millis();
	}
	if ((millis() - this->last_millis) > this->debounce_millis)
	{
		this->state[DEBOUNCE_STATE_CHANGED] =
			(this->state[DEBOUNCED_STATE] != this->state[DEBOUNCE_CURRENT_STATE]);
		this->state[DEBOUNCED_STATE] = this->state[DEBOUNCE_CURRENT_STATE];
	}
	this->state[DEBOUNCE_PREVIOUS_STATE] = this->state[DEBOUNCE_CURRENT_STATE];
	return (this->state[DEBOUNCE_STATE_CHANGED]);
}

/** Returns the debounced state of the input */
int Debounce::debounced()
{
	return (this->state[DEBOUNCED_STATE] ? HIGH : LOW);
}
/** Returns true on the rising edge state */
bool Debounce::risingEdge()
{
	return (this->state[DEBOUNCED_STATE] && this->state[DEBOUNCE_STATE_CHANGED]);
}
/** Returns true on the falling edge state */
bool Debounce::fallingEdge()
{
	return (!this->state[DEBOUNCED_STATE] && this->state[DEBOUNCE_STATE_CHANGED]);
}

