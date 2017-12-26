#include <Arduino.h>
#include <SoftwareSerial.h>
#include "ReceivingCommand.h"

#define RedPinOut       2
#define YellowPinOut    3
#define GreenPinOut     4
#define ABPinOut        5
#define CDPinOut		6
#define SoundPinOut     7

#define SSerialTxControl 9   //RS485 Direction control
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define TRANSMIT   			HIGH
#define RECEIVE     		LOW
#define MAX_MESSAGE_LENGTH 	32
#define MAX_COMMAND_LENGTH 	8
#define MAX_SEQUENCE_SIZE  	128
#define MAX_RECORDS        	32
#define RECORD_LENGTH      	7

#define ON 				 	LOW
#define OFF 			 	HIGH

#define PERIODIC_PULSE_INTERVAL 1000
SoftwareSerial _rs485Serial(SSerialRX, SSerialTX); // RX, TX

enum SequenceState {
	NONE, RUNNING, PAUSED, FINISHED
};

struct Sequence {
	enum SequenceState state = NONE;
	int recordCounter = 0;
	int lastHandledRecordCounter = -1;
	unsigned long startTime;
	unsigned long timeRunningSequence;
	unsigned long timeRunningReal;
	char currentSequence[MAX_SEQUENCE_SIZE];
	unsigned long scheduledRecordStartTimes[MAX_RECORDS];
	long timeShootingCountdown;
	int timeShootingCountdownSeconds; //
	bool timeShootingCountdownSecondsChangedPulse = false; //
	long timeWaitingCountdown;
	int timeWaitingCountdownSeconds; //
	bool timeWaitingCountdownSecondsChangedPulse = false; //
	bool isShooting = false; //
	bool isShootingChangedPulse = false; //
	bool sequenceIsRunningChangedPulse = false;
	int arrowsToShoot;
	int noArrowsLeft; //
	bool noArrowsChangedPulse = false; //
	bool changedABCDPulse = false; //
	bool shootingPaused = false; //
	bool isInWaitingCountdown = false;
};

struct Sound {
	int noSignals;
	unsigned int duration = 800;
	boolean sounding = false;
	unsigned long startTime = 0;
};

struct Command _command;
struct Sequence _sequence;
struct Sound _sound;

// Command definitions
// "S1 TTT": Start single sequence with green time TTT
// "S2 TTT": Start double sequence AB-CD/CD-AB with green time TTT
// "S3 TTT": Start double sequence AB-CD/AB-CD with green time TTT
// "A1": AB light on
// "A2": CD light on
// "LGREEN": Green light on only
// "LYELLO": Yellow light on only
// "LRED": Red light on only
// "ZN": N sound signals
// "I": Interrupt
// "R": Reset to default (red light)
// "P": Pause
// "C": Continue

// Operation types:
// 'A': Single sound signals
// 'B': Two sound signals
// 'C': Three sound signals
// 'G': Green light on, others off
// 'Y': Yellow light on, others off
// 'R': Red light on, others off
// 'Z': End
// 'E': Empty placeholder

char _singleRoundTemplate[9 * 7] = { //
		'E', ' ', ' ', '0', '\0', ' ', ' ', // Start
				'B', ' ', ' ', '0', '\0', ' ', ' ', // Two sound signals
				'W', ' ', '1', '0', '\0', ' ', ' ', // Red light for 10 seconds with countdown
				'A', ' ', ' ', '0', '\0', ' ', ' ', // One sound signal
				'G', 'T', 'T', 'T', '\0', 'I', ' ', // Green light for TTT seconds, interruptable
				'Y', ' ', '3', '0', '\0', 'I', 'S', // Yellow light for 30 seconds, interruptable, skipable
				'R', ' ', ' ', '0', '\0', ' ', ' ', // Red light
				'C', ' ', ' ', '0', '\0', ' ', ' ', // Three sound signals
				'Z', ' ', ' ', '0', '\0', ' ', ' '  // End
		};

