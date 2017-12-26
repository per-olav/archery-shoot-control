#include <SoftwareSerial.h>
#include <FastLED.h>
#define LEDS_PER_SEGMENT 4
#define NO_SEGMENTS 7
#define NUM_LEDS 84

#define NO_CHARACTERS 19
#define LED_CONTROL_PIN 6

CRGB leds[NUM_LEDS];

#define SSerialTxControl 9   //RS485 Direction control
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin
#define RS485Transmit    HIGH
#define RS485Receive     LOW
#define MAX_COMMAND_LENGTH 8

#define LEFT	1
#define MIDDLE 	2
#define RIGHT	3

struct HSVColor {
	int hue;
	int saturation = 255;
	int intensity = 255;
};
HSVColor _color;

bool _isShooting;

int _shootTimer;
int _waitTimer;

// 2    --
// 3   !  !  1
// 0    --
// 4   !  !  6
// 5    --
char segmentCodes[NO_CHARACTERS][NO_SEGMENTS] = { //
		{ 0, 1, 1, 1, 1, 1, 1 }, // row 0 - character 0
				{ 0, 1, 0, 0, 0, 0, 1 }, // row 1 - character 1
				{ 1, 1, 1, 0, 1, 1, 0 }, // row 2 - character 2
				{ 1, 1, 1, 0, 0, 1, 1 }, // row 3 - character 3
				{ 1, 1, 0, 1, 0, 0, 1 }, // row 4 - character 4
				{ 1, 0, 1, 1, 0, 1, 1 }, // row 5 - character 5
				{ 1, 0, 1, 1, 1, 1, 1 }, // row 6 - character 6
				{ 0, 1, 1, 0, 0, 0, 1 }, // row 7 - character 7
				{ 1, 1, 1, 1, 1, 1, 1 }, // row 8 - character 8
				{ 1, 1, 1, 1, 0, 1, 1 }, // row 9 - character 9
				{ 0, 0, 0, 0, 0, 0, 0 }, // row 10 - blank
				{ 1, 1, 1, 1, 0, 0, 0 }, // row 11 - upper square
				{ 1, 0, 0, 0, 1, 1, 1 }, // row 12 - lower square
				{ 1, 1, 1, 1, 1, 0, 1 }, // row 13 - character A
				{ 1, 0, 0, 1, 1, 1, 1 }, // row 14 - character b
				{ 1, 0, 0, 0, 1, 1, 0 }, // row 15 - character c
				{ 0, 0, 1, 1, 1, 1, 0 }, // row 16 - character C
				{ 1, 1, 0, 0, 1, 1, 1 }, // row 17 - character d
				{ 1, 0, 1, 1, 1, 1, 0 } // row 18 - character E
		};

SoftwareSerial RS485Serial(SSerialRX, SSerialTX);

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

void setup() {
	FastLED.addLeds<NEOPIXEL, LED_CONTROL_PIN>(leds, NUM_LEDS, 0);
	// Start the built-in serial port, probably to Serial Monitor
	Serial.begin(9600);
	Serial.println("Start LED display");

	pinMode(SSerialTxControl, OUTPUT);

	digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver

	// Start the software serial port, to another device
	RS485Serial.begin(4800);   // set the data rate

	// Set white color
	_color.hue = 0;
	_color.saturation = 0;
	_color.intensity = 255;

	_shootTimer = 120;
	_waitTimer = 0;
	_isShooting = false;

	// Startup light show
	for (int i = 0; i < 2 * NUM_LEDS; i++) {
		int hue = (int) ((1.0 * i * 255) / NUM_LEDS);
		if (i < NUM_LEDS) {
			leds[i] = CHSV(hue, 255, 255);
		}
		for (int j = 0; j < i; j++) {
			int intensity = 255 - (i - j) * 8;
			if (intensity < 0) {
				intensity = 0;
			}
			if (j < NUM_LEDS) {
				leds[j] = CHSV(hue, 255, intensity);
			}
		}
		FastLED.delay(50);
		FastLED.show();
	}

	showThreeDigitNumber(0);
	updateColor();
	updateDisplay();
}

