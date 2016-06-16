#include "NCD2Relay.h"
#include "spark_wiring_usbserial.h"

//Comment line below out to turn off Serial logging
//#define LOGGING

//Constructor
NCD2Relay::NCD2Relay(){
}

//Retry added
void NCD2Relay::setAddress(int a0, int a1, int a2){
    address = 0x20;
    if(a0 == 1){
        address = address | 1;
    }
    if(a1 == 1){
        address = address | 2;
    }
    if(a2 == 1){
        address = address | 4;
    }
    //Start I2C port
    Wire.begin();
    //Open connection to specified address
    retryAddress1:
    Wire.beginTransmission(address);
    //Set all channels to outputs
    Wire.write(0x00);
    Wire.write(0xFC);
    //Determine if device is present at that address
    byte status = Wire.endTransmission();

    Wire.beginTransmission(address);
    Wire.write(0x06);
    Wire.write(0xFC);
    status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry set address command");
    #endif
            retrys++;
            goto retryAddress1;
        }else{
    #ifdef LOGGING
            Serial.println("Set Address failed");
    #endif
            initialized = false;
            retrys = 0;
        }

    }else{
        // Serial.println("Command Successful");
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD2Relay::turnOnRelay(int relay){
    if(relay > 2 || relay < 0){
        return;
    }
    byte bankValue = bankOneStatus;
    byte registerAddress = 0x0A;
    switch(relay){
        case 1:
            bankValue = bankValue | 1;
            break;
        case 2:
            bankValue = bankValue | 2;
            break;
    }
    turnOnRelayRetry:
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    Wire.write(bankValue);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry Turn On Relay command");
    #endif
            retrys++;
            goto turnOnRelayRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Turn On Relay failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD2Relay::turnOffRelay(int relay){
    if(relay > 2 || relay < 0){
        return;
    }
    byte bankValue = bankOneStatus;
    byte registerAddress = 0x0A;
    switch(relay){
        case 1:
            bankValue = bankValue & ~1;
            break;
        case 2:
            bankValue = bankValue & ~2;
            break;
    }
turnOffRelaysRetry:
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    Wire.write(bankValue);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry Turn Off Relay command");
#endif
            retrys++;
            goto turnOffRelaysRetry;
        }else{
#ifdef LOGGING
            Serial.println("Turn Off Relay command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD2Relay::turnOnAllRelays(){
turnOnAllRelays1:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    Wire.write(0x03);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry turn off all relays banks 1-2 command");
    #endif
            retrys++;
            goto turnOnAllRelays1;
        }else{
    #ifdef LOGGING
            Serial.println("Turn off all relays banks 1-2 command failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD2Relay::turnOffAllRelays(){
turnOffAllRelaysRetry1:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    Wire.write(0);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry turn off all relays banks 1-2 command");
    #endif
            retrys++;
            goto turnOffAllRelaysRetry1;
        }else{
    #ifdef LOGGING
            Serial.println("Turn off all relays banks 1-2 command failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD2Relay::toggleRelay(int relay){
    if(relay > 2 || relay < 0){
        return;
    }
    byte bankValue =  bankOneStatus;
    switch(relay){
        case 1:
            bankValue = bankValue ^ 1;
            break;
        case 2:
            bankValue = bankValue ^ 2;
            break;
    }
toggleRelayRetry:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    Wire.write(bankValue);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry toggle relay command");
#endif
            retrys++;
            goto toggleRelayRetry;
        }else{
#ifdef LOGGING
            Serial.println("Toggle relay command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD2Relay::setBankStatus(int status){
    setBankStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    Wire.write(status);
    byte s = Wire.endTransmission();
    if(s != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry set bank status command");
    #endif
            retrys++;
            goto setBankStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Set bank status command failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

int NCD2Relay::readRelayStatus(int relay){
    if(relay > 2 || relay < 0){
        return 256;
    }
    int value;
    switch(relay){
        case 1:
            value = 1;
            break;
        case 2:
            value = 2;
            break;
    }
    getRelayStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read relay status command");
    #endif
            retrys++;
            goto getRelayStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read relay status command failed");
    #endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(address, 1);
    byte bankStatus = Wire.read();
    if(bankStatus & value){
        return 1;
    }else{
        return 0;
    }
}

int NCD2Relay::readRelayBankStatus(){
    readBankStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read bank status command");
    #endif
            retrys++;
            goto readBankStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read bank status command failed");
    #endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(address, 1);
    byte bankStatus = Wire.read();
    return bankStatus;
}

void NCD2Relay::readStatus(){
readBankOneRetry:
    //Open Connection to controller
    Wire.beginTransmission(address);
    //Get current status of relays 1-8
    Wire.write(0x0A);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read status banks 1-2 command");
    #endif
            retrys++;
            goto readBankOneRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read ststus banks 1-2 command failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        retrys = 0;
        initialized = true;
        Wire.requestFrom(address, 1);
        bankOneStatus = Wire.read();
    }

}

int NCD2Relay::readInputStatus(int input){
    if(input > 6 || input < 1){
        return 256;
    }
    int value;
    switch(input){
        case 1:
            value = 4;
            break;
        case 2:
            value = 8;
            break;
        case 3:
            value = 16;
            break;
        case 4:
            value = 32;
            break;
        case 5:
        	value = 64;
        	break;
        case 6:
        	value = 128;
        	break;
    }
    getGPIOStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x09);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read input status command");
    #endif
            retrys++;
            goto getGPIOStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read input status command failed");
    #endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(address, 1);
    byte bankStatus = Wire.read();
    if(bankStatus & value){
        return 0;
    }else{
        return 1;
    }
}

int NCD2Relay::readAllInputs(){
    readInputStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x09);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read bank status command");
    #endif
            retrys++;
            goto readInputStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read bank status command failed");
    #endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(address, 1);
    byte bankStatus = Wire.read();
    byte inverted = ~bankStatus;
    byte shifted = inverted >> 2;
    return shifted;
}
