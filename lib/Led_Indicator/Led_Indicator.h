#ifndef LED_INDICATOR_UNIT_H
#define LED_INDICATOR_UNIT_H
#include <LedControl.h>
#include <Arduino.h>
class Led_Indicator {
private:
    LedControl *boxLed; //(DIN, CLK, LOAD, no_of_chained_devices)
    unsigned long blinkPrevioustime=0;
    bool blinkState=false;
public:
    Led_Indicator(const byte & dataPin, const byte & clkPin,
        const byte & csPin, byte numDevices);
    void setColor(byte boxNo, char color)const;
    void initializeLed()const;
    void ledTestFunction()const;
    void blink(byte boxNo,char color);
};
#endif //LED_INDICATOR_UNIT_H
