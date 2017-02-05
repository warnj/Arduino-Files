/**********************
*	MessageHandler.h
*
*	Triumph Actuation Systems - Yakima, LLC
*	Copyright (C) 2016
*
*	Written by: Nathan Durnan
*	Date: 2016-JUN-09
*
*	Purpose:
*
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
*	 1.0.0  2016JUN09  ND  Initial Development
*
*
*/


/*-------------------------------------------------------------------*/
/* Command format WSXXXCCNNNN
				  01234567890
	WS            The workstation or device number prefix - may be WS or DV
	  XXX         The target workstation or device number for the message (Example: 005 or 229)
	              NOTE: ID# 999 is reserved for the server/host workstation or device.
	     CC       The command. (Example: PG, OP, CL, EX, etc.)
		   NNNN   The workstation or device number sending this command.

		       ...The remaining text consists of extended commands or return data.
*/

/* Extended Command Message Format:
	Example Extended Command: WS104EX00920009SOLE(1,1)
	“WS104” – Workstation the command is being send to
	“EX” – This is an extended command
	“0092” – The workstation sending the command
	A standard command would end here at 11 char, extended always has at least 5 more char
	“0009” – How many bytes/characters to read after these first four char of the extended portion of the command
	“SOLE(1,1)” – The command, turn solenoid one on
*/
/*	Return Value Message Format
	Example Return Value: WS092RT0104000215
	"WS092" - Workstation the return value is being sent to
	"RT" - This is a return value message
	"0104" - The workstation sending the return value
	"0002" - How many bytes/characters to read after these first four char of the return portion of the command
	"15" - The return value
*/

#ifndef _MESSAGEHANDLER_HEADER_
#define _MESSAGEHANDLER_HEADER_

#include <string.h> // required for memset/memcpy functions
#include "ExtendedParameter.h"

#define MAX_EXTENDED_PARAMETERS 7

#define MIN_MESSAGE_HEADER_LENGTH 11

#define MSG_HEADER_WORKSTATION "WS"
#define MSG_HEADER_DEVICE      "DV"

#define MH_OK                   0
#define MH_MSG_LENGTH_ERROR    -1
#define MH_MSG_HEADER_ERROR    -2
#define MH_MSG_TARGET_ID_ERROR -3
#define MH_MSG_COMMAND_ERROR   -4
#define MH_MSG_SENDER_ID_ERROR -5
#define MH_MSG_PARAMETER_ERROR -6

//* Message Handler Action Return Value Type.
enum mh_action_ret_type {
	MH_ACTION_RETURN_ERR = -1,	//* Action handler return for error or failure.
	MH_ACTION_RETURN_OK = 0,	//* Action handler return for success.
	MH_ACTION_RETURN_OKVAL = 1	//* Action handler return for success including a return value.
};


/*-------------------------------------------------------------------*/
//* Message Command Identifier Enumeration
//* NOTE: MUST be kept aligned with MessageCommandTextType array!
enum MessageCommandType
{
	MCT_NONE = 0,			//* __
	MCT_OK,					//* OK
	MCT_NO,					//* NO
	MCT_PING_REQUEST,		//* PG
	MCT_PING_RESPONSE,		//* PO
	MCT_CONNECTION_OPEN,	//* OP
	MCT_CONNECTION_CLOSE,	//* CL
	MCT_EXTENDED,			//* EX
	MCT_RETURN,				//* RT
	MCT_COUNT	// Number of message command types defined - add new items ABOVE this line!
};

/*-------------------------------------------------------------------*/
//* Message Command Text Array - 2-Character message command identification codes.
//* NOTE: MUST be kept aligned with MessageCommandType enumeration!
const char MessageCommandTextType[MCT_COUNT][3] = {
	"__",	// MCT_NONE
	"OK",	// MCT_OK
	"NO",	// MCT_NO
	"PG",	// MCT_PING_REQUEST
	"PO",	// MCT_PING_RESPONSE
	"OP",	// MCT_CONNECTION_OPEN
	"CL",	// MCT_CONNECTION_CLOSE
	"EX",	// MCT_EXTENDED
	"RT"	// MCT_RETURN
};