char _doubleRoundTemplate[15 * 7] = { //
		'E', ' ', ' ', '0', '\0', ' ', ' ', // Start
				'B', ' ', ' ', '0', '\0', ' ', ' ', // Two sound signals
				'W', ' ', '1', '0', '\0', ' ', ' ', // Red light for 10 seconds with countdown
				'A', ' ', ' ', '0', '\0', ' ', ' ', // One sound signal
				'G', 'T', 'T', 'T', '\0', 'I', ' ', // Green light for TTT seconds, interruptable
				'Y', ' ', '3', '0', '\0', 'I', 'S', // Yellow light for 30 seconds, interruptable, skipable
				'B', ' ', ' ', '0', '\0', ' ', ' ', // Two sound signals
				'S', ' ', ' ', '0', '\0', ' ', ' ', // Switch AB-CD
				'W', ' ', '1', '0', '\0', ' ', ' ', // Red light for 10 seconds with countdown
				'A', ' ', ' ', '0', '\0', ' ', ' ', // One sound signal
				'G', 'T', 'T', 'T', '\0', 'I', ' ', // Green light for TTT seconds, interruptable
				'Y', ' ', '3', '0', '\0', 'I', 'S', // Yellow light for 30 seconds, interruptable, skipable
				'R', ' ', ' ', '0', '\0', ' ', ' ', // Red light
				'C', ' ', ' ', '0', '\0', ' ', ' ', // Three sound signals
				'Z', ' ', ' ', '0', '\0', ' ', ' ' // End
		};

bool _lightABIsOn = true;

unsigned long _dt;
unsigned long _tPrev;
unsigned long _t;
unsigned long _timeOfLastStatus;

void setup() {
	_command.state = EMPTY;
	_command.byteCounter = 0;
	for (int i = 0; i < MAX_COMMAND_LENGTH; i++) {
		_command.receiveBuffer[i] = '\0';
	}

	// Start the built-in serial port, probably to Serial Monitor
	Serial.begin(9600);

	pinMode(SSerialTxControl, OUTPUT);
	pinMode(RedPinOut, OUTPUT);
	pinMode(YellowPinOut, OUTPUT);
	pinMode(GreenPinOut, OUTPUT);
	pinMode(ABPinOut, OUTPUT);
	pinMode(CDPinOut, OUTPUT);
	pinMode(SoundPinOut, OUTPUT);

	lightsOff();
	digitalWrite(ABPinOut, _lightABIsOn ? ON : OFF);
	digitalWrite(CDPinOut, _lightABIsOn ? OFF : ON);
	_sequence.changedABCDPulse = true;

	digitalWrite(SoundPinOut, OFF);
	digitalWrite(SSerialTxControl, RECEIVE);  // Init Transceiver

	// Start the software serial port, to another device
	_rs485Serial.begin(4800);   // set the data rate

	_t = millis();
	_timeOfLastStatus = _t;
	redLightOn();
}

void loop() {
	_tPrev = _t;
	_t = millis();
	_dt = _t - _tPrev;

	updateTimes(&_sequence);
	updateSequence(&_sequence);
	sendMessages(&_sequence);

	updateCommand(&_command);
	if (_command.state == NEW_RECEIVED) {
		_command.state = EMPTY;
		Serial.println("Do command");
		Serial.println(_command.receiveBuffer);
		doCommand(&_command, &_sequence);
		resetCommand(&_command);
	}
}

