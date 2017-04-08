/*
 * ReceivingCommand.h
 *
 *  Created on: 8. apr. 2017
 *      Author: per-olavr
 */

#ifndef RECEIVINGCOMMAND_H_
#define RECEIVINGCOMMAND_H_

#define MAX_COMMAND_LENGTH 	8

//#include <SoftwareSerial.h>

enum CommandState {
	EMPTY, RECEIVING, NEW_RECEIVED
};

struct Command {
	enum CommandState state;// = EMPTY;
	int byteCounter;// = 0;
	char receiveBuffer[MAX_COMMAND_LENGTH];// = "";
	unsigned long startReceivingTime;
};

void updateCommand(struct Command *, void *);

#endif /* RECEIVINGCOMMAND_H_ */
