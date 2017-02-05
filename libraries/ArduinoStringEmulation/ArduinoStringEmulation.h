/**********************
*	ArduinoStringEmulation.h
*
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-JUN-03
*
*	Purpose:
*	Emulation of the Arduino String class using c++ std::string implementation.
*	This emulated class allows libraries to be shared 
*	between Arduino projects and regular c++ projects.
*
*	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*	NOTE: This class is intended to be reused in non-Arduino c++ applications.
*		Take care when modifying it to ensure only generic methods and types
*		are used and avoid using any Arduino-specific functions or definitions.
*	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*
*/

/*********************
*	Revision History
*   Version|Date      |Who|Notes
*   -------|----------|---|------------------
*	 1.0.0  2016JUN03  ND  Initial Development
*
*
*/

#ifndef String_class_h	

// NOTE: the definition name is from the Arduino wstring.h header where the String class is defined.
// This MUST match so this redefinition will be excluded when included in Arduino projects!
#define String_class_h

#include <string>

using namespace std;

class String
{
public:
	String() { _string.empty(); }
	String(const char * cstr) { _string = cstr; }
	String(const int   value) { _string = to_string(value); }
	String(const float value) { _string = to_string(value); }

	const char * c_str() { return(_string.c_str()); }

	String & operator = (const char *cstr)
	{
		_string = cstr;
		return(*this);
	}

	unsigned char operator == (const char *cstr) { return(_string == cstr); }

	unsigned int length(void) const { return(_string.length()); }

	int toInt(void) const { return(atoi(_string.c_str())); }
	float toFloat(void) const { return((float)atof(_string.c_str())); }

	void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index = 0) const
	{
		if ((index + (bufsize - 1)) > _string.length()) { return; }
		for (unsigned int i = 0; i < (bufsize - 1); i++)
		{
			buf[i] = _string.c_str()[index + i];
		}
		buf[(bufsize - 1)] = 0;
	}

	int indexOf(char ch, unsigned int fromIndex = 0) const
	{
		if (fromIndex > _string.length()) { return(-1); }
		int i = _string.find(ch, fromIndex);
		if (i == std::string::npos) { return(-1); }
		return(i);
	}

	String substring(unsigned int beginIndex) const
	{
		return String(_string.substr(beginIndex).c_str());
	};
	String substring(unsigned int beginIndex, unsigned int endIndex) const
	{
		return String(_string.substr(
			beginIndex
			, (endIndex - beginIndex)
			).c_str()
			);
	}
	

	unsigned char concat(const string &str)
	{
		int _startLength = _string.length();
		_string.append(str);
		return((_string.length() == _startLength) ? 0 : 1);

	}
	unsigned char concat(const String &str)
	{
		String _temp = str;
		return(concat(_temp.c_str()));
	}
	unsigned char concat(const char * cstr) { return(concat(string(cstr))); }
	unsigned char concat(      char      c) { return(concat(string(&c))); }
	unsigned char concat(         int  num) { return(concat(to_string(num))); }
	unsigned char concat(unsigned int  num) { return(concat(to_string(num))); }
	unsigned char concat(         long num) { return(concat(to_string(num))); }
	unsigned char concat(unsigned long num) { return(concat(to_string(num))); }
	unsigned char concat(        float num) { return(concat(to_string(num))); }
	unsigned char concat(       double num) { return(concat(to_string(num))); }
	

private:
	string _string; // internal std::string memory.
};

#endif