void sendMessages(struct Sequence *sequence) {
	bool periodicStatusPulse = _t - _timeOfLastStatus > PERIODIC_PULSE_INTERVAL;
	if (periodicStatusPulse) {
		_timeOfLastStatus = _t;
	}

	// Waiting time count down message
	if (periodicStatusPulse
			|| sequence->timeWaitingCountdownSecondsChangedPulse) {
		sequence->timeWaitingCountdownSecondsChangedPulse = false;
		sendWaitingCountdown(sequence);
	}

	// Time count down message
	if (periodicStatusPulse
			|| sequence->timeShootingCountdownSecondsChangedPulse) {
		sequence->timeShootingCountdownSecondsChangedPulse = false;
		sendTimeCountdown(sequence);
	}

	// Arrows left message
	if (periodicStatusPulse || sequence->noArrowsChangedPulse) {
		sequence->noArrowsChangedPulse = false;
		sendNumberOfArrowsLeft(sequence);
	}

	// Changed AB/CD message
	if (sequence->changedABCDPulse) {
		sequence->changedABCDPulse = false;
		sendABCDStatus();
	}

	// Is shooting change message
	if (periodicStatusPulse || sequence->isShootingChangedPulse) {
		sequence->isShootingChangedPulse = false;
		sendIsShootingStatus(sequence);
	}

	// Sequence running change message
	if (periodicStatusPulse || sequence->sequenceIsRunningChangedPulse) {
		sequence->sequenceIsRunningChangedPulse = false;
		sendRunningStatus(sequence);
	}
}

void sendTimeCountdown(Sequence *sequenceP) {
	int t = sequenceP->timeShootingCountdownSeconds;
	t = t < 0 ? 0 : t;
	t = t > 240 ? 240 : t;
	char s[4];
	s[3] = '\0';
	sprintf(s, "%03d", sequenceP->timeShootingCountdownSeconds);
	digitalWrite(SSerialTxControl, TRANSMIT);
	_rs485Serial.write('T');
	_rs485Serial.write(s[0]);
	_rs485Serial.write(s[1]);
	_rs485Serial.write(s[2]);
	_rs485Serial.write((unsigned char) '\0');

	digitalWrite(SSerialTxControl, RECEIVE);
	Serial.print("Send T");
	Serial.print(s);
	Serial.println(" (time count down seconds) ");
}

void sendWaitingCountdown(Sequence *sequenceP) {
	int t = sequenceP->timeWaitingCountdownSeconds;
	t = t < 0 ? 0 : t;
	t = t > 30 ? 30 : t;
	char s[3];
	s[2] = '\0';
	sprintf(s, "%02d", sequenceP->timeWaitingCountdownSeconds);
	digitalWrite(SSerialTxControl, TRANSMIT);
	_rs485Serial.write('W');
	_rs485Serial.write(s[0]);
	_rs485Serial.write(s[1]);
	_rs485Serial.write((unsigned char) '\0');

	digitalWrite(SSerialTxControl, RECEIVE);
	Serial.print("Send W");
	Serial.print(s);
	Serial.println(" (waiting time count down seconds) ");
}

void sendNumberOfArrowsLeft(Sequence *sequenceP) {
	char s[2];
	s[1] = '\0';
	sprintf(s, "%d", sequenceP->noArrowsLeft);
	digitalWrite(SSerialTxControl, TRANSMIT);
	_rs485Serial.write('A');
	_rs485Serial.write(s[0]);
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RECEIVE);
	Serial.print("Send A");
	Serial.print(s[0]);
	Serial.println(" (number of arrows left)");
}

void sendABCDStatus() {
	digitalWrite(SSerialTxControl, TRANSMIT);
	_rs485Serial.write('a');
	if (_lightABIsOn) {
		Serial.println("Send a1 (AB) ");
		_rs485Serial.write('1');
	} else {
		Serial.println("Send a2 (CD) ");
		_rs485Serial.write('2');
	}
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RECEIVE);
}

void sendIsShootingStatus(Sequence *sequenceP) {
	digitalWrite(SSerialTxControl, TRANSMIT);
	_rs485Serial.write('S');
	if (sequenceP->isShooting) {
		_rs485Serial.write('1');
		Serial.println("Send S1 (is shooting)");
	} else {
		_rs485Serial.write('0');
		Serial.println("Send S0 (is not shooting)");
	}
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RECEIVE);
}