/*-------------------------------------------------------------------*/
//* Extended Command Identifier Enumeration
//* NOTE: MUST be kept aligned with ExtendedCommandTextType array!
enum ExtendedCommandType
{
	ECT_NONE = 0,
	ECT_SET_SOLENOID,
	ECT_READ_ANALOG,
	ECT_WRITE_ANALOG,
	ECT_SEND_HOST_TEXT,
	ECT_RESET_IO,
	ECT_COUNT // Number of extended command types defined - add new items ABOVE this line!
};

/*-------------------------------------------------------------------*/
//* Extended Command Text Array - identification text for extended commands.
//* NOTE: MUST be kept aligned with ExtendedCommandType enumeration!
const char ExtendedCommandTextType[ECT_COUNT][15] = {
	"",					// ECT_NONE
	"SOLE",				// ECT_SET_SOLENOID
	"ADC",				// ECT_READ_ANALOG
	"DAC",				// ECT_WRITE_ANALOG
	"SENDHOSTTEXT",		// ECT_SEND_HOST_TEXT
	"RESETIO"			// ECT_RESET_IO
};


/*-------------------------------------------------------------------*/
//* Extended Command Data Structure for keeping track of command and parameters.
struct ExtendedCommand
{
	//* Empty Constructor.
	ExtendedCommand() { reset(); }

	//* The command identifier.
	ExtendedCommandType cmdID;
	
	//* The parameters for the command.
	CExtendedParameter parameter[MAX_EXTENDED_PARAMETERS];
	
	//* The number of valid parameters.
	unsigned int parameterCount;

	//* The return value for the command.
	CExtendedParameter returnValue;
	
	/*-------------------------------------------------------------------*/
	/** Reset internal command data memory before evaluating a new command.
	*/
	void reset(void)
	{
		cmdID = ECT_NONE;
		for (byte i = 0; i < MAX_EXTENDED_PARAMETERS; i++)
		{
			parameter[i].clearValue();
		}
		parameterCount = 0;
		returnValue.clearValue();
		return;
	}

	/*-------------------------------------------------------------------*/
	/** Returns the extended command as a formatted String.
	*/
	const String toString(void)
	{
		String returnString = "";
		returnString.concat(ExtendedCommandTextType[cmdID]);
		returnString.concat('(');
		for (byte i = 0; i < parameterCount; i++)
		{
			returnString.concat(parameter[i].getString());
			if (i != (parameterCount - 1))
			{
				returnString.concat(',');
			}
		}
		returnString.concat(')');
		return(returnString);
	}

};


/*-------------------------------------------------------------------*/
//* Message Data Structure for keeping track of message information.
struct MessageData
{
	// Empty Constructor.
	MessageData() { reset(); }

	//* The first two characters from the message identifying the communication type.
	char header[3];

	//* The message command type identifier.
	MessageCommandType cmdID;

	//* A number identifying the intended recipient of the message.
	unsigned int targetID;

	//* A number identifying the sender of the message.
	unsigned int senderID;

	//* Extended Command Data
	ExtendedCommand extended;

	/*-------------------------------------------------------------------*/
	/** Reset internal message data memory before evaluating a new message.
	*/
	void reset(void)
	{
		memset(header , 0, 3);
		cmdID = MCT_NONE;
		targetID = 0;
		senderID = 0;
		extended.reset();
		return;
	}
};


/*-------------------------------------------------------------------*/
/** Prototype class for DAYCAT Message interactions.
*	This class must be implemented as an inherited class definition.
*/
class CMessageHandler
{
public:
	//* Empty Constructor.
	CMessageHandler() { ; }
		
	//* Destructor - frees memory used.
	~CMessageHandler() { ; }
		

