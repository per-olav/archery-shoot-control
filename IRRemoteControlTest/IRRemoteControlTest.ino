#include <IRremote.h>
#include "irremoteheader.h"

int const RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
	Serial.begin(9600);
	Serial.println("Enabling IRin");
	irrecv.enableIRIn(); // Start the receiver
	Serial.println("Enabled IRin");
}

void loop() {
	int key = keyesRemoteReceived();
	if (key >= 0){
		Serial.println(key);
	}
	delay(100);
}


