class I2CBZ{
public:
    bool initialize(int directionReg);
    void buzzer(int command);
    
private:
    int address = 0x41;
    int inputPortReg = 0x00;
    int outputPortReg = 0x01;
    int polarityInversionReg = 0x02;
    int gpioConfigReg = 0x03;
};