	/******** INTERFACE METHODS ********/
public:
	/*-------------------------------------------------------------------*/
	/** Evaluate the message string and trigger the command action(s).
	*	Returns zero on success.  Non-zero return indicates error(s).
	*/
	int evaluateMessageString(const String * pMessageString)
	{
		_messageData.reset();
		if (pMessageString->length() < MIN_MESSAGE_HEADER_LENGTH)
		{
			return(MH_MSG_LENGTH_ERROR);
		}
		if (checkMessageHeader(pMessageString) != MH_OK)
		{
			return(MH_MSG_HEADER_ERROR);
		}
		if (checkMessageTargetID(pMessageString) != MH_OK)
		{
			return(MH_MSG_TARGET_ID_ERROR);
		}
		if (checkMessageCommandType(pMessageString) != MH_OK)
		{
			return(MH_MSG_COMMAND_ERROR);
		}
		if (checkMessageSenderID(pMessageString) != MH_OK)
		{
			return(MH_MSG_SENDER_ID_ERROR);
		}
		evalueCommandActions(pMessageString);
		return(MH_OK);
	}

	/*-------------------------------------------------------------------*/
	/** Sends a formatted command message to the specified target.
	--> commandID: The identifier specifying the type of message to send.
	--> targetID: The workstation/device identification number that the command is being sent to.
	--> header: Optionally encode whether this is a workstation or device message (defaults to workstation).
	--> extCmd: Optionally include an extended command. Required for extended command messages.
	*/
	void sendCommand(
		  const MessageCommandType commandID
		, const unsigned int targetID
		, const char header[3] = MSG_HEADER_WORKSTATION
		, ExtendedCommand * extCmd = NULL
		)
	{
		String _valueString = "";
		if (extCmd != NULL)
		{
			_valueString = extCmd->toString();
		}
		doSendMessage(buildMessageString(
			  commandID
			, _valueString
			, header
			, targetID
			));
		return;
	}

	/*-------------------------------------------------------------------*/
	

	/******** MEMBER ACCESS METHODS ********/
public:		
	/*-------------------------------------------------------------------*/
	//* Sets the internal workstation/device identification number.
	//* This ID is used to filter messages for only this workstation/device.
	void setID(const unsigned int id)
	{
		if ((id > 0) && (id < 1000)) { this->_id = id; }
	}
	/*-------------------------------------------------------------------*/
	//* Returns the internal workstation/device identification number.
	unsigned int getID(void) const { return(_id); }

	/*-------------------------------------------------------------------*/
	//* Returns the received message data structure.
	MessageData getMessage(void) const { return(_messageData); }

	/******** VIRTUAL INTERFACE METHODS ********/
	// Virtual Prototypes (to be implemented by inheriting class)
protected:
	//* Send a message string using the communications class reference.
	virtual void doSendMessage(const String message) { ; }

	//* Action triggered when an "OK" (Success) message has been identified.
	virtual void onMessageOK(void) { ; }
	
	//* Action triggered when a "NO" (Failure) message has been identified.
	virtual void onMessageNO(void) { ; }
	
	/** Action triggered when a "PG" (Ping Request) message has been identified.
	*	NOTE: sending a message in response to a ping is handled automatically.
	*	This action handler is provided to allow additional actions to be performed.
	*/
	virtual void onMessagePingRequest(void) { ; }
	
	//* Action triggered when a "PO" (Ping Response) message has been identified.
	virtual void onMessagePingResponse(void) { ; }
	
	//* Action triggered when a "OP" (Open Connection) message has been identified.
	virtual mh_action_ret_type onMessageOpenConnection(void) { return(MH_ACTION_RETURN_OK); }
	
	//* Action triggered when a "CL" (Close Connection) message has been identified.
	virtual mh_action_ret_type onMessageCloseConnection(void) { return(MH_ACTION_RETURN_OK); }
	
	//* Action triggered when a "RT" (Return) message has been identified.
	virtual void onMessageReturnValue(void) { ; }


	/******** EXTENDED COMMAND VIRTUAL INTERFACE METHODS ********/
	// Virtual Prototypes (to be implemented by inheriting class)
protected:
	//* Action triggered when a "SOLE" Extended Command has been identified.
	virtual mh_action_ret_type onCommandSetSolenoid(unsigned int channel, int value) { return(MH_ACTION_RETURN_OK); }
	
