#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-04-08 16:50:53

#include "Arduino.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "ReceivingCommand.h"
void setup() ;
void loop() ;
void handleButtonPushes() ;
void handleSwitches() ;
void resetCommand(struct Command *cP) ;
void doCommand(struct Command * commandP) ;
void updateStatus(int s) ;
void performAction(int buttonPushed) ;
void changeABCD() ;
void decreaseNoArrows() ;
void setDefaultNumberOfArrows() ;
void sendResetSequence() ;
void sendGreenLightOn() ;
void sendYellowLightOn() ;
void sendRedLightOn() ;
void sendSoundSignal() ;
void sendStartSequence() ;
void sendPauseSequence() ;
void sendStopSequence() ;
void updateCommand(struct Command *cP, SoftwareSerial *rs485SerialP) ;


#include "ShootControlPanel.ino"

#endif