void sendRunningStatus(Sequence *sequenceP) {
	digitalWrite(SSerialTxControl, TRANSMIT);
	_rs485Serial.write('R');
	if (sequenceP->state == FINISHED || sequenceP->state == NONE) {
		Serial.println("Send R0 (sequence is not running)");
		_rs485Serial.write('0');
	} else if (sequenceP->state == RUNNING) {
		_rs485Serial.write('1');
		Serial.println("Send R1 (sequence running)");
	} else if (sequenceP->state == PAUSED) {
		_rs485Serial.write('P');
		Serial.println("Send RP (sequence paused)");
	}
	_rs485Serial.write((unsigned char) '\0');
	digitalWrite(SSerialTxControl, RECEIVE);
}

void updateTimes(struct Sequence *sequenceP) {
	if (sequenceP->state == RUNNING) {
		sequenceP->timeRunningSequence += _dt;
	} else if (sequenceP->state == RUNNING || sequenceP->state == PAUSED) {
		sequenceP->timeRunningReal += _dt;
	} else {
		sequenceP->timeRunningSequence = 0;
		sequenceP->timeRunningReal = 0;
	}
	if (sequenceP->isShooting) {
		sequenceP->timeShootingCountdown -= (long) _dt;
	}
	if (sequenceP->isInWaitingCountdown) {
		sequenceP->timeWaitingCountdown -= (long) _dt;
	}else{
		sequenceP->timeWaitingCountdown = 0;
	}
}

void updateCommand(struct Command *cP) {

	if (millis() - cP->startReceivingTime > 400) {
		resetCommand(cP);
	}

	if (_rs485Serial.available() == 0) {
		return;
	}

	char byteReceived = (char) _rs485Serial.read();
	++cP->byteCounter;
	if (cP->byteCounter == 1) {
		cP->startReceivingTime = millis();
	}

	boolean commandEnd = (byteReceived == '\0'
			|| cP->byteCounter == MAX_COMMAND_LENGTH);
	if (commandEnd) {
		if (byteReceived == '\0' && cP->byteCounter == 1) { // First and only byte is a zero
			resetCommand(cP);
		} else {
			cP->receiveBuffer[cP->byteCounter - 1] = '\0';
			cP->state = NEW_RECEIVED;
		}
	} else {
		cP->receiveBuffer[cP->byteCounter - 1] = byteReceived;
		cP->state = RECEIVING;
	}
}

void resetCommand(struct Command *cP) {
	cP->byteCounter = 0;
	cP->state = EMPTY;
}

