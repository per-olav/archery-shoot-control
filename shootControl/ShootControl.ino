#include "Arduino.h"
#include <SoftwareSerial.h>

#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 9   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW
#define MAX_COMMAND_LENGTH 	8

#define ArrowLedPinStart 3
#define ArrowLedPinEnd 8

SoftwareSerial _rs485Serial(SSerialRX, SSerialTX); // RX, TX

enum CommandState {
	EMPTY, RECEIVING, NEW_RECEIVED
};

struct Command {
	enum CommandState state = EMPTY;
	int byteCounter = 0;
	char receiveBuffer[MAX_COMMAND_LENGTH] = "";
	unsigned long startReceivingTime;
};
struct Command _command;

#define PUSH_STEPS 100

enum PushButtons {
	ABCD_CHANGE = 30,
	DECREASE_ARROWS = 31,
	RESET = 32,
	GREEN_LIGHT = 33,
	YELLOW_LIGHT = 34,
	RED_LIGHT = 35,
	SOUND_LIGHT = 36,
	START_SEQUENCE = 37,
	PAUSE_SEQUENCE = 38,
	STOP_SEQUENCE = 39
} _pushButtons;
unsigned long _noOns[STOP_SEQUENCE + 1];
unsigned long _noOffs[STOP_SEQUENCE + 1];
bool _buttonPushed[STOP_SEQUENCE + 1];
bool _buttonsEnabled[STOP_SEQUENCE + 1];

enum RotarySwitches {
	ARROWS_THREE = 22,
	ARROWS_SIX = 23,
	SEQUENCE_ABC = 24,
	SEQUENCE_ABCD_CDAB = 25,
	SEQUENCE_ABCD_ABCD = 26
} _rotarySwitches;
int _switchStates[SEQUENCE_ABCD_ABCD + 1];

/*
 void handleButtonPushes();
 void performAction(int);
 void sendResetSequence();
 void sendGreenLightOn();
 void sendYellowLightOn();
 void sendRedLightOn();
 void sendSoundSignal();
 void sendStartSequence();
 void sendPauseSequence();
 void sendStopSequence();
 void updateStatus(int);
 void decreaseNoArrows();
 void changeABCD();
 */

enum AbcdLight {
	AB = 0, CD = 1
};

enum SequenceType {
	ABC, ABCD_CDAB, ABCD_ABCD
};

struct Status {
	int numberOfArrows = 3;
	AbcdLight abcdLight = AB;
	SequenceType sequenceType = ABC; //
	bool sequenceIsRunning = false; //
	bool isShooting = false;
	int countDownSeconds = 0;
};

Status _status;

void setup() {
	Serial.begin(9600);

	for (int i = ARROWS_THREE; i <= SEQUENCE_ABCD_ABCD; i++) {
		_switchStates[i] = 0;
	}

	for (int i = ABCD_CHANGE; i <= STOP_SEQUENCE; i++) {
		pinMode(i, INPUT);
		// test false
		_buttonsEnabled[i] = true;
	}

	for (int i = ARROWS_THREE; i <= SEQUENCE_ABCD_ABCD; i++) {
		// test false
		_buttonsEnabled[i] = true;
		pinMode(i, INPUT);
	}

	pinMode(SSerialTxControl, OUTPUT);

	digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver

	// Start the software serial port, to another device
	_rs485Serial.begin(4800);   // set the data rate
}
unsigned long _c;

void loop() {
	_c++;

	// Read command
	updateCommand(&_command);

	// Do received command
	if (_command.state == NEW_RECEIVED) {
		Serial.println("Do command");
		Serial.println(_command.receiveBuffer);
		doCommand(&_command);
		resetCommand(&_command);
	}

	handleButtonPushes();

	handleSwitches();

	// Set arrow leds
	for (int i = 0; i < 6; i++) {
		int pin = ArrowLedPinStart + i;
		bool ledOn = (i < _status.numberOfArrows);
		if (i == _status.numberOfArrows - 1 && _status.isShooting) {
			ledOn = (millis() / 500) % 2 == 0;
		}
		digitalWrite((uint8_t) pin, ledOn ? HIGH : LOW);
	}
}

