/*
 * ReceivingCommand.h
 *
 *  Created on: 8. apr. 2017
 *      Author: per-olavr
 */

#ifndef RECEIVINGCOMMAND_H_
#define RECEIVINGCOMMAND_H_

#define MAX_COMMAND_LENGTH 	8

enum CommandState {
	EMPTY, RECEIVING, NEW_RECEIVED
};

struct Command {
	enum CommandState state;
	int byteCounter;
	char receiveBuffer[MAX_COMMAND_LENGTH];
	unsigned long startReceivingTime;
};

#endif /* RECEIVINGCOMMAND_H_ */
