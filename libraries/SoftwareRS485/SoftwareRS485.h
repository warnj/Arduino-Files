/**********************
*	SoftwareRS485.h
*
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-APR-19
*
*/

#ifndef _SOFTWARERS485_HEADER_
	#define _SOFTWARERS485_HEADER_

	#include "SerialPacketManager.h"

	#define SRS485_BUFFER_SIZE 80
	#define RTS_STATE_READ  LOW
	#define RTS_STATE_WRITE HIGH


	class SoftwareRS485 : public CSerialPacketManager
	{
	public:

		//* Parameterized Constructor - initializes class
		SoftwareRS485(
			  uint8_t   rxPin      = MISO
			, uint8_t   txPin      = MOSI
			, uint8_t   rtsPin     = SCK
			, const int bufferSize = SRS485_BUFFER_SIZE
			)
			: CSerialPacketManager(bufferSize)
			, _rtsPin(rtsPin)
			, _ss(SoftwareSerial(rxPin, txPin))
		{
			pinMode(_rtsPin, OUTPUT);
			digitalWrite(_rtsPin, RTS_STATE_READ);
		}

		//* destructor - frees memory used.
		~SoftwareRS485() { end(); }

		/******** INTERFACE METHODS ********/
		
		/** Wraps the packet manager method with hardware flow control.
		*/
		void sendMsg(const byte * data, const byte length);

	protected:
		/******** VIRTUAL INTERFACE METHOD OVERRIDES ********/

		//* Begin serial port operation. Return zero on success.
		int serialBegin(long speed);
		//* Ends serial port operation.
		void serialEnd(void);
		//* Returns the number of bytes available to be read from the serial port.
		int  serialAvailable(void);
		//* Returns a single byte read from the serial port.
		byte serialReadByte(void);
		//* Writes a single byte to the serial port.
		void serialWriteByte(const byte outByte);
		//* Writes a String to the serial port.
		void serialPrint(const String outString);
		//* Empties the serial port buffers.
		void serialFlush(void);
		//* Returns a timestamp (in milliseconds).
		long timestamp(void);

		/******** MEMBER ACCESS METHODS ********/

	private:
		/******** INTERNAL MEMBER OBJECTS ********/
		
		uint8_t _rtsPin;	//* IO Pin to use for RTS (transmit-enable) signal.
		SoftwareSerial _ss; //* Internal SoftwareSerial instance.

	};

#endif // _SOFTWARERS485_HEADER_