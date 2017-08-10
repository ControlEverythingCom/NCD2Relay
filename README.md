# About

This Library is intended for use with NCD 2 Relay Particle Core/Photon compatible relay controllers.

The intention of this library is to make use of the NCD 2 channel relay controller with Particle development web IDE as simple as possible for users.
###Developer information
NCD has been designing and manufacturing computer control products since 1995.  We have specialized in hardware design and manufacturing of Relay controllers for 20 years.  We pride ourselves as being the industry leader of computer control relay products.  Our products are proven reliable and we are very excited to support Particle.  For more information on NCD please visit www.controlanything.com

### Requirements
- NCD 2 Channel Particle Core/Photon Compatible Relay board
- Particle Core/Photon module
- Knowledge base for developing and programming with Particle Core/Photon modules.

### Version
1.0.0

### How to use this library

The libary must be imported into your application.  This can be done through the Particle WEB IDE by selecting Libraries, then select the NCD2Relay.  Click Include in App button.  Select the App you want to include the library in.  Finally click Add to this app.  For more information see [Particle's documentation] [sparkIncludeLibrary].

### Example use

Once the Library is included in your applicaiton you should see an include statement at the top like this:
```cpp
//This #include statement was automatically added by the Particle IDE.
#include "NCD2Relay/NCD2Relay.h"
```

Now you need to instanciate an object of the library for use in your application like this:
```cpp
NCD2Relay relayController;
```

Here is an example use case for the class:
```cpp
// This #include statement was automatically added by the Particle IDE.
#include "NCD2Relay/NCD2Relay.h"
NCD2Relay relayController;

void setup() {
    Serial.begin(115200);
    relayController.setInit(0,0,0);
}

void loop() {
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
```

### Public accessible methods
```cpp
void setInit(int a0, int a1, int a2, byte direction = 0xFC, byte pullup = 0xFC);
```
>Must be called first before using the object.  This method should also be called any time communication with
>the controller is lost or broken to recover communication  This method accepts three int arguments and optionally
>two byte arguments.  int a0, int a1 and int a2 tell the Library what address to direct commands to.  a0, a1 and a2 
>ints are representations of the three jumpers on the 2 channel relay controller which are labeled on the board A0, 
>A1, and A2.  If the jumper is installed then that int in this call should be set to 1.  If it is not installed then
>the int should be set to 0.
>
>So if I have A0, A1, and A2 installed I would call ```relayController.setInit(1, 1, 1).```
>
>The direction and pullup arguments are optional and default to Input on GP2 through GP7 (input 1 through 6) with
>pull-up resistors enabled. direction can be called without pullup and will enable all input pull-up resistors. 
>pullup cannot be called without first calling direciton. direction and pullup are set bitwise with bit 2 representing
>input/output 1 and bit 7 representing input/output 6.
>
>If I wanted to switch Input 6 to an output I would call ```setInit(0,0,0,0x7C)```
>
>If I wanted to turn off the pull-up resistor on Input 1 I would call ```setInit(0,0,0,0xFC,0xF8)```


```cpp
void turnOnRelay(int Relay);
```
>This method accepts one int argument.  Valid int arguments 1-2.  A call to this method will turn on the
>relay indicated by the passed int argument.


```cpp
void turnOffRelay(int Relay);
```
>This method accepts one int argument.  Valid int arguments 1-2.  A call to this method will turn off the relay
>indicated by the passed int argument.


```cpp
void turnOnAllRelays();
```
>This method does not accept any arguments.  A call to this method will turn on both relays on the
>controller.


```cpp
void turnOffAllRelays();
```
>This method does not accept any arguments.  A call to this method will turn off both relays on the
>controller.


```cpp
void toggleRelay(int relay);
```
>This method accepts one int argument.  Valid int arguments are 1-2.  A call to this method will toggle the
>status of the relay indicated by the passed int argument.  If the relay was previously off before the method
>call the relay will turn on and vice versa.


```cpp
void setBankStatus(int status);
```
>This method accepts two int arguments.  Valid status int arguments 0-2.  A call
>to this method will set the status of all relays on the board to the status byte passed in
the argument(status).  Each relay on the board(total of 2) are represented as bits in the status
>argument.


```cpp
int readRelayStatus(int relay);
```
>This method accepts one int argument and returns one int.  Valid relay int arguments 1-2.  A call to this
>method will read the status of the given relay passed by the relay argument and return the current on/off
>status of the given relay.  1 will be returned if the relay is on and 0 will be returned if the relay is off.
>256 will be returned if an error has occured(generally due to lack of communication with the controller).


```cpp
int readRelayBankStatus();
```
>This method accepts no arguments and returns one int.  A call to this
>method will read and return the status of all relays on the board.
>Each relay in the bank is represented as a bit in the returned int.  Valid returns are 0-3.  256 will be
>returned if an error has occured(generally due to lack of communciation with controller).


```cpp
int readInputStatus(int input);
```
>This method accepts one int argument and returns one int.  Valid input int arguments 1-6.  A call to this
>method will read the status of the given input passed by the input argumetn and return the current closed/open
>status of the given input.  1 will be returned if the input is closed and 0 will be returned if the input is open.
>256 will be returned if an error has occured(generally due to lack of communication with the controller).


```cpp
int readAllInputs();
```
>This method accepts no arguments and returns one byte.  A call to this
>method will read and return the status of all 6 inputs on the board.
>Each input on the board is represented as a bit in the returned byte.  Valid returns are 0-64.  If the input is closed
>the bit in the byte is set to 1, if the input is open the bit in the byte is set to 0.  256 will be
>returned if an error has occured(generally due to lack of communciation with controller).

```cpp
void setOutputHigh(int output);
```
>This method accepts one int. Valid int arguments 1-6. A call to this method will set the open drain output
>to float.

```cpp
void setOutputLow(int output);
```
>This method accepts one int. Valid arguments 1-6. A call to this method will set the open drain output to ground.

###Public accessible variables
```cpp
bool initialized;
```
>This boolean indicates the current status of the interface connection to the controller.  This variable should
>be checked often throughout your application.  If communication to the board is lost for any reason this
>boolean variable will return false.  If all is well it will return true.


License
----

GNU
[sparkIncludeLibrary]:https://docs.particle.io/guide/getting-started/build/photon/