void updateSequence(struct Sequence *sequenceP) {
	char *record = getRecord(sequenceP);
	if (record[0] == 'Z') {
		sequenceP->state = FINISHED;
	}

	if (sequenceP->state == RUNNING) {
		if (sequenceP->lastHandledRecordCounter != sequenceP->recordCounter) {
			doRecord(getRecord(sequenceP));
			sequenceP->lastHandledRecordCounter = sequenceP->recordCounter;
		}

		// Step to next record if time has reached scheduled time for record start
		if (sequenceP->timeRunningSequence
				>= sequenceP->scheduledRecordStartTimes[sequenceP->recordCounter
						+ 1]) {
			++sequenceP->recordCounter;
			Serial.print("Step sequence record ");
			Serial.println(sequenceP->recordCounter);
		}

	}
	updateSound();

	// Count down update
	int tCountdownSec = sequenceP->timeShootingCountdown / 1000 + 1;
	int maxTime = sequenceP->arrowsToShoot * 40;
	tCountdownSec = tCountdownSec > maxTime ? maxTime : tCountdownSec;
	tCountdownSec = sequenceP->timeShootingCountdown <= 0 ? 0 : tCountdownSec;
	sequenceP->timeShootingCountdownSecondsChangedPulse = tCountdownSec
			!= sequenceP->timeShootingCountdownSeconds;
	sequenceP->timeShootingCountdownSeconds = tCountdownSec;

	// Waiting count down update
	int tWaitCountdownSec = sequenceP->timeWaitingCountdown / 1000 + 1;
	int maxWaitTime = 10;
	tWaitCountdownSec =
			tWaitCountdownSec > maxWaitTime ? maxWaitTime : tWaitCountdownSec;
	tWaitCountdownSec =
			sequenceP->timeWaitingCountdown <= 0 ? 0 : tWaitCountdownSec;
	sequenceP->timeWaitingCountdownSecondsChangedPulse = tWaitCountdownSec
			!= sequenceP->timeWaitingCountdownSeconds;
	sequenceP->timeWaitingCountdownSeconds = tWaitCountdownSec;

	// Number of arrows left update
	int noArrowsLeft = (sequenceP->timeShootingCountdownSeconds - 1) / 40 + 1;
	noArrowsLeft =
			sequenceP->timeShootingCountdownSeconds == 0 ? 0 : noArrowsLeft;
	noArrowsLeft =
			noArrowsLeft > sequenceP->arrowsToShoot ?
					sequenceP->arrowsToShoot : noArrowsLeft;
	sequenceP->noArrowsChangedPulse = sequenceP->noArrowsLeft != noArrowsLeft;
	sequenceP->noArrowsLeft = noArrowsLeft;
}

void updateSound() {
	_sound.sounding = _sound.sounding
			&& _t - _sound.startTime
					< _sound.duration * (2 * _sound.noSignals - 1);
	boolean soundNow = _sound.sounding
			&& ((_t - _sound.startTime) / _sound.duration) % 2 == 0;
	digitalWrite(SoundPinOut, soundNow ? ON : OFF);
}

void doRecord(char* record) {
	char task = record[0];
	Serial.print("** Do sequence record at time (s): ");
	Serial.print(_sequence.timeRunningSequence / 1000L);
	Serial.print("  ");
	Serial.print(_sequence.recordCounter);
	Serial.print("  ");
	Serial.println(task);
	switch (task) {
	case 'A':
		startSoundSignal(1);
		break;
	case 'B':
		startSoundSignal(2);
		break;
	case 'C':
		startSoundSignal(3);
		break;
	case 'G':
		_sequence.isShootingChangedPulse = !_sequence.isShooting;
		_sequence.isShooting = true;
		_sequence.isInWaitingCountdown = false;
		greenLightOn();
		break;
	case 'Y':
		yellowLightOn();
		break;
	case 'R':
		_sequence.isShootingChangedPulse = _sequence.isShooting;
		_sequence.isShooting = false;
		_sequence.timeShootingCountdown = _sequence.arrowsToShoot * 40e3;
		redLightOn();
		break;
	case 'W':
		_sequence.isShootingChangedPulse = _sequence.isShooting;
		_sequence.isShooting = false;
		_sequence.timeShootingCountdown = _sequence.arrowsToShoot * 40e3;
		_sequence.timeWaitingCountdown = getRecordDuration(record) * 1000;
		_sequence.isInWaitingCountdown = true;
		break;
	case 'S':
		switchABCD();
		break;
	case 'E':
		// nothing
		break;
	case 'Z':
		endSequence();
	}
}

void switchABCD() {
	_lightABIsOn = !_lightABIsOn;
	digitalWrite(ABPinOut, _lightABIsOn ? ON : OFF);
	digitalWrite(CDPinOut, _lightABIsOn ? OFF : ON);
	_sequence.changedABCDPulse = true;
}

void greenLightOn() {
	lightsOff();
	digitalWrite(GreenPinOut, ON);
	Serial.println("Green light on");
}

void yellowLightOn() {
	lightsOff();
	digitalWrite(YellowPinOut, ON);
	Serial.println("Yellow light on");
}

