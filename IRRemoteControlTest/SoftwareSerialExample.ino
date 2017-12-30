#include <IRremote.h>

long const ZERO_0 = 0x1BC0157B;
long const ONE_1 = 0xC101E57B;
long const TWO_2 = 0x97483BFB;
long const THREE_3 = 0xF0C41643;
long const FOUR_4 = 0x9716BE3F;
long const FIVE_5 = 0x3D9AE3F7;
long const SIX_6 = 0x6182021B;
long const SEVEN_7 = 0x8C22657B;
long const EIGHT_8 = 0x488F3CBB;
long const NINE_9 = 0x449E79F;
long const LEFT_10 = 0x52A3D41F;
long const UP_11 = 0x511DBB;
long const RIGHT_12 = 0x20FE4DBB;
long const DOWN_13 = 0xA3C8EDDB;
long const OK_14 = 0xD7E84B1B;
long const STAR_15 = 0x32C6FDF7;
long const HASH_16 = 0x3EC3FC1B;

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


int keyesRemoteReceived(){
	int key = -1;
	if (irrecv.decode(&results)) {
		long val = results.value;

		switch(val){
			case ZERO_0:  key = 0; break;
			case ONE_1:   key = 1; break;
			case TWO_2:   key = 2; break;
			case THREE_3: key = 3; break;
			case FOUR_4:  key = 4; break;
			case FIVE_5:  key = 5; break;
			case SIX_6:   key = 6; break;
			case SEVEN_7: key = 7; break;
			case EIGHT_8: key = 8; break;
			case NINE_9:  key = 9; break;
			case LEFT_10: key = 10; break;
			case UP_11:   key = 11; break;
			case RIGHT_12:key = 12; break;
			case DOWN_13: key = 13; break;
			case OK_14:   key = 14; break;
			case STAR_15: key = 15; break;
			case HASH_16: key = 16; break;
		}
		irrecv.resume(); // Receive the next value
		Serial.println(val, HEX);
	}

	return key;
}
