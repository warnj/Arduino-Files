/**********************
*	ExtendedParameter.h
*
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-JUN-09
*
*	Purpose:
*	Extended Parameter Class for command arguments.
*
*
*/

/*********************
*	Revision History
*   Version|Date      |Who|Notes
*   -------|----------|---|------------------
*	 1.0.0  2016JUN09  ND  Initial Development
*
*
*/

#ifndef _EXTENDEDPARAMETER_HEADER_
#define _EXTENDEDPARAMETER_HEADER_

#include <stdlib.h> // required for strtol/strtod functions
#include <ctype.h>  // required for isdigit function
#include "..\ArduinoStringEmulation\ArduinoStringEmulation.h" // included for non-Arduino Projects.


/*-------------------------------------------------------------------*/
/** Extended Parameter Class for command arguments.
*/
class CExtendedParameter
{
public:
	enum parameterType
	{
		PT_EMPTY = 0U,
		PT_INTEGER,
		PT_FLOAT,
		PT_CHARACTER
	};

	// Empty Constructor
	CExtendedParameter(){ clearValue(); }
	//* Parameterized Constructor - String datatype
	CExtendedParameter(const String valueString) { setValue(valueString); }
	//* Parameterized Constructor - char* datatype
	CExtendedParameter(const char * valueString) { setValue(valueString); }
	//* Parameterized Constructor - int datatype
	CExtendedParameter(const int          value) { setValue(value); }
	//* Parameterized Constructor - float datatype
	CExtendedParameter(const float        value) { setValue(value); }

	//* Destructor - frees memory used.
	~CExtendedParameter(){ ; }


	/******** INTERFACE METHODS ********/

	/*-------------------------------------------------------------------*/
	/** Resets the parameter to an empty value.
	*/
	void clearValue(void)
	{
		this->_value = "";
		this->_type = PT_EMPTY;
	}


	/*-------------------------------------------------------------------*/
	/** Sets the parameter to the given value.
	*   This also determines what type of value is being assigned.
	*   --> valueString: String containing new parameter value.
	*/
	void setValue(const String valueString)
	{
		_value = valueString;
		if (this->_value.c_str()[0] == 0)
		{
			this->_type = PT_EMPTY;
		}
		else if (isInteger(this->_value.c_str()))
		{
			this->_type = PT_INTEGER;
		}
		else if (isFloat(this->_value.c_str()))
		{
			this->_type = PT_FLOAT;
		}
		else
		{
			this->_type = PT_CHARACTER;
		}
	}
	/** Sets the parameter to the given value.
	*   This also determines what type of value is being assigned.
	*   --> valueString: character array string containing new parameter value.
	*/
	void setValue(const char* valueString) { setValue(String(valueString)); }
	//* Sets the parameter to the given int value.
	void setValue(const int value)
	{
		_value = String(value);
		_type = PT_INTEGER;
	}
	//* Sets the parameter to the given float value.
	void setValue(const float value)
	{
		_value = String(value);
		_type = PT_FLOAT;
	}

	/******** MEMBER ACCESS METHODS ********/
public:
	//* Returns the datatype of the value stored in the parameter.
	const enum parameterType getType(void) { return(_type); }
	//* Returns a String representation of the parameter.
	const String getString(void) { return(_value); }
	//* Returns a character array string representation of the parameter.
	const char *       getData(void)       { return(_value.c_str()); }
	//* Returns the length of the character array string representation of the parameter.
	const unsigned int getDataLength(void) { return(_value.length()); }
	//* Returns an integer representation of the parameter.
	//-->  (will return 0 if not an integer parameter).
	const int    toInt(void)     { return(_value.toInt()); }
	//* Returns a floating-point representation of the parameter.
	//-->  (will return 0 if not an floating-point parameter).
	const float  toFloat(void)   { return(_value.toFloat()); }



private:
	/******** INTERNAL MEMBER VARIABLES ********/

	enum parameterType _type; //* Datatype represented in the parameter.
	String _value; //* Internal storage string.

	/******** INTERNAL HELPER FUNCTIONS ********/
private:
	/*-------------------------------------------------------------------*/
	/** Returns true if the string pointer represents an integer number.
	*/
	bool isInteger(const char *s)
	{
		if ((s[0] == 0) ||
			((!isdigit(s[0])) &&
			(s[0] != '-') &&
			(s[0] != '+')))
		{
			return false;
		}
		char * p;
		strtol(s, &p, 10);
		return (*p == 0);
	}


	/*-------------------------------------------------------------------*/
	/** Returns true if the string pointer represents a floating-point number.
	*/
	bool isFloat(const char *s)
	{
		if ((s[0] == 0) ||
			((!isdigit(s[0])) &&
			(s[0] != '.') &&
			(s[0] != '-') &&
			(s[0] != '+')))
		{
			return false;
		}
		char * p;
		strtod(s, &p);
		return (*p == 0);
	}


};

#endif // _EXTENDEDPARAMETER_HEADER_