	//* Action triggered when a "ADC" Extended Command has been identified.
	virtual mh_action_ret_type onCommandReadADC(int * value, int channel, int gain, int filter, int mode, int level) { return(MH_ACTION_RETURN_OK); }
	
	//* Action triggered when a "DAC" Extended Command has been identified.
	virtual mh_action_ret_type onCommandWriteDAC(unsigned int channel, int value) { return(MH_ACTION_RETURN_OK); }
	
	//* Action triggered when a "SENDHOSTTEXT" Extended Command has been identified.
	virtual mh_action_ret_type onCommandSendHostText(const String text, CExtendedParameter * returnValue) { return(MH_ACTION_RETURN_OK); }
	
	//* Action triggered when a "RESETIO" Extended Command has been identified.
	virtual mh_action_ret_type onCommandResetIO(void) { return(MH_ACTION_RETURN_OK); }
		
	/******** INTERNAL MEMBER OBJECTS & VARIABLES ********/
protected:
	MessageData _messageData; //* Internal message data storage
private:
	unsigned int _id; //* Internal workstation/device identification number


	/******** INTERNAL HELPER FUNCTIONS ********/
private:
	/*-------------------------------------------------------------------*/
	/** Message must begin with "WS" or "DV" header.
	*	Message string addresses: [0] [1]
	--> Returns zero on success, otherwise non-zero.
	*/
	int checkMessageHeader(const String * pMessageString)
	{
		pMessageString->getBytes((unsigned char*)_messageData.header, 3, 0);
		if (strcmp(_messageData.header, MSG_HEADER_WORKSTATION) == 0)
		{
			return(MH_OK);
		}
		if (strcmp(_messageData.header, MSG_HEADER_DEVICE) == 0)
		{
			return(MH_OK);
		}
		return(MH_MSG_HEADER_ERROR);
	}

	/*-------------------------------------------------------------------*/
	/** Message must be for this device.
	*	Message string addresses: [2] [3] [4]
	--> Returns zero on success, otherwise non-zero.
	*/
	int checkMessageTargetID(const String * pMessageString)
	{
		char _strID[4] = "";
		pMessageString->getBytes((unsigned char*)_strID, 4, 2);
		_messageData.targetID = (unsigned int)atoi(_strID);
		if (_messageData.targetID == this->_id)
		{
			return(MH_OK);
		}
		return(MH_MSG_TARGET_ID_ERROR);
	}

	/*-------------------------------------------------------------------*/
	/** Decode message command type.
	*	Message string addresses: [5] [6]
	--> Returns zero on success, otherwise non-zero.
	*/
	int checkMessageCommandType(const String * pMessageString)
	{
		_messageData.cmdID = MCT_NONE;
		char _strCmd[3] = "";
		pMessageString->getBytes((unsigned char*)_strCmd, 3, 5);
		for (int i = 0; i < MCT_COUNT; i++)
		{
			if (strcmp(MessageCommandTextType[i], _strCmd) == 0)
			{
				_messageData.cmdID = (MessageCommandType)i;
				break;
			}
		}
		return((_messageData.cmdID != MCT_NONE) ? MH_OK : MH_MSG_COMMAND_ERROR);
	}

	/*-------------------------------------------------------------------*/
	/** Check Sender ID - cannot be the same as this device! (?)
	*	Message string addresses: [7] [8] [9] [10]
	--> Returns zero on success, otherwise non-zero.
	*/
	int checkMessageSenderID(const String * pMessageString)
	{
		char _strID[5] = "";
		pMessageString->getBytes((unsigned char*)_strID, 5, 7);
		_messageData.senderID = (unsigned int)atoi(_strID);
		if ((_messageData.senderID > 0) &&
			(_messageData.senderID < 1000) &&
			(_messageData.senderID != this->_id))
		{
			// TODO: /\/\/\ is it OK to send messages to myself?

			return(MH_OK);
		}
		return(MH_MSG_SENDER_ID_ERROR);
	}


