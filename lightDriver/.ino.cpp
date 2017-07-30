#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-07-30 01:37:04

#include "Arduino.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "ReceivingCommand.h"
void setup() ;
void loop() ;
void sendMessages(struct Sequence *sequence) ;
void sendTimeCountdown(Sequence *sequenceP) ;
void sendWaitingCountdown(Sequence *sequenceP) ;
void sendNumberOfArrowsLeft(Sequence *sequenceP) ;
void sendABCDStatus() ;
void sendIsShootingStatus(Sequence *sequenceP) ;
void sendRunningStatus(Sequence *sequenceP) ;
void updateTimes(struct Sequence *sequenceP) ;
void updateCommand(struct Command *cP) ;
void resetCommand(struct Command *cP) ;
void updateSequence(struct Sequence *sequenceP) ;
void updateSound() ;
void doRecord(char* record) ;
void switchABCD() ;
void greenLightOn() ;
void yellowLightOn() ;
void redLightOn() ;
void lightsOff() ;
void endSequence() ;
void doCommand(struct Command * commandP, struct Sequence * sequenceP) ;
void startSequence(struct Command * commandP, struct Sequence * sequenceP) ;
void printSequence(struct Sequence * sequenceP) ;
long getRecordDuration(char *record) ;
void interruptSequenceRecord(struct Sequence * sequenceP) ;
char* getRecord(struct Sequence * sequenceP) ;
void setABCD(struct Command * commandP) ;
void setLight(struct Command * commandP) ;
void startSoundSignal(struct Command * commandP) ;
void startSoundSignal(int noSignals) ;
void doPause(struct Sequence * sequenceP) ;
void doContinue(struct Sequence * sequenceP) ;
void resetToDefault() ;


#include "LightDriver.ino"

#endif
