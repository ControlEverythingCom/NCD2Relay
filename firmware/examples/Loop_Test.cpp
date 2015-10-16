/* Includes ------------------------------------------------------------------*/
#include "NCD2Relay/NCD2Relay.h"

SYSTEM_MODE(AUTOMATIC);
NCD2Relay relayController;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	Serial.begin(115200);
	relayController.setAddress(0,0,0);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	//Relay test
	relayController.turnOnRelay(1);
	int rOnStatus = relayController.readRelayStatus(1);
	Serial.print("Status after On: ");
	Serial.println(rOnStatus);
	delay(500);
	relayController.turnOnRelay(2);
	delay(500);
	relayController.turnOffRelay(1);
	int rOffStatus = relayController.readRelayStatus(1);
	Serial.print("Status after Off: ");
	Serial.println(rOffStatus);
	delay(500);
	relayController.turnOffRelay(2);
	delay(500);
	relayController.turnOnAllRelays();
	delay(500);
	relayController.turnOffAllRelays();
	delay(500);
	relayController.toggleRelay(1);
	delay(500);
	relayController.toggleRelay(1);
	delay(500);
	relayController.toggleRelay(2);
	delay(500);
	relayController.toggleRelay(2);
	delay(500);

	//Input Test
	byte status = relayController.readAllInputs();
	if(status != 0){
		Serial.print("Status: ");
		Serial.println(status);
	}
	delay(50);
}
