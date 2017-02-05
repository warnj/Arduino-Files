/**********************
*	SerialPacketManager.h
*
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-MAY-12
*
*	Purpose:
*	Manages data packet formation and error checking.

*	Data packets consist of:
*	--> Packet start indicator byte (STX)
*	--> Data encoded into repeated-and-complimented nibbles for error checking
*	--> Packet end indicator byte (ETX)
*	--> Packet CRC (checksum) byte
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
*	 1.0.0  2016MAY12  ND  Initial Development
*
*
*/

#ifndef _SERIALPACKETMANAGER_H_
#define _SERIALPACKETMANAGER_H_

#include <stdlib.h>
#include <string.h>


#define SERIAL_BUFFER_DEFAULT_SIZE 80
#define STX '\2'
#define ETX '\3'

/*-------------------------------------------------------------------*/
/** Serial Packet Manager Class
*	Manages data packet formation and error checking.
*	NOTE: This is an Abstract class and has pure virtual functions.
*	These pure virtual functions must be overridden by any inheriting classes.

*/
class CSerialPacketManager
{
public:
	enum errorCode {
		ERR_NONE,			//* No error
		ERR_BUF_OVERFLOW,	//* Received data was longer than buffer length.
		ERR_BAD_CRC,		//* CRC did not match data
		ERR_BAD_NIBBLE		//* Received byte did not contain a properly encoded nibble.
	};

	CSerialPacketManager(const size_t bufferSize = SERIAL_BUFFER_DEFAULT_SIZE)
		: _data(NULL)
		, _bufferSize(bufferSize)
	{}

	/** Destructor - frees memory used.
	--> pure virtual destructor for abstract class inheritance.
	--> NOTE: inheriting class must implement destructor calling this->end() method!
	--> NOTE: inheriting class must implement empty destructor for this base abstract class.
	*/
	virtual ~CSerialPacketManager() = 0;


	/******** INTERFACE METHODS ********/

	/*-------------------------------------------------------------------*/
	/** Allocate buffer memory and start communication.
	--> speed: Baud rate to be used.
	*	Returns zero on success, non-zero otherwise.
	*	IMPORTANT: Any serial port configuration must be done BEFORE calling this method!
	*/
	int begin(long speed)
	{
		if (_data == NULL)
		{
			_data = (byte *)malloc(_bufferSize);
		}
		reset();
		_lastError = ERR_NONE;
		_errorCount = 0;
		return(serialBegin(speed));
	}
		

	/*-------------------------------------------------------------------*/
	//* Terminate communication and free buffer memory.
	void end(void)
	{
		serialEnd();
		reset();
		if (_data != NULL)
		{
			free(_data);
			_data = NULL;
		}
		return;
	}


