// IBM (C) Copyright 2010 Eclipse Public License
// http://www.eclipse.org/org/documents/epl-v10.html
#include "smSocket.h"
#include "vmapiVirtual.h"
#include "smapiTableParser.h"
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define PARSER_TABLE_NAME     Virtual_Network_LAN_Access_Layout
#define OUTPUT_STRUCTURE_NAME vmApiVirtualNetworkLanAccessOutput

/**
 * Virtual_Network_LAN_Access SMAPI interface
 */
int smVirtual_Network_LAN_Access(struct _VmApiInternalContext* vmapiContextP,
		char * userid, int passwordLength, char * password,
		char * targetIdentifier, char * lanName, char * lanOwner,
		char * accessOption, char * accessGrantName, char * promiscuity,
		vmApiVirtualNetworkLanAccessOutput ** outData) {
	const char * const functionName = "Virtual_Network_LAN_Access";
	tableParserParms parserParms;
	int tempSize;
	char * cursor;
	char * stringCursor; // Used for outData string area pointer
	int arrayCount;
	int totalStringSize;
	int rc;
	int sockDesc;
	int requestId;

	int inputSize = 4 + 4 + strlen(functionName) + 4 + strlen(userid) + 4
			+ passwordLength + 4 + strlen(targetIdentifier) + strlen(lanName)
			+ 1 + strlen(lanOwner) + 1 + strlen(accessOption) + 1 + strlen(
			accessGrantName) + 1 + strlen(promiscuity) + 1;
	char * inputP = 0;
	char * smapiOutputP = 0;
	char line[LINESIZE];
	int i;

	TRACE_ENTRY_FLOW(vmapiContextP, TRACEAREA_RESOURCE_LAYER);

	// Build SMAPI input parameter buffer
	if (0 == (inputP = malloc(inputSize)))
		return MEMORY_ERROR;
	cursor = inputP;
	PUT_INT(inputSize - 4, cursor);

	tempSize = strlen(functionName);
	PUT_INT(tempSize, cursor);
	memcpy(cursor, functionName, tempSize);
	cursor += tempSize;

	tempSize = strlen(userid); // Userid 1..8 or 0..8 chars
	PUT_INT(tempSize, cursor);
	if (tempSize > 0) {
		memcpy(cursor, userid, tempSize);
		cursor += tempSize;
	}

	tempSize = passwordLength; // Password 1..200 or 0..200 chars
	PUT_INT(tempSize, cursor);
	if (tempSize > 0) {
		memcpy(cursor, password, tempSize);
		cursor += tempSize;
	}

	tempSize = strlen(targetIdentifier); // Target identifier 1..8
	PUT_INT(tempSize, cursor);
	memcpy(cursor, targetIdentifier, tempSize);
	cursor += tempSize;

	tempSize = strlen(lanName); // Lan name 1..8 chars
	strcpy(cursor, lanName);
	cursor += tempSize + 1;

	tempSize = strlen(lanOwner); // Lan owner 1..8 chars
	strcpy(cursor, lanOwner);
	cursor += tempSize + 1;

	tempSize = strlen(accessOption); // Access option GRANT or REVOKE
	strcpy(cursor, accessOption);
	cursor += tempSize + 1;

	tempSize = strlen(accessGrantName); // Access grant user 1..8 chars
	strcpy(cursor, accessGrantName);
	cursor += tempSize + 1;

	tempSize = strlen(promiscuity); // Promiscuous or nonpromiscuous
	strcpy(cursor, promiscuity);
	cursor += tempSize + 1;

	// This routine will send SMAPI the input, delete the input storage
	// and call the table parser to set the output in outData
	rc = getAndParseSmapiBuffer(vmapiContextP, &inputP, inputSize,
			PARSER_TABLE_NAME, // Integer table
			TO_STRING(PARSER_TABLE_NAME), // String name of the table
			(char * *) outData);
	return rc;
}
