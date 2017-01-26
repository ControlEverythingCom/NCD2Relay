// This #include statement was automatically added by the Particle IDE.
#include <SI7020-A20_CE.h>

// This #include statement was automatically added by the Particle IDE.
#include <blynk.h>

/* Includes ------------------------------------------------------------------*/
#include <NCD2Relay.h>

//Relay Controller Library object
NCD2Relay relayController;

//SI7020 temperature sensor Library object
SI7020_A20_CE tempSensor;

//Variables used for debouncing digital inputs on relay board when reading things like buttons/switches.
bool tripped[6];

int debugTrips[6];

int minTrips = 5;

//Variable for reading connected SI7020 temperatue sensor.
double tempF = 0;

//Authentication code for Blynk app running on phone.  Enter the Auth code for the project on your Blynk app here.
char blynkAuth[] = "e18c5ab081164750a12811b22db41600";

//LED widget objects on Blynk app user interface.
//Used for indicating status of inputs on relay board(Closed/Open)
WidgetLED led[6] = {V2, V3, V4, V5, V6, V7};

//Variables used for temperature publishing interval.  Only publish every 1000 milliseconds.
unsigned long tempPubInt = 1000;
unsigned long lastTempPub = 0;

/* This function is called once at start up ----------------------------------*/
void setup()
{
    //Configure Blynk
    Blynk.begin(blynkAuth);
    //Initialize relay controller by passing status of the 3 on board address jumpers.  This function must be called prior to interfacing with the controller.
	relayController.setAddress(0,0,0);
	
	//Publish status of inputs to app on boot
	updateInputs(true);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
    //Perform Housekeeping of Blynk connection
    Blynk.run();
    //Check Blynk publish interval for temperature reading so we only publish every 1 second.
    if(millis() > lastTempPub + tempPubInt){
        lastTempPub = millis();
        tempF = tempSensor.temperatureF();
        char temp[8];
        sprintf(temp, "%.2fº", tempF);
        Serial.println(temp);
        //Publish the current temperature reading to the Blynk app.
        Blynk.virtualWrite(V8, temp);
    }
    //Read inputs on relay board:
    updateInputs(false);
	
}

BLYNK_WRITE(V0){
    //Recieved command to control relay 1.
    int value = param.asInt();
    Serial.printf("Blynk virtual write pin %i with param %i \n", V0, value);
    switch(value){
        case 1:
            relayController.turnOnRelay(1);
            break;
        case 0:
            relayController.turnOffRelay(1);
            break;
    }
}
BLYNK_WRITE(V1){
    //Recieved command to control relay 2.
    int value = param.asInt();
    Serial.printf("Blynk virtual write pin %i with param %i \n", V1, value);
    switch(value){
        case 1:
            relayController.turnOnRelay(2);
            break;
        case 0:
            relayController.turnOffRelay(2);
            break;
    }
}

void updateInputs(bool startup){
    //Read and debounce digital inputs on relay board.
	int status = relayController.readAllInputs();
	int a = 0;
	for(int i = 1; i < 33; i*=2){
		if(status & i){
			debugTrips[a]++;
			if(debugTrips[a] >= minTrips || startup){
				if(!tripped[a] || startup){
				    //Input is closed
				    Serial.println("Input Closed");
					tripped[a] = true;
					//Publish high status to LED indicator in Blynk app notifying user that input is closed.
					led[a].on();
					if(a == 3){
					    //If this is input 4 on the relay board then push notification to user's phone.
					    Blynk.notify("You're really pushing my buttons");
					}
				}
			}
		}else{
			debugTrips[a] = 0;
			if(tripped[a] || startup){
			    Serial.println("Input Open");
			    //Input is open
				tripped[a] = false;
				//Publish low status to LED indicator in Blynk app notifying user that input is open.
				led[a].off();
			}
		}
		a++;
	}
}