	/*-------------------------------------------------------------------*/
	/** Trigger command actions from the received message.
	*/
	void evalueCommandActions(const String * pMessageString)
	{
		//* Message data string for Extended Command or Return Value.
		//* Message string address [15] --> [end]
		//* (may be empty!)
		String _dataString = 
			(pMessageString->length() < 15) ?
			"" :
			pMessageString->substring(15);

		// Check which command was received:
		switch (_messageData.cmdID)
		{
		case MCT_OK:
			_messageData.extended.returnValue.setValue(_dataString);
			onMessageOK();
			break;

		case MCT_NO:
			_messageData.extended.returnValue.setValue(_dataString);
			onMessageNO();
			break;

		case MCT_PING_REQUEST: commandPingRequest(); break;

		case MCT_PING_RESPONSE: onMessagePingResponse(); break;

		case MCT_CONNECTION_OPEN: commandOpenConnection(); break;

		case MCT_CONNECTION_CLOSE: commandCloseConnection(); break;

		case MCT_EXTENDED: evaluateExtendedCommand(&_dataString); break;

		case MCT_RETURN:
			_messageData.extended.returnValue.setValue(_dataString);
			onMessageReturnValue();
			break;

		default:
			// Undefined message command type - what to do?
			break;
		}
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Perform "Ping Request" command actions.
	*/
	void commandPingRequest(void)
	{
		onMessagePingRequest();
		doSendMessage(buildMessageString(MCT_PING_RESPONSE));
		return;
	}

	/*-------------------------------------------------------------------*/
	/** Perform "Open Connection" command actions.
	*/
	void commandOpenConnection(void)
	{
		if (onMessageOpenConnection() == 0)
		{
			doSendMessage(buildMessageString(MCT_OK));
		}
		else
		{
			doSendMessage(buildMessageString(MCT_NO));
		}
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Perform "Close Connection" command actions.
	*/
	void commandCloseConnection(void)
	{
		if (onMessageCloseConnection() == 0)
		{
			doSendMessage(buildMessageString(MCT_OK));
		}
		else
		{
			doSendMessage(buildMessageString(MCT_NO));
		}
		return;
	}

	
	/*-------------------------------------------------------------------*/
	/** Trigger extended command actions from the received message.
	*/
	void evaluateExtendedCommand(const String * pCommandString)
	{
		checkExtendedCommandType(pCommandString);
		parseExtendedParameters(pCommandString);
		ExtendedCommand * pEC = &_messageData.extended;
		switch (_messageData.extended.cmdID)
		{
		case ECT_SET_SOLENOID: extendedCommandSetSolenoid(pEC); break;

		case ECT_READ_ANALOG: extendedCommandReadAnalog(pEC); break;

		case ECT_WRITE_ANALOG: extendedCommandWriteAnalog(pEC); break;

		case ECT_SEND_HOST_TEXT: extendedCommandSendHostText(pEC); break;

		case ECT_RESET_IO: extendedCommandResetIO(pEC); break;

		default:
			// Undefined extended command type - what to do?
			break;
		}
		return;
	}

	
	/*-------------------------------------------------------------------*/
	/** Decode extended command type.
	*/
	void checkExtendedCommandType(const String * pCommandString)
	{
		_messageData.extended.cmdID = ECT_NONE;
		int _length = pCommandString->indexOf('(');
		if (_length < 0) { _length = 0; }
		String _strCmd = pCommandString->substring(0, _length);
		for (int i = 0; i < ECT_COUNT; i++)
		{
			if (_strCmd == ExtendedCommandTextType[i])
			{
				_messageData.extended.cmdID = (ExtendedCommandType)i;
				break;
			}
		}
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Parses an extended command string and sets any specified parameters.
	*   Check for data between identifier characters (Parentheses).
	*   Identify and set parameter values (separated by commas).
	*/
	void parseExtendedParameters(const String * pCommandString)
	{
		String _paramStr = parseParameterStringBetween(pCommandString, '(', ')');
		if (_paramStr.length() == 0) { return; }
		
		int iS = -1; //* separator index
		int iP = -1; //* previous separator index
		for (byte i = 0; 1 < MAX_EXTENDED_PARAMETERS; i++)
		{
			iS = _paramStr.indexOf(',', (iP + 1));
			this->_messageData.extended.parameter[i].setValue(
				_paramStr.substring(
				(iP + 1)
				, (iS != -1) ? iS : _paramStr.length()
				));
			iP = iS;
			this->_messageData.extended.parameterCount++;
			if (iS == -1) { break; }
		}
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Return the indexes from between the specified beginning and ending characters.
	*   Sets the indexes to -1 if the character(s) are not found.
	*/
	String parseParameterStringBetween(
		  const String * input
		, const char &begin
		, const char &end
		)
	{
		int _index[2] = { -1, -1 };
		_index[0] = input->indexOf(begin);
		_index[1] = input->indexOf(end, _index[0] + 1);
		if ((_index[0] == -1) || (_index[1] == -1)) { return(""); }
		return input->substring((_index[0] + 1), _index[1]);
	}


	/*-------------------------------------------------------------------*/
	/** Perform the Set Solenoid extended command function.
	*/
	void extendedCommandSetSolenoid(ExtendedCommand * extCmd)
	{
		// Validate parameters...
		if ((extCmd->parameterCount != 2) ||
			(extCmd->parameter[0].getType() != CExtendedParameter::PT_INTEGER) ||
			(extCmd->parameter[1].getType() != CExtendedParameter::PT_INTEGER))
		{
			// Parameters do no match requirements!
			extCmd->returnValue.setValue(MH_MSG_PARAMETER_ERROR);
			doSendMessage(buildMessageString(MCT_NO
				, extCmd->returnValue.getString()
				));
			return;
		}
		if (onCommandSetSolenoid(
			  extCmd->parameter[0].toInt()
			, extCmd->parameter[1].toInt()
			) == MH_ACTION_RETURN_OK)
		{
			doSendMessage(buildMessageString(MCT_OK));
		}
		else
		{
			doSendMessage(buildMessageString(MCT_NO));
		}
		return;
	}

	/*-------------------------------------------------------------------*/
	/** Perform the Read Analog (ADC) extended command function.
	*/
	void extendedCommandReadAnalog(ExtendedCommand * extCmd)
	{
		// Validate parameters...
		if ((extCmd->parameterCount != 5) ||
			(extCmd->parameter[0].getType() != CExtendedParameter::PT_INTEGER) ||
			(extCmd->parameter[1].getType() != CExtendedParameter::PT_INTEGER) ||
			(extCmd->parameter[2].getType() != CExtendedParameter::PT_INTEGER) ||
			(extCmd->parameter[3].getType() != CExtendedParameter::PT_INTEGER) ||
			(extCmd->parameter[4].getType() != CExtendedParameter::PT_INTEGER))
		{
			// Parameters do no match requirements!
			extCmd->returnValue.setValue(MH_MSG_PARAMETER_ERROR);
			doSendMessage(buildMessageString(MCT_NO
				, extCmd->returnValue.getString()
				));
			return;
		}
		int _value = 0;
		mh_action_ret_type _ret =
			onCommandReadADC(
			  &_value
			, extCmd->parameter[0].toInt()
			, extCmd->parameter[1].toInt()
			, extCmd->parameter[2].toInt()
			, extCmd->parameter[3].toInt()
			, extCmd->parameter[4].toInt()
			);
		switch (_ret)
		{
		case MH_ACTION_RETURN_OK: // no break - fall thru to return value.
		case MH_ACTION_RETURN_OKVAL:
			extCmd->returnValue.setValue(_value);
			doSendMessage(buildMessageString(MCT_RETURN
				, extCmd->returnValue.getString()
				));
			break;

		case MH_ACTION_RETURN_ERR: // no break - fall thru to default.
		default:
			doSendMessage(buildMessageString(MCT_NO));
			break;
		}
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Perform the Write Analog (DAC) extended command function.
	*/
	void extendedCommandWriteAnalog(ExtendedCommand * extCmd)
	{
		// Validate parameters...
		if ((extCmd->parameterCount != 2) ||
			(extCmd->parameter[0].getType() != CExtendedParameter::PT_INTEGER) ||
			(extCmd->parameter[1].getType() != CExtendedParameter::PT_INTEGER))
		{
			// Parameters do no match requirements!
			extCmd->returnValue.setValue(MH_MSG_PARAMETER_ERROR);
			doSendMessage(buildMessageString(MCT_NO
				, extCmd->returnValue.getString()
				));
			return;
		}
		if (onCommandWriteDAC(
			  extCmd->parameter[0].toInt()
			, extCmd->parameter[1].toInt()
			) == MH_ACTION_RETURN_OK)
		{
			doSendMessage(buildMessageString(MCT_OK));
		}
		else
		{
			doSendMessage(buildMessageString(MCT_NO));
		}
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Perform the Send Host Text extended command function.
	*/
	void extendedCommandSendHostText(ExtendedCommand * extCmd)
	{
		// Validate parameters...
		if ((extCmd->parameterCount != 1) ||
			(extCmd->parameter[0].getType() == CExtendedParameter::PT_EMPTY))
		{
			// Parameters do no match requirements!
			extCmd->returnValue.setValue(MH_MSG_PARAMETER_ERROR);
			doSendMessage(buildMessageString(MCT_NO
				, extCmd->returnValue.getString()
				));
			return;
		}
		mh_action_ret_type _ret =
			onCommandSendHostText(
			    extCmd->parameter[0].getString()
			, &(extCmd->returnValue)
			);
		switch (_ret)
		{
		case MH_ACTION_RETURN_OK:
			doSendMessage(buildMessageString(MCT_OK));
			break;

		case MH_ACTION_RETURN_OKVAL:
			doSendMessage(buildMessageString(MCT_RETURN
				, extCmd->returnValue.getString()
				));
			break;

		case MH_ACTION_RETURN_ERR: // no break - fall thru to default
		default:
			doSendMessage(buildMessageString(MCT_NO
				, extCmd->returnValue.getString()
				));
			break;
		}
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Perform the Reset IO extended command function.
	*/
	void extendedCommandResetIO(ExtendedCommand * extCmd)
	{
		if (onCommandResetIO() == MH_ACTION_RETURN_OK)
		{
			doSendMessage(buildMessageString(MCT_OK));
		}
		else
		{
			doSendMessage(buildMessageString(MCT_NO));
		}
		return;
	}


	/*-------------------------------------------------------------------*/
	/** Build a formatted message string for the given target, command and value.
	--> valueString: may be empty if no return or extended command value is required.
	--> header: may be omitted - will use header from last received message, or "WS" if none.
	--> targetID: may be omitted - will use senderID from last message.
	*/
	String buildMessageString(
		  const MessageCommandType commandID
		, const String valueString = ""
		, const char header[3] = ""
		, const unsigned int targetID = 0
		) const
	{
		String _messageString = "";
		_messageString.concat(
			(header[0] != 0) ? header :
			(_messageData.header[0] != 0) ? _messageData.header :
			MSG_HEADER_WORKSTATION
			);
		unsigned int _targetID;
		_targetID = (targetID != 0) ? targetID : _messageData.senderID;
		_messageString.concat(makePaddedNumberString(_targetID, 3));
		_messageString.concat(MessageCommandTextType[commandID]);
		_messageString.concat(makePaddedNumberString(this->_id, 4));
		if (((commandID == MCT_RETURN) || 
			(commandID == MCT_EXTENDED)) &&
			(valueString.length() != 0))
		{
			_messageString.concat(makePaddedNumberString(valueString.length(), 4));
			_messageString.concat(valueString);
		}
		return(_messageString);
	}


	/*-------------------------------------------------------------------*/
	/** Returns a string representation of a number that is a fixed width and padded with zeros.
	*/
	template <typename T> String makePaddedNumberString(
		const T number
		, const byte width
		) const
	{
		String _temp = "";
		T _currentMax = 10;
		for (byte i = 1; i < width; i++)
		{
			if (number < _currentMax)
			{
				_temp.concat('0');
			}
			_currentMax *= 10;
		}
		_temp.concat(number);
		return(_temp);
	}


};

#endif // _MESSAGEHANDLER_HEADER_