int testCounter = 0;

void loop() {

	updateCommand(&_command);

	if (_command.state == NEW_RECEIVED) {
		bool needDisplayUpdate = false;
		needDisplayUpdate = updateIsShooting() || needDisplayUpdate;
		needDisplayUpdate = updateShootTimer() || needDisplayUpdate;
		needDisplayUpdate = updateWaitTimer() || needDisplayUpdate;

		if (needDisplayUpdate) {
			updateColor();
			updateDisplay();
		}

		resetCommand(&_command);
	}
}

void updateCommand(struct Command *cP) {

	if (millis() - cP->startReceivingTime > 400) {
		resetCommand(cP);
	}

	if (RS485Serial.available() == 0) {
		return;
	}

	char byteReceived = (char) RS485Serial.read();
	//Serial.println(byteReceived);

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

void updateColor() {
	_color.saturation = 255;
	if (!_isShooting) {
		_color.hue = 0; // Red
	} else {
		if (_shootTimer > 30) {
			_color.hue = 255 * 120 / 360; // Green
		} else {
			_color.hue = 255 * 60 / 360; // Yellow
		}
	}
}

bool updateIsShooting() {
	bool isShootingNow = _isShooting;
	if (strlen(_command.receiveBuffer) == 2
			&& _command.receiveBuffer[0] == 'S') {
		_isShooting = atoi(_command.receiveBuffer + 1) == 1;
	}
	return _isShooting != isShootingNow;
}

bool updateShootTimer() {
	int shootTimerNow = _shootTimer;
	if (strlen(_command.receiveBuffer) > 0
			&& _command.receiveBuffer[0] == 'T') {
		_shootTimer = atoi(_command.receiveBuffer + 1);
	}
	return _shootTimer != shootTimerNow;
}

bool updateWaitTimer() {
	int waitTimerNow = _waitTimer;
	if (strlen(_command.receiveBuffer) > 0
			&& _command.receiveBuffer[0] == 'W') {
		_waitTimer = atoi(_command.receiveBuffer + 1);
	}
	return _waitTimer != waitTimerNow;
}

void updateDisplay() {
	if (_waitTimer > 0) {
		Serial.println("Show wait timer");
		showThreeDigitNumber(_waitTimer);
		Serial.println(_waitTimer);
	} else {
		Serial.println("Show shoot timer");
		showThreeDigitNumber(_shootTimer);
		Serial.println(_shootTimer);
	}
	Serial.println("update display");
	FastLED.show();
}

void showDigit(int position, int digit) {
	int ledOffset = (position - 1) * 7 * LEDS_PER_SEGMENT;
	int ledNum;
	for (int segment = 0; segment < NO_SEGMENTS; segment++) {
		char ledStatus = segmentCodes[digit][segment];
		for (int j = 0; j < LEDS_PER_SEGMENT; j++) {
			ledNum = ledOffset + segment * LEDS_PER_SEGMENT + j;
			leds[ledNum] = CHSV(_color.hue, _color.saturation,
					ledStatus * _color.intensity);
		}
	}
}

void clearDigit(int position) {
	int ledOffset = (position - 1) * 7 * LEDS_PER_SEGMENT;
	int ledNum;
	for (int segment = 0; segment < NO_SEGMENTS; segment++) {
		for (int j = 0; j < LEDS_PER_SEGMENT; j++) {
			ledNum = ledOffset + segment * LEDS_PER_SEGMENT + j;
			leds[ledNum] = CRGB(0, 0, 0);
		}
	}
}

void showThreeDigitNumber(int number) {
	Serial.println("showThreeDigitNumber - ");
	Serial.println(number);
	int rightDigit = number % 10;
	int middleDigit = (number / 10) % 10;
	int leftDigit = (number / 100) % 10;

	if (number > 99) {
		showDigit(LEFT, leftDigit);
	} else {
		clearDigit(LEFT);
	}

	if (number > 9) {
		showDigit(MIDDLE, middleDigit);
	} else {
		clearDigit(MIDDLE);
	}

	showDigit(RIGHT, rightDigit);
}