	/*-------------------------------------------------------------------*/
	//* Called after an error or timeout to start fresh for next packet.
	void reset(void)
	{
		if (_data != NULL)
		{
			memset(_data, 0, _bufferSize);
		}
		serialFlush();
		_haveSTX = false;
		_haveETX = false;
		_available = false;
		_inputPos = 0;
		_startTime = 0;
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Call periodically from main loop to process incoming data.
	*   Reads any available data and assembles a completed data packet.
	*   Returns true when a completed data packet has been received.
	*/
	bool update(void)
	{
		// Check to see if memory has been allocated yet (must call begin() first).
		if (_data == NULL) { return(false); }

		_lastError = ERR_NONE; // Start fresh each update.
		while (serialAvailable() > 0)
		{
			byte inByte = serialReadByte();

			switch (inByte)
			{
			case STX: // start of packet
				packetBegin();
				break;

			case ETX: // end of packet (now expecting CRC check)
				_haveETX = true;
				break;

			default:
				if (!_haveSTX) { break; } // wait until beginning of a packet.
				if (!isComplemented(inByte)) { break; } // checking for bad data.
				if (!decodeComplemented(inByte)) { break; } // collect the nibbles.
				if (_haveETX)
				{
					// If we have the ETX, this byte should be the CRC
					if (!crc8Valid(_currentByte)) { break; } // checking for valid CRC.
					_available = true;
					return(true); // data is now available in the buffer.
				}
				fillBuffer();
				break;
			}
		} // end of while incoming data.
		return(false); // not ready yet
	} // end of update


	/*-------------------------------------------------------------------*/
	/** Send a message out on the communication trunk.
	*   Start with STX byte.
	*   Complemented data in the middle.
	*   Terminate with an ETX byte.
	*   Follow termination with CRC (checksum) byte.
	*   --> data: Address of message to be sent out.
	*   --> length: Number of bytes (max 255) to be send out.
	*/
	virtual void sendMsg(const byte * data, const byte length)
	{
		byte c1 = 0, c2 = 0;
		String _packetString;
		_packetString.concat(STX);
		for (byte i = 0; i < length; i++)
		{
			makeComplemented(data[i], &c1, &c2);
			_packetString.concat(char(c1));
			_packetString.concat(char(c2));
		}
		_packetString.concat(ETX);
		makeComplemented(crc8(data, length), &c1, &c2);
		_packetString.concat(char(c1));
		_packetString.concat(char(c2));
		serialPrint(_packetString);
		return;
	}


	/******** PURE VIRTUAL INTERFACE METHODS ********/
protected:

	//* Begin serial port operation. Return zero on success.
	virtual int     serialBegin(long speed) = 0;
	//* Ends serial port operation.
	virtual void    serialEnd(void) = 0;
	//* Returns the number of bytes available to be read from the serial port.
	virtual int     serialAvailable(void) = 0;
	//* Returns a single byte read from the serial port.
	virtual byte    serialReadByte(void) = 0;
	//* Writes a single byte to the serial port.
	virtual void    serialWriteByte(const byte outByte) = 0;
	//* Writes a String to the serial port.
	virtual void    serialPrint(String outString) = 0;
	//* Empties the serial port buffers.
	virtual void	serialFlush(void) = 0;
	//* Returns a timestamp (in milliseconds).
	virtual long    timestamp(void) = 0;

	/******** MEMBER ACCESS METHODS ********/
public:
	//* Returns true if packet data is available.
	bool available(void) const { return(_available); }

	//* Returns the address of the current received message
	const byte * getData(void) { return(_data); }

	//* Returns the length of the current received message
	const byte getLength(void) { return(_inputPos); }

	//* Returns value of last error code.
	//* NOTE: the last error code will be reset when update() is called.
	errorCode getLastError(void) const { return(_lastError); }

	//* Returns the total count of errors detected since begin() was called.
	unsigned long getErrorCount(void) const { return(_errorCount); }

	//* Returns the timestamp when the last packet started.
	unsigned long getPacketStartTime(void) const { return(_startTime); }

	/******** INTERNAL MEMBER VARIABLES ********/
private:
	byte * _data;             	//* Buffer for incoming data.
	const size_t _bufferSize;	//* maximum size of buffered data.
	byte _inputPos;	          	//* current index of input buffer being filled.
	errorCode _lastError;     	//* most recent error code when an error is detected.
	unsigned long _errorCount;	//* number of errors detected in the current packet.
	unsigned long _startTime; 	//* timestamp of most recent STX (start of text).
	bool _haveSTX;   	      	//* an STX (start of text) signals a packet start.
	bool _haveETX;   	      	//* an ETX (end of text) signals a packet end.
	byte _currentByte;        	//* current byte being received.
	bool _firstNibble;        	//* true when currently receiving the high nibble.
	bool _available;          	//* true when there is valid data in the buffer.


	/******** INTERNAL HELPER FUNCTIONS ********/

	/*-------------------------------------------------------------------*/
	/** Called when an STX (start of text) is received.
	*   Sets up internal flags and variables for beginning of a packet.
	*/
	void packetBegin(void)
	{
		if (_data != NULL)
		{
			memset(_data, 0, _bufferSize);
		}
		_haveSTX = true;
		_haveETX = false;
		_inputPos = 0;
		_firstNibble = true;  // incoming data byes start with the high nibble.
		_startTime = timestamp();
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Send a byte out as repeated-and-complemented nibbles.
	*	(e.g. hex values 0F 1E 2D 3C 4B 5A 69 78 87 96 A5 B4 C3 D2 E1 F0)
	*   This reduces the possibility of noise being interpreted as valid data.
	*   --> outByte: The outgoing data to be sent.
	*/
	void sendComplemented(const byte outByte)
	{
		byte c1 = 0, c2 = 0;;
		makeComplemented(outByte, &c1, & c2);
		serialWriteByte(c1);
		serialWriteByte(c2);
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Make a byte output into two bytes as repeated-and-complemented nibbles.
	*	(e.g. hex values 0F 1E 2D 3C 4B 5A 69 78 87 96 A5 B4 C3 D2 E1 F0)
	*   This reduces the possibility of noise being interpreted as valid data.
	*   --> outByte: The outgoing data to be sent.
	*   --> returnByte1: The first (high) nibble complemented output.
	*   --> returnByte2: The second (low) nibble complemented output.
	*/
	void makeComplemented(
		  const byte outByte
		, byte * returnByte1
		, byte * returnByte2
		)
	{
		byte c;
		// First (high) nibble
		c = outByte >> 4;
		*returnByte1 = ((c << 4) | (c ^ 0x0F));
		// Second (low) nibble
		c = outByte & 0x0F;
		*returnByte2 = ((c << 4) | (c ^ 0x0F));
		return;

	}


	/*-------------------------------------------------------------------*/
	/** Returns true if the byte contains a repeated-and-complemented nibble.
	*	(e.g. hex values 0F 1E 2D 3C 4B 5A 69 78 87 96 A5 B4 C3 D2 E1 F0)
	*   --> inByte: Incoming byte to check.
	*/
	bool isComplemented(const byte inByte)
	{
		if ((inByte >> 4) != ((inByte & 0x0F) ^ 0x0F))
		{
			reset();
			_lastError = ERR_BAD_NIBBLE;
			_errorCount++;
			return(false);
		}
		return(true);
	}


	/*-------------------------------------------------------------------*/
	/** Decodes the incoming byte from repeated-and-complemented nibbles.
	*   Returns true when both a high nibble and a low nibble have been received.
	*   --> inByte: Incoming byte to decode.
	*/
	bool decodeComplemented(const byte inByte)
	{
		if (_firstNibble)
		{
			// high-order nibble
			_currentByte = (inByte & 0xF0);
			_firstNibble = false;
		}
		else
		{
			// low-order nibble
			_currentByte |= (inByte >> 4);
			_firstNibble = true;
		}
		return(_firstNibble);
	}


	/*-------------------------------------------------------------------*/
	/** Calculate and return an 8-bit CRC.
	--> address: Location of data to calculate CRC for.
	--> length:  Length of data block at address.
	*/
	byte crc8(const byte *address, byte length)
	{
		byte crc = 0;
		while (length--)
		{
			byte inbyte = *address++;
			for (byte i = 8; i; i--)
			{
				byte mix = (crc ^ inbyte) & 0x01;
				crc >>= 1;
				if (mix) { crc ^= 0x8C; }
				inbyte >>= 1;
			}  // end of for
		}  // end of while
		return(crc);
	}


	/*-------------------------------------------------------------------*/
	/** Check CRC against the data in the input buffer.
	*   Returns true if CRC matches the data.
	--> crcByte:  CRC value to check against the data.
	*/
	bool crc8Valid(const byte crcByte)
	{
		if (crc8(_data, _inputPos) != crcByte)
		{
			reset();
			_lastError = ERR_BAD_CRC;
			_errorCount++;
			return(false);
		}
		return(true);
	}

		
	/*-------------------------------------------------------------------*/
	/** Push the currently received byte onto the end of the data buffer.
	*/
	void fillBuffer(void)
	{
		if (_inputPos < _bufferSize)
		{
			_data[_inputPos++] = _currentByte;
		}
		else
		{
			reset();
			_lastError = ERR_BUF_OVERFLOW;
			_errorCount++;
		}
		return;
	}

};


#endif // _SERIALPACKETMANAGER_H_
