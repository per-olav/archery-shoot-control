#include <arduino.h>
#include <RFTransmitter.h>
#include <EEPROM.h>
#include <string.h>

#define NODE_ID          1
#define OUTPUT_PIN      12

#define LED_PIN         10
#define MAX_KEY_SEQUENCE 16

typedef enum {
	NOARROWS_THREE = 3, NOARROWS_SIX = 6
} NoArrows;
NoArrows noArrows;

typedef enum {
	SINGLE = 0, ABCD_CDAB = 1, ABCD_ABCD = 2
} ABCDSequence;
ABCDSequence abcdSequence;

typedef enum {
	EMPTY, RECEIVING, UNHANDLED
} KeySequenceStatus;

struct KeySequence {
	KeySequenceStatus status = EMPTY;
	char sequence[MAX_KEY_SEQUENCE + 1];
	int noKeys = 0;
};
KeySequence keySequence;

RFTransmitter transmitter(OUTPUT_PIN, NODE_ID);

byte row_pins[4] = { 2, 3, 4, 5 };
byte col_pins[4] = { 6, 7, 8, 9 };
char keys[16] = { 'D', 'C', 'B', 'A', //
		'#', '9', '6', '3', //
		'0', '8', '5', '2', //
		'*', '7', '4', '1' };
char key = 0;

int timeNextRound;
unsigned long lastTimePushed = 0;

void setup() {
	Serial.begin(9600);
	Serial.println("Start");

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);

	uint8_t narrs = EEPROM.read(0);
	if ((int) narrs == 3) {
		noArrows = NOARROWS_THREE;
		timeNextRound = 120;
	} else if ((int) narrs == 6) {
		noArrows = NOARROWS_SIX;
		timeNextRound = 240;
	} else {
		noArrows = NOARROWS_THREE;
		timeNextRound = 120;
	}

	Serial.print("Number of arrows: ");
	Serial.println(noArrows);

	uint8_t abcd = EEPROM.read(1);
	if ((int) abcd == 0) {
		abcdSequence = SINGLE;
	} else if ((int) abcd == 1) {
		abcdSequence = ABCD_CDAB;
	} else if ((int) abcd == 2) {
		abcdSequence = ABCD_ABCD;
	} else {
		abcdSequence = SINGLE;
	}

	for (byte i = 0; i < 4; i++) {
		pinMode(row_pins[i], INPUT);
		pinMode(col_pins[i], INPUT_PULLUP);
	}

}

void loop() {
	char newKey = getKey();
	bool isPushed = newKey > 0 && key == 0;

	if (isPushed){
		lastTimePushed = millis();
	}

	digitalWrite(LED_PIN, millis() - lastTimePushed > 300 ? HIGH : LOW);

	key = newKey;
	if (isPushed) {
		Serial.print("Key ");
		Serial.println(key);

		if (key == '*') {
			keySequence.status = RECEIVING;
			keySequence.noKeys = 0;
		} else if (key == '#') {
			keySequence.status = UNHANDLED;
			keySequence.sequence[keySequence.noKeys] = '\0';
		}

		if ((keySequence.status == RECEIVING)
				&& keySequence.noKeys < MAX_KEY_SEQUENCE) {
			keySequence.sequence[keySequence.noKeys] = key;
			++keySequence.noKeys;
		}
	}

	// Single key command
	if (isPushed && keySequence.status == EMPTY) {
		if (key == 'A') {
			sendStart(); // Start
		} else if (key == 'B') {
			sendMessage("P"); // Pause
		} else if (key == 'C') {
			sendMessage("C"); // Continue
		} else if (key == 'D') {
			sendMessage("I"); // Interrupt
		} else if (key == '0') {
			sendMessage("Z1"); // Sound
		} else if (key == '1' && timeNextRound > 40) {
			timeNextRound -= 40;
			Serial.print("Time next round ");
			Serial.println(timeNextRound);
		} else if (key == '2' && timeNextRound <= 200) {
			timeNextRound += 40;
			Serial.print("Time next round ");
			Serial.println(timeNextRound);
		} else if (key == '7') {
			sendMessage("A1"); // AB next
		} else if (key == '8') {
			sendMessage("A2"); // CD next
		}
	}

	// Handle the key sequence
	if (keySequence.status == UNHANDLED) {
		handleKeySequence();
		emptyKeySequence();
	}

}

void sendStart() {
	char msg[7] = "SX XXX";

	switch (abcdSequence) {
	case SINGLE:
		msg[1] = '1';
		break;
	case ABCD_CDAB:
		msg[1] = '2';
		break;
	case ABCD_ABCD:
		msg[1] = '3';
		break;
	default:
		msg[1] = '1';
	}

	sprintf(&msg[3], "%03d", timeNextRound);
	msg[6] = '\0';

	sendMessage(msg);

	timeNextRound = noArrows == NOARROWS_THREE ? 120 : 240;
}

void sendMessage(const char* msg) {
	Serial.print("Sending message [");
	Serial.print(msg);
	Serial.println("]");

	transmitter.send((byte *) msg, strlen(msg) + 1);
	transmitter.resend((byte *) msg, strlen(msg) + 1);
}

void handleKeySequence() {
	Serial.print("Key sequence: ");
	Serial.println(keySequence.sequence);

	if (strcmp(keySequence.sequence, "*3") == 0) {
		Serial.println("Three arrows");
		noArrows = NOARROWS_THREE;
		timeNextRound = 120;
		EEPROM.update(0, (uint8_t) 3);
	} else if (strcmp(keySequence.sequence, "*6") == 0) {
		Serial.println("Six arrows");
		noArrows = NOARROWS_SIX;
		timeNextRound = 240;
		EEPROM.update(0, (uint8_t) 6);
	}

	if (strcmp(keySequence.sequence, "*ABC") == 0) {
		Serial.println("Single round");
		abcdSequence = SINGLE;
		EEPROM.update(1, (uint8_t) 0);
	} else if (strcmp(keySequence.sequence, "*ABCD") == 0) {
		Serial.println("ABCD CDAB");
		abcdSequence = ABCD_CDAB;
		EEPROM.update(1, (uint8_t) 1);
	} else if (strcmp(keySequence.sequence, "*ABAB") == 0) {
		Serial.println("ABCD ABCD");
		abcdSequence = ABCD_ABCD;
		EEPROM.update(1, (uint8_t) 2);
	}

	if (strcmp(keySequence.sequence, "*0") == 0) {
		sendMessage("R"); // Reset
	}

}

void emptyKeySequence() {
	keySequence.status = EMPTY;
	keySequence.noKeys = 0;
	keySequence.sequence[0] = '\0';
}

char getKey() {
	char key = 0;
	for (byte row = 0; row < 4; row++) {
		pinMode(row_pins[row], OUTPUT);
		digitalWrite(row_pins[row], LOW);
		for (byte col = 0; col < 4; col++) {
			if (digitalRead(col_pins[col]) == LOW) {
				key = keys[row * 4 + col];
			}
		}
		pinMode(row_pins[row], INPUT);
	}

	return key;
}