void handleButtonPushes() {
	for (int button = ABCD_CHANGE; button <= STOP_SEQUENCE; button++) {
		bool bPushed = _buttonsEnabled[button]
				&& digitalRead((unsigned char) button) == HIGH;

		if (!_buttonPushed[button]) {
			if (bPushed) {
				++_noOns[button];
			} else {
				_noOns[button] = 0;
			}

			if (_noOns[button] == (unsigned long) PUSH_STEPS) {
				++_noOns[button];
				_noOffs[button] = 0;
				_buttonPushed[button] = true;
				Serial.print("Button: ");
				Serial.println(button);
				performAction(button);
			}
		} else {
			if (!bPushed) {
				++_noOffs[button];
			} else {
				_noOffs[button] = 0;
			}
			if (_noOffs[button] == (unsigned long) PUSH_STEPS) {
				++_noOffs[button];
				_buttonPushed[button] = false;
			}
		}
	}
}

void handleSwitches() {
	for (int s = ARROWS_THREE; s <= SEQUENCE_ABCD_ABCD; s++) {
		int prevState = _switchStates[s];
		_switchStates[s] = _buttonsEnabled[s]
				&& digitalRead((unsigned char) s) == HIGH;
		if (_switchStates[s] && !prevState) {
			updateStatus(s);
		}
	}
}

void updateCommand(struct Command *cP) {

	if (millis() - cP->startReceivingTime > 200) {
		resetCommand(cP);
	}

	if (_rs485Serial.available() == 0) {
		return;
	}

	char byteReceived = (char) _rs485Serial.read();

	// For debuging
	//Serial.print("Byte received: ");
	//if (byteReceived > 0){
	//	Serial.println(byteReceived);
	//}else{
	//	Serial.println("end mark");
	//}

	++cP->byteCounter;
	if (cP->byteCounter == 1) {
		cP->startReceivingTime = millis();
	}

	boolean commandEnd = (byteReceived == 0
			|| cP->byteCounter == MAX_COMMAND_LENGTH);
	if (commandEnd) {
		cP->receiveBuffer[cP->byteCounter - 1] = '\0';
		cP->state = NEW_RECEIVED;
	} else {
		cP->receiveBuffer[cP->byteCounter - 1] = byteReceived;
		cP->state = RECEIVING;
	}
}

void resetCommand(struct Command *cP) {
	cP->byteCounter = 0;
	cP->state = EMPTY;
}

void doCommand(struct Command * commandP) {
	char cType = commandP->receiveBuffer[0];
	int noArrowsLeft;
	switch (cType) {
	case 'A':
		char s[2];
		s[0] = commandP->receiveBuffer[1];
		s[1] = '\0';
		noArrowsLeft = atoi(s);
		if (noArrowsLeft > 0) {
			_status.numberOfArrows = noArrowsLeft;
		} else {
			_status.numberOfArrows =
					digitalRead((unsigned char) ARROWS_THREE) == HIGH ? 3 : 6;
		}
		break;
	case 'S':
		_status.isShooting = commandP->receiveBuffer[1] == '1';
		break;
	case 'R':
		_status.sequenceIsRunning = commandP->receiveBuffer[1] == '1';
		if (!_status.sequenceIsRunning) {
			setDefaultNumberOfArrows();
		}
		break;
	case 'a':
		_status.abcdLight = commandP->receiveBuffer[1] == '1' ? AB : CD;
		break;
	case 'T':
		int t = atoi(commandP->receiveBuffer + 1);
		_status.countDownSeconds = t;
		Serial.println(t);
	}
}

void updateStatus(int s) {
	Serial.print("Update status: ");
	Serial.println(s);
	switch (s) {
	case ARROWS_THREE:
		_status.numberOfArrows = 3;
		break;
	case ARROWS_SIX:
		_status.numberOfArrows = 6;
		break;
	case SEQUENCE_ABC:
		_status.sequenceType = ABC;
		break;
	case SEQUENCE_ABCD_CDAB:
		_status.sequenceType = ABCD_CDAB;
		break;
	case SEQUENCE_ABCD_ABCD:
		_status.sequenceType = ABCD_ABCD;
	}
}

