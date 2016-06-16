
#include "I2CBZ.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_constants.h"

bool I2CBZ::initialize(int directionReg){
    Wire.begin();
    Wire.beginTransmission(address);
    Wire.write(gpioConfigReg);
    Wire.write(directionReg);
    byte status = Wire.endTransmission();
    if(status != 0){
        Serial.println("initialize PCA9536 failed");
        return false;
    }else{
        //It worked
        Wire.beginTransmission(address);
        Wire.write(outputPortReg);
        Wire.write(0);
        byte commandStatus = Wire.endTransmission();
        if(commandStatus != 0){
            Serial.println("set output port all off");
            return false;
        }else{
            //It Worked
            return true;
        }
        
    }
}

void I2CBZ::buzzer(int command){
    byte portStatus;
    Wire.beginTransmission(address);
    Wire.write(outputPortReg);
    byte status = Wire.endTransmission();
    if(status != 0){
        Serial.println("read output port direction failed");
        return;
    }else{
        Wire.requestFrom(address, 1);
        portStatus = Wire.read();
    }
    //Turn on Buzzer
    if(command == 1){
        portStatus = portStatus | 8;
        Wire.beginTransmission(address);
        Wire.write(outputPortReg);
        Wire.write(portStatus);
        byte commandStatus = Wire.endTransmission();
        if(status != 0){
            Serial.println("set output port direction failed");
            return;
        }else{
            //It Worked
            return;
        }
    }
    //Turn off Buzzer
    if(command == 0){
        portStatus = portStatus & ~8;
        Wire.beginTransmission(address);
        Wire.write(outputPortReg);
        Wire.write(portStatus);
        byte commandStatus = Wire.endTransmission();
        if(status != 0){
            Serial.println("set output port direction failed");
            return;
        }else{
            //It Worked
            return;
        }
    }
}