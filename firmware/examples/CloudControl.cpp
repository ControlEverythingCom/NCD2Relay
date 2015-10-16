/* Includes ------------------------------------------------------------------*/
#include "NCD2Relay/NCD2Relay.h"

NCD2Relay relayController;

SYSTEM_MODE(AUTOMATIC);

int triggerRelay(String command);

bool tripped[6];

int debugTrips[6];

int minTrips = 5;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	Particle.function("controlRelay", triggerRelay);
	Serial.begin(115200);
	relayController.setAddress(0,0,0);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	int status = relayController.readAllInputs();
	int a = 0;
	for(int i = 1; i < 33; i*=2){
		if(status & i){
			debugTrips[a]++;
			if(debugTrips[a] >= minTrips){
				if(!tripped[a]){
					tripped[a] = true;
					//set input trip event to true
					String eventName = "Input_";
					eventName+=(a+1);
					Particle.publish(eventName, "ON");
					Serial.print("eventName: ");
					Serial.println(eventName);
					Serial.print("eventContents: ");
					Serial.println("ON");
				}
			}
		}else{
			debugTrips[a] = 0;
			if(tripped[a]){
				tripped[a] = false;
				//set input trip event to false
				String eventName = "Input_";
				eventName+=(a+1);
				Particle.publish(eventName, "OFF");
				Serial.print("eventName: ");
				Serial.println(eventName);
				Serial.print("eventContents: ");
				Serial.println("OFF");
			}
		}
		a++;
	}
}

int triggerRelay(String command){
	if(command.equalsIgnoreCase("turnonallrelays")){
		relayController.turnOnAllRelays();
		return 1;
	}
	if(command.equalsIgnoreCase("turnoffallrelays")){
		relayController.turnOffAllRelays();
		return 1;
	}
	if(command.startsWith("setBankStatus:")){
		int status = command.substring(14).toInt();
		if(status < 0 || status > 255){
			return 0;
		}
		Serial.print("Setting bank status to: ");
		Serial.println(status);
		relayController.setBankStatus(status);
		Serial.println("done");
		return 1;
	}
	//Relay Specific Command
	int relayNumber = command.substring(0,1).toInt();
	Serial.print("relayNumber: ");
	Serial.println(relayNumber);
	String relayCommand = command.substring(1);
	Serial.print("relayCommand:");
	Serial.print(relayCommand);
	Serial.println(".");
	if(relayCommand.equalsIgnoreCase("on")){
		Serial.println("Turning on relay");
		relayController.turnOnRelay(relayNumber);
		Serial.println("returning");
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("off")){
		relayController.turnOffRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("toggle")){
		relayController.toggleRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("momentary")){
		relayController.turnOnRelay(relayNumber);
		delay(300);
		relayController.turnOffRelay(relayNumber);
		return 1;
	}
	return 0;
}
