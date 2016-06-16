#include "NCD2Relay/I2CBZ.h"

/* Includes ------------------------------------------------------------------*/
#include "NCD2Relay/NCD2Relay.h"

NCD2Relay relayController;

SYSTEM_MODE(AUTOMATIC);

int triggerRelay(String command);

bool tripped[6];

int debugTrips[6];

int minTrips = 5;

bool flicker1 = false;

bool flicker2 = false;

int flickerTick1 = 0;

int flickerTick2 = 0;

int flickerCount = 10;

unsigned long flickerInterval = 2000;

unsigned long lastFlicker1 = 0;

unsigned long lastFlicker2 = 0;

I2CBZ buzzer;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	Particle.function("controlRelay", triggerRelay);
	Serial.begin(115200);
	relayController.setAddress(0,0,0);
    buzzer.initialize(0);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	//Flicker counter
	if(flicker1){
	    if(millis() > lastFlicker1 + flickerInterval){
	        if(flickerTick1 == flickerCount){
	            //Flickered 10 times so stop;
	            flickerTick1 = 0;
	            flicker1 = false;
	            relayController.turnOffRelay(1);
	        }else{
	            relayController.toggleRelay(1);
	            buzzer.buzzer(0);
	            lastFlicker1 = millis();
	            flickerTick1++;
	        }
	    }
	    
	}
	if(flicker2){
	    if(millis() > lastFlicker2 + flickerInterval){
	        if(flickerTick2 == flickerCount){
	            //Flickered 10 times so stop;
	            flickerTick2 = 0;
	            flicker2 = false;
	            relayController.turnOffRelay(2);
	        }else{
	            relayController.toggleRelay(2);
	            buzzer.buzzer(0);
	            lastFlicker2 = millis();
	            flickerTick2++;
	        }
	    }
	    
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
	if(relayCommand.equalsIgnoreCase("flicker")){
	    relayController.turnOnRelay(relayNumber);
	    buzzer.buzzer(1);
	    switch(relayNumber){
	        case 1:
	        flicker1 = true;
	        lastFlicker1 = millis();
	        break;
	        case 2:
	        flicker2 = true;
	        lastFlicker2 = millis();
	        break;
	    }
	    
	    return 1;
	}
	return 0;
}
