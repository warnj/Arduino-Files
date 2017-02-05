/**********************
*	Debounce.h
*
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-APR-19
*
*/

#ifndef _DEBOUNCE_HEADER_
	#define _DEBOUNCE_HEADER_

	#include <Arduino.h>
	#include <BitBool.h> // https://arduino.land/Code/BitBool/ 


	/*********** State Flag Definitions ***********/
	#define DEBOUNCE_CURRENT_STATE   0 // 
	#define DEBOUNCE_PREVIOUS_STATE  1 //
	#define DEBOUNCED_STATE          2 //
	#define DEBOUNCE_STATE_CHANGED   3 //

	/** Monitor a digital input pin and debounce its state */
	class Debounce
	{
	public:

		/** Assign a digital input pin to be debounced */
		void attach(uint8_t _inputPin);

		/** Assign a digital input pin to be debounced and set its mode */
		void attach(uint8_t _inputPin, uint8_t _mode);

		/** Set the number of milliseconds required to debounce the input */
		void setInterval(uint16_t _debounce_millis);

		/** Reads the input and updates the debounce state
		*   Returns true if the debounced state has changed
		*   Returns false if the debounced state has NOT changed
		*/
		bool update();

		/** Returns the debounced state of the input */
		int debounced();
		/** Returns true on the rising edge state */
		bool risingEdge();
		/** Returns true on the falling edge state */
		bool fallingEdge();

	private:
		uint8_t       inputPin;        //* Digital input pin number to be debounced
		BitBool<4>    state;           //* State flags
		uint16_t      debounce_millis; //* Number of milliseconds that the input must be stable before updating the debounced state.
		unsigned long last_millis;	   //* Timestamp of previous update

	};



#endif // _DEBOUNCE_HEADER_