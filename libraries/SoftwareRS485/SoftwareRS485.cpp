/**********************
*	SoftwareRS485.cpp
*
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-APR-19
*
*	Purpose: 
*	RS485 communication library using SoftwareSerial and CSerialPacketManager.
*	This includes data packet error checking as well as managing
*	the half-duplex directional control of the RS485 hardware.
*
*
*/

/*********************
*	Revision History
*   Version|Date      |Who|Notes
*   -------|----------|---|------------------
*	 1.0.0  2016APR19  ND  Initial Development 
*	 1.1.0  2016MAY12  ND  Created CSerialPacketManager class and implemented as root of this class.
*
*/


#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SoftwareRS485.h"
#include "SerialPacketManager.h"


//* Pure Virtual Destructor of base class must have a body!
CSerialPacketManager::~CSerialPacketManager() {}

//* Begin serial port operation. Return zero on success.
int SoftwareRS485::serialBegin(long speed) {
	_ss.begin(speed);
	return(0);
}

//* Ends serial port operation.
void SoftwareRS485::serialEnd(void) {
	_ss.end();
}

//* Returns the number of bytes available to be read from the serial port.
int SoftwareRS485::serialAvailable(void){
	return(_ss.available());
}

//* Returns a single byte read from the serial port.
byte SoftwareRS485::serialReadByte(void) {
	digitalWrite(_rtsPin, RTS_STATE_READ);
	return(_ss.read());
}

//* Writes a single byte to the serial port.
void SoftwareRS485::serialWriteByte(const byte outByte) {
	_ss.write(outByte);
}

//* Writes a String to the serial port.
void SoftwareRS485::serialPrint(const String outString) {
	_ss.print(outString);
}

//* Empties the serial port buffers.
void SoftwareRS485::serialFlush(void) {
	_ss.flush();
}

//* Returns a timestamp (in milliseconds).
long SoftwareRS485::timestamp(void){
	return(millis());
}


/** Wraps the packet manager method with hardware flow control.
*/
void SoftwareRS485::sendMsg(const byte * data, const byte length) {
	digitalWrite(_rtsPin, RTS_STATE_WRITE);
	CSerialPacketManager::sendMsg(data, length);
	digitalWrite(_rtsPin, RTS_STATE_READ);
	return;
}

