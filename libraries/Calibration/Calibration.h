/*
*	Calibration.h
*	
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2015
*
*	Written by: Nathan Durnan
*	Date: 2015-Oct-02
*
*/

#ifndef _CALIBRATION_HEADER_
	#define _CALIBRATION_HEADER_


	#include <Arduino.h>
	#include <Debounce.h>


	#define CALIBRATION_DEBOUNCE_TIME 50 //* milliseconds
	#define CALIBRATION_DELAY       3000 //* milliseconds
	#define CALIBRATION_ERR_DELAY   5000 //* milliseconds
	#define CALIBRATION_DELAY_GAP   1000 //* milliseconds
	#define CALIBRATION_TOLERANCE      8 //* ADC Counts


	/** Data structure for Calibration Point information */
	struct CalibrationPoint
	{
		long raw; //* ADC Counts
		long eng; //* Engineering Units
	};

	/** Data structure containing Hi and Lo calibration points */
	struct CalibrationData
	{
		CalibrationPoint lo; //* Low Calibration Point
		CalibrationPoint hi; //* High Calibration Point
	};


	/** Class for managing calibration data and actions */
	class Calibration
	{
	public:
		/** Parameterized Constructor:
		*	_calTolerance --> Tolerance window value for stability checking.
		*	_calDelay -->  Delay time (milliseconds) for stability checking.
		*	_calErrDelay -->  Delay time (milliseconds) for error checking.
		*/
		Calibration(
			  long _calTolerance         = CALIBRATION_TOLERANCE
			, unsigned long _calDelay    = CALIBRATION_DELAY
			, unsigned long _calErrDelay = CALIBRATION_ERR_DELAY
			);

		/** Assign IO Pins to calibration functions:
		*	_inCalLo --> Digital Input to trigger low calibration capture.
		*	_inCalHi --> Digital Input to trigger high calibration capture.
		*	_mode --> pinMode for the input pins (INPUT or INPUT_PULLUP).
		*	_outPin --> Digital Output for status indication.
		*/
		void setPins(
			  uint8_t _inCalLo
			, uint8_t _inCalHi
			, uint8_t _mode
			, uint8_t _outPin
			);

		/** Set up calibration data values */
		void setData(
			  long _RawLo = 0
			, long _RawHi = 1023
			, long _EngLo = 0
			, long _EngHi = 10000
			);

		/** Set tolerance window value for stability checking */
		void setTolerance(long _calTolerance);

		/** Set delay time (milliseconds) for stability checking.
		*	Minimum value = CALIBRATION_DEBOUNCE_TIME
		*	Returns 0 on success, non-zero on error or overwrite condition.
		*   NOTE: Error Delay will be extended if it is shorter than Stability Delay + CALIBRATION_DELAY_GAP
		*/
		int setDelay(unsigned long _calDelay);

		/** Set delay time (milliseconds) for error checking.
		*	Returns 0 on success, non-zero on error or overwrite condition.
		*   NOTE: Error Delay will be extended if it is shorter than Stability Delay + CALIBRATION_DELAY_GAP
		*/
		int setErrDelay(unsigned long _calErrDelay);

		/** Check the calibration input button(s)
		*   Update the calibration value(s) based on input raw value
		*   Returns 'true' if a calibration has been updated.
		*/
		bool update(const long _inputRawVal);

		/** Return a scaled/calibrated value */
		long getScaled(const long _inputRawValue);

		/** Return a pointer to the calibration data value structure */
		CalibrationData * getData();

	private:
		/** Data structure supporting external calibration interface */
		struct CalibrationControl
		{
			Debounce Button;
			long CaptureMin = 1023;
			long CaptureMax = 0;
			unsigned long CaptureTimer = 0;
			unsigned long ErrorTimer = 0;
			bool OneShotTrigger = false;
		};

		/** Main Calibration Data Structure */
		struct CalibrationStructure
		{
			CalibrationData     data; //* Calibration Points
			CalibrationControl ctrLo; //* Control for Low Calibration Point
			CalibrationControl ctrHi; //* Control for High Calibration Point
		};

		CalibrationStructure cal;  //* Internal Calibration Data Structure
		uint8_t outStatusPin;      //* Calibration Status LED output pin
		long calTolerance;         //* Calibration raw value tolerance window for stability check
		unsigned long calDelay;    //* Milliseconds of stable signal required before accepting a new calibration value.
		unsigned long calErrDelay; //* Milliseconds to wait for stable signal before indicating an error. (MUST be > calDelay)
	
		enum CalibrationResult
		{
			CAL_RESULT_NONE,
			CAL_RESULT_IN_PROCESS,
			CAL_RESULT_SUCCESS,
			CAL_RESULT_ERROR,
			CAL_RESULT_DONE
		};

		/** Update the Calibration Point data
		*   Check button state and input stability
		*   Update calibration raw value on successful conditions
		*/
		CalibrationResult updateCalPoint(
			  CalibrationControl * _ctr
			, CalibrationPoint   * _pt
			, const long _inputRawVal);

		/** Reset capture timer and min/max values */
		void updateCalPointResetCapture(CalibrationControl * _ctr);

		/** Check the limits of the Stability and Error delay times.
		*	Error Delay must be longer than Stability Delay.
		*   Error Delay will be extended if it is shorter than Stability Delay + CALIBRATION_DELAY_GAP
		*/
		int setDelayCheckLimits(void);

	};



#endif // _CALIBRATION_HEADER_