void redLightOn() {
	lightsOff();
	digitalWrite(RedPinOut, ON);
	Serial.println("Red light on");
}

void lightsOff() {
	digitalWrite(RedPinOut, OFF);
	digitalWrite(YellowPinOut, OFF);
	digitalWrite(GreenPinOut, OFF);
}

void endSequence() {
	Serial.println("End sequence");
	resetToDefault();
}

void doCommand(struct Command * commandP, struct Sequence * sequenceP) {
	char cType = commandP->receiveBuffer[0];
	switch (cType) {
	case 'S':
		startSequence(commandP, sequenceP);
		break;
	case 'A':
		setABCD(commandP);
		break;
	case 'L':
		setLight(commandP);
		break;
	case 'Z':
		startSoundSignal(commandP);
		break;
	case 'I':
		interruptSequenceRecord(sequenceP);
		break;
	case 'R':
		resetToDefault();
		break;
	case 'P':
		doPause(sequenceP);
	}
}

void startSequence(struct Command * commandP, struct Sequence * sequenceP) {
	if (sequenceP->state == PAUSED) {
		doContinue(sequenceP);
		return;
	}

	Serial.println("startSequence");
	char sequenceType = commandP->receiveBuffer[1];
	int greenDuration = atoi(commandP->receiveBuffer + 3);
	Serial.print("Green duration (s): ");
	Serial.println(greenDuration);

	sequenceP->arrowsToShoot = (int) ((greenDuration + 30.0) / 40.0 + 0.5);
	sequenceP->noArrowsChangedPulse = true;
	sequenceP->timeShootingCountdown = _sequence.arrowsToShoot * 40e3;
	sequenceP->isShooting = false;

	Serial.print("Arrows to shoot: ");
	Serial.println(sequenceP->arrowsToShoot);

	// "S1 TTT": Start single sequence with green time TTT
	// "S2 TTT": Start double sequence AB-CD/CD-AB with green time TTT
	// "S3 TTT": Start double sequence AB-CD/AB-CD with green time TTT

	char *sequenceTemplatep;
	if (sequenceType == '1') {
		sequenceTemplatep = _singleRoundTemplate;
	} else {
		sequenceTemplatep = _doubleRoundTemplate;
	}

	boolean atEnd = false;
	int r = 0;
	long scheduledTAtRecordStart = 0;
	while (!atEnd) {
		int k0 = r * RECORD_LENGTH;
		int k;

		sequenceP->scheduledRecordStartTimes[r] = scheduledTAtRecordStart;
		for (int j = 0; j < RECORD_LENGTH; j++) {
			k = k0 + j;
			sequenceP->currentSequence[k] = sequenceTemplatep[k];
		}
		if (sequenceP->currentSequence[k0] == 'G') {
			char s[4];
			s[3] = '\0';
			sprintf(s, "%03d", greenDuration);
			sequenceP->currentSequence[k0 + 1] = s[0];
			sequenceP->currentSequence[k0 + 2] = s[1];
			sequenceP->currentSequence[k0 + 3] = s[2];
		}

		if (sequenceType == '3' && sequenceP->currentSequence[k0] == 'S') { // No switch AB/CD
			sequenceP->currentSequence[k0] = 'E'; // Empty
		}

		atEnd = sequenceP->currentSequence[k0] == 'Z';
		sequenceP->recordCounter = r;
		//long recordDuration = getRecordDuration(getRecord(sequenceP));
		long t = atoi(&(sequenceP->currentSequence[k0 + 1]));
		scheduledTAtRecordStart += t * 1000;
		++r;
	}

	sequenceP->state = RUNNING;
	sequenceP->recordCounter = 0;
	sequenceP->startTime = millis();
	sequenceP->timeRunningSequence = 0;
	sequenceP->timeRunningReal = 0;
	sequenceP->sequenceIsRunningChangedPulse = true;

	printSequence(sequenceP);
}