void performAction(int buttonPushed) {
	Serial.print("Perform action for button ");
	Serial.println(buttonPushed);

	switch (buttonPushed) {
	case ABCD_CHANGE:
		changeABCD();
		break;
	case DECREASE_ARROWS:
		if (!_status.sequenceIsRunning) {
			decreaseNoArrows();
		}
		break;
	case RESET:
		sendResetSequence();
		break;
	case GREEN_LIGHT:
		sendGreenLightOn();
		break;
	case YELLOW_LIGHT:
		sendYellowLightOn();
		break;
	case RED_LIGHT:
		sendRedLightOn();
		break;
	case SOUND_LIGHT:
		sendSoundSignal();
		break;
	case START_SEQUENCE:
		sendStartSequence();
		break;
	case PAUSE_SEQUENCE:
		sendPauseSequence();
		break;
	case STOP_SEQUENCE:
		sendStopSequence();
	}
}

void changeABCD() {
	digitalWrite(SSerialTxControl, RS485Transmit);
	if (_status.abcdLight == AB) {
		_status.abcdLight = CD;
		_rs485Serial.write("A2");
		_rs485Serial.write((unsigned char) '\0');
		Serial.println("Sends command A2");
	} else {
		_status.abcdLight = AB;
		_rs485Serial.write("A1");
		_rs485Serial.write((unsigned char) '\0');
		Serial.println("Sends command A1");
	}
	digitalWrite(SSerialTxControl, RS485Receive);
}

void decreaseNoArrows() {
	if (_status.numberOfArrows > 1) {
		--_status.numberOfArrows;
	} else {
		setDefaultNumberOfArrows();
	}
}

void setDefaultNumberOfArrows() {
	if (_buttonsEnabled[ARROWS_THREE]
			&& digitalRead((unsigned char) ARROWS_THREE) == HIGH) {
		_status.numberOfArrows = 3;
	} else {
		_status.numberOfArrows = 6;
	}
}

void sendResetSequence() {
	// "R": Reset to default (red light)
	digitalWrite(SSerialTxControl, RS485Transmit);
	_rs485Serial.write("R");
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RS485Receive);
}

void sendGreenLightOn() {
// "LGREEN": Green light on only
	digitalWrite(SSerialTxControl, RS485Transmit);
	_rs485Serial.write("LGREEN");
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RS485Receive);
}

void sendYellowLightOn() {
// "LYELLO": Yellow light on only
	digitalWrite(SSerialTxControl, RS485Transmit);
	_rs485Serial.write("LYELLO");
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RS485Receive);
}

void sendRedLightOn() {
// "LRED": Red light on only
	digitalWrite(SSerialTxControl, RS485Transmit);
	_rs485Serial.write("LRED");
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RS485Receive);
}

void sendSoundSignal() {
// "ZN": N sound signals
	digitalWrite(SSerialTxControl, RS485Transmit);
	_rs485Serial.write("Z1");
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RS485Receive);
}

void sendStartSequence() {
// "S1 TTT": Start single sequence with green time TTT
// "S2 TTT": Start double sequence AB-CD/CD-AB with green time TTT
// "S3 TTT": Start double sequence AB-CD/AB-CD with green time TTT
	char sequenceCommand[7];
	sequenceCommand[0] = 'S';
	if (_status.sequenceType == ABC) {
		sequenceCommand[1] = '1';
	} else if (_status.sequenceType == ABCD_CDAB) {
		sequenceCommand[1] = '2';
	} else {
		sequenceCommand[1] = '3';
	}
	sequenceCommand[2] = ' ';

	char s[4];
	s[3] = '\0';
	int greenSeconds = _status.numberOfArrows * 40 - 30;
	sprintf(s, "%03d", greenSeconds);
	sequenceCommand[3] = s[0];
	sequenceCommand[4] = s[1];
	sequenceCommand[5] = s[2];
	sequenceCommand[6] = '\0';
	Serial.print("Send sequence command: ");
	Serial.println(sequenceCommand);

	digitalWrite(SSerialTxControl, RS485Transmit);
	_rs485Serial.write(sequenceCommand);
	digitalWrite(SSerialTxControl, RS485Receive);
}

void sendPauseSequence() {
// "P": Pause
	digitalWrite(SSerialTxControl, RS485Transmit);
	_rs485Serial.write("P");
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RS485Receive);
}

void sendStopSequence() {
// "I": Interrupt
	digitalWrite(SSerialTxControl, RS485Transmit);
	_rs485Serial.write("I");
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RS485Receive);

	if (digitalRead((unsigned char) ARROWS_THREE) == HIGH) {
		_status.numberOfArrows = 3;
	} else {
		_status.numberOfArrows = 6;
	}
}
