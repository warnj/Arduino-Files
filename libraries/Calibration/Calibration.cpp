/*
*	Calibration.cpp
*	
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-APR-20
*
*	Purpose: Calibration management for an analog input channel.
*
*/

/*********************
*	Revision History
*   Version|Date      |Who|Notes
*   -------|----------|---|------------------
*	 1.0.0  2016APR20  ND  Initial Development
*
*
*/


#include "Calibration.h"

/*-----------------------------------------------------------------*/
// PUBLIC METHODS

/** Parameterized Constructor:
*	_calTolerance --> Tolerance window value for stability checking.
*	_calDelay -->  Delay time (milliseconds) for stability checking.
*	_calErrDelay -->  Delay time (milliseconds) for error checking.
*/
Calibration::Calibration(
	  long _calTolerance
	, unsigned long _calDelay
	, unsigned long _calErrDelay
	)
{
	this->setTolerance(_calTolerance);
	this->setDelay(_calDelay);
	this->setErrDelay(_calErrDelay);
}

/** Assign IO Pins to calibration functions:
*	_inCalLo --> Digital Input to trigger low calibration capture.
*	_inCalHi --> Digital Input to trigger high calibration capture.
*	_mode --> pinMode for the input pins (INPUT or INPUT_PULLUP).
*	_outPin --> Digital Output for status indication.
*/
void Calibration::setPins(
	  uint8_t _inCalLo
	, uint8_t _inCalHi
	, uint8_t _mode
	, uint8_t _outPin
	)
{
	this->cal.ctrLo.Button.attach(_inCalLo, _mode);
	this->cal.ctrLo.Button.setInterval(CALIBRATION_DEBOUNCE_TIME);
	this->cal.ctrHi.Button.attach(_inCalHi, _mode);
	this->cal.ctrHi.Button.setInterval(CALIBRATION_DEBOUNCE_TIME);
	this->outStatusPin = _outPin;
	pinMode(this->outStatusPin, OUTPUT);
	return;
}

/** Set up calibration data values */
void Calibration::setData(
	  long _RawLo
	, long _RawHi
	, long _EngLo
	, long _EngHi
	)
{
	this->cal.data.lo.raw = _RawLo;
	this->cal.data.hi.raw = _RawHi;
	this->cal.data.lo.eng = _EngLo;
	this->cal.data.hi.eng = _EngHi;
	return;
}

/** Set tolerance window value for stability checking */
void Calibration::setTolerance(long _calTolerance)
{
	this->calTolerance = abs(_calTolerance);
}

/** Set delay time (milliseconds) for stability checking.
*	Minimum value = CALIBRATION_DEBOUNCE_TIME
*	Returns 0 on success, non-zero on error or overwrite condition.
*   NOTE: Error Delay will be extended if it is shorter than Stability Delay + CALIBRATION_DELAY_GAP
*/
int Calibration::setDelay(unsigned long _calDelay)
{
	_calDelay = max(_calDelay, CALIBRATION_DEBOUNCE_TIME);
	this->calDelay = _calDelay;
	return(this->setDelayCheckLimits());
}

/** Set delay time (milliseconds) for error checking.
*	Returns 0 on success, non-zero on error or overwrite condition.
*   NOTE: Error Delay will be extended if it is shorter than Stability Delay + CALIBRATION_DELAY_GAP
*/
int Calibration::setErrDelay(unsigned long _calErrDelay)
{
	this->calErrDelay = _calErrDelay;
	return(this->setDelayCheckLimits());
}

/** Check the limits of the Stability and Error delay times.
*	Error Delay must be longer than Stability Delay.
*   Error Delay will be extended if it is shorter than Stability Delay + CALIBRATION_DELAY_GAP
*/
int Calibration::setDelayCheckLimits(void)
{
	if (this->calErrDelay <= (this->calDelay + CALIBRATION_DELAY_GAP))
	{
		this->calErrDelay = (this->calDelay + CALIBRATION_DELAY_GAP);
		return(-1);
	}
	return(0);
}