void printSequence(struct Sequence * sequenceP) {
	Serial.println("The sequence:");
	boolean atEnd = false;
	int r = 0;
	char *c = sequenceP->currentSequence;
	while (!atEnd) {
		int k0 = r * RECORD_LENGTH;
		for (int i = 0; i < RECORD_LENGTH; i++) {
			char s = c[k0 + i];
			if (s == 0) {
				s = ' ';
			}
			Serial.print(s);
		}
		Serial.print(" - starts at ");
		long t = (sequenceP->scheduledRecordStartTimes[r]) / 1000;
		Serial.print(t);
		Serial.println("s");

		atEnd = c[k0] == 'Z';
		++r;
	}
}

long getRecordDuration(char *record) {
	return atoi(record + 1);
}

void interruptSequenceRecord(struct Sequence * sequenceP) {
	char *record = getRecord(sequenceP);
	if (record[5] == 'I' && record[0] != 'Z') { // Record is interruptable and not the last
		do {
			++sequenceP->recordCounter;
			record = getRecord(sequenceP);
		} while (record[6] == 'S'); // Record is skipable
	}

	sequenceP->timeRunningSequence =
			sequenceP->scheduledRecordStartTimes[sequenceP->recordCounter];
}

char* getRecord(struct Sequence * sequenceP) {
	return &(sequenceP->currentSequence[sequenceP->recordCounter * RECORD_LENGTH]);
}

void setABCD(struct Command * commandP) {
	Serial.println("setABCD");
	_lightABIsOn = commandP->receiveBuffer[1] == '1';
	digitalWrite(ABPinOut, _lightABIsOn ? ON : OFF);
	digitalWrite(CDPinOut, _lightABIsOn ? OFF : ON);
}

void setLight(struct Command * commandP) {
	char light = commandP->receiveBuffer[1];
	switch (light) {
	case 'R':
		redLightOn();
		break;
	case 'Y':
		yellowLightOn();
		break;
	case 'G':
		greenLightOn();
		break;
	}
}

void startSoundSignal(struct Command * commandP) {
	int noSignals = atoi(&(commandP->receiveBuffer[1]));
	startSoundSignal(noSignals);
}

void startSoundSignal(int noSignals) {
	Serial.print("Start sound signal ");
	Serial.println(noSignals);
	_sound.sounding = true;
	_sound.noSignals = noSignals;
	_sound.startTime = _t;
}

void doPause(struct Sequence * sequenceP) {
	if (sequenceP->state == RUNNING) {
		sequenceP->state = PAUSED;
		sequenceP->shootingPaused = sequenceP->isShooting;
		if (sequenceP->shootingPaused) {
			sequenceP->isShooting = false;
			sequenceP->isShootingChangedPulse = true;
		}
	}
}

void doContinue(struct Sequence * sequenceP) {
	if (sequenceP->state == PAUSED) {
		sequenceP->state = RUNNING;
		sequenceP->isShooting = sequenceP->shootingPaused;
		sequenceP->isShootingChangedPulse = sequenceP->isShooting;
		sequenceP->shootingPaused = false;
	}
}

void resetToDefault() {
	redLightOn();
	_sequence.isShooting = false;
	_sequence.isShootingChangedPulse = true;
	_sequence.state = NONE;
	_sequence.sequenceIsRunningChangedPulse = true;
	_sequence.noArrowsLeft = 0;
	_sequence.noArrowsChangedPulse = true;
	_sequence.timeShootingCountdown = 0;
	_sequence.timeShootingCountdownSeconds = 0;
	_sequence.timeShootingCountdownSecondsChangedPulse = true;
	_sequence.timeWaitingCountdown = 0;
	_sequence.timeWaitingCountdownSeconds = 0;
	_sequence.timeWaitingCountdownSecondsChangedPulse = true;
	_sequence.isInWaitingCountdown = false;
	_sequence.shootingPaused = false;
}
