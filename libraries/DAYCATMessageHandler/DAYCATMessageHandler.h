/**********************
*	DAYCATMessageHandler.h
*
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-JUN-09
*
*	Purpose:
*	Message building and parsing for interacting with DAYCAT communications.
*	This header file contains declarations for base class virtual method overrides.
*	Any implementations not defined in this file must be
*	  defined by the end user within their application.
*
*/

/*********************
*	Revision History
*   Version|Date      |Who|Notes
*   -------|----------|---|------------------
*	 1.0.0  2016JUN09  ND  Initial Development
*
*/

#ifndef _DAYCATMESSAGEHANDLER_HEADER_
#define _DAYCATMESSAGEHANDLER_HEADER_

#include <SoftwareRS485.h>
#include "ExtendedParameter.h"
#include "MessageHandler.h"

/** Message Handler implementation class for DAYCAT Arduino interface.
--> Class must be initialized with a pointer to a preexisting communication class instance.
--> IMPORTANT: use setID() method to configure the communication address identifier for this instance.
*/
class DAYCATMessageHandler : public CMessageHandler
{
public:

	/** Parameterized Constructor.
	--> This class must be attached to a preexisting communications class instance.
	*/
	DAYCATMessageHandler(
		SoftwareRS485 * comms	//* Pointer to preexisting communications class instance.
		) : _comms(comms)	{ ; }

	/******** INTERFACE METHODS ********/
		

	/******** VIRTUAL INTERFACE METHOD OVERRIDES ********/
protected:
	//* Send a message string using the communications class reference.
	void doSendMessage(const String message)
	{
		_comms->sendMsg((const byte*)message.c_str(), message.length());
	}
	
	//* Action triggered when an "OK" (Success) message has been identified.
	void onMessageOK(void);
	//* Action triggered when a "NO" (Failure) message has been identified.
	void onMessageNO(void);
	/** Action triggered when a "PG" (Ping Request) message has been identified.
	*	NOTE: sending a message in response to a ping is handled automatically.
	*	This action handler is provided to allow additional actions to be performed.
	*/
	void onMessagePingRequest(void);
	//* Action triggered when a "PO" (Ping Response) message has been identified.
	void onMessagePingResponse(void);
	//* Action triggered when a "SENDHOSTTEXT" Extended Command has been identified.
	mh_action_ret_type onCommandSendHostText(const String text, CExtendedParameter * returnValue);
	//* Action triggered when a "RESETIO" Extended Command has been identified.
	mh_action_ret_type onCommandResetIO(void);

	/******** MEMBER ACCESS METHODS ********/
public:


	/******** INTERNAL MEMBER OBJECTS ********/
private:
	//* Pointer to communications class to handle data send/receive.
	SoftwareRS485 * _comms;

};

#endif // _DAYCATMESSAGEHANDLER_HEADER_