/** Check the calibration input button(s)
*   Update the calibration value(s) based on input raw value
*   Returns 'true' if a calibration has been updated.
*/
bool Calibration::update(const long _inputRawVal)
{
	CalibrationResult loResult = 
		updateCalPoint(&this->cal.ctrLo, &this->cal.data.lo, _inputRawVal);
	CalibrationResult hiResult = 
		updateCalPoint(&this->cal.ctrHi, &this->cal.data.hi, _inputRawVal);
	return(
		(loResult == CAL_RESULT_SUCCESS) ||
		(hiResult == CAL_RESULT_SUCCESS)
		);
}

/** Return a scaled/calibrated value */
long Calibration::getScaled(const long _inputRawValue)
{
	return(map(_inputRawValue
		, this->cal.data.lo.raw
		, this->cal.data.hi.raw
		, this->cal.data.lo.eng
		, this->cal.data.hi.eng
		));
}

/** Return a pointer to the calibration data value structure */
CalibrationData * Calibration::getData() { return(&this->cal.data); }


/*-----------------------------------------------------------------*/
// PRIVATE METHODS

/** Update the Calibration Point data
*   Check button state and input stability
*   Update calibration raw value on successful conditions
*/
Calibration::CalibrationResult Calibration::updateCalPoint(
	  CalibrationControl * _ctr
	, CalibrationPoint   * _pt
	, const long _inputRawVal)
{
	_ctr->Button.update();
	// Button state will be LOW when debounce triggered.
	if (_ctr->Button.debounced() == HIGH)
	{
		// not debounced yet...
		updateCalPointResetCapture(_ctr);
		_ctr->ErrorTimer = 0;
		_ctr->OneShotTrigger = false;
		return(CAL_RESULT_NONE);
	}
	// Button is debounced LOW - update capture data...
	if (_ctr->OneShotTrigger == true) { return(CAL_RESULT_DONE); }
	if (_ctr->CaptureTimer   ==    0) { _ctr->CaptureTimer = millis(); }
	if (_ctr->ErrorTimer     ==    0) { _ctr->ErrorTimer   = millis(); }
	_ctr->CaptureMin = min(_inputRawVal, _ctr->CaptureMin);
	_ctr->CaptureMax = max(_inputRawVal, _ctr->CaptureMax);
	// Make sure the input value is stabilized.
	if (abs(_ctr->CaptureMax - _ctr->CaptureMin) > this->calTolerance)
	{
		updateCalPointResetCapture(_ctr);
	}
	// Has it taken too long to stabilize?
	if ((_ctr->ErrorTimer != 0) &&
		((millis() - _ctr->ErrorTimer) > this->calErrDelay))
	{
		for (int i = 0; i < 10; i++)
		{
			digitalWrite(this->outStatusPin, HIGH); delay(125);
			digitalWrite(this->outStatusPin, LOW ); delay(125);
		}
		_ctr->OneShotTrigger = true;
		return(CAL_RESULT_ERROR);
	}
	// Have the conditions been stable for the preset time?
	if ((_ctr->CaptureTimer != 0) &&
		((millis() - _ctr->CaptureTimer) > this->calDelay))
	{
		digitalWrite(this->outStatusPin, HIGH); delay(750);
		digitalWrite(this->outStatusPin, LOW );
		_pt->raw = ((_ctr->CaptureMin + _ctr->CaptureMax) / 2L);
		_ctr->OneShotTrigger = true;
		return(CAL_RESULT_SUCCESS);
	}
	return(CAL_RESULT_IN_PROCESS);
}

/** Reset capture timer and min/max values */
void Calibration::updateCalPointResetCapture(CalibrationControl * _ctr)
{
	_ctr->CaptureTimer   = 0;
	_ctr->CaptureMin     = 1023;
	_ctr->CaptureMax     = 0;
	return;
}
