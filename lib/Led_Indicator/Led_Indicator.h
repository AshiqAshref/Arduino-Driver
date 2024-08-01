#ifndef LED_INDICATOR_UNIT_H
#define LED_INDICATOR_UNIT_H
#include <LedControl.h>
#include <Arduino.h>
#include <LED_ARRAY_PINS.h>

class Led_Indicator {
    LedControl boxLed = LedControl(
     static_cast<byte>(LED_ARRAY_PINS::dataPin),
     static_cast<byte>(LED_ARRAY_PINS::clkPin),
     static_cast<byte>(LED_ARRAY_PINS::csPin),
     2
   ); //(DIN, CLK, LOAD, no_of_chained_devices)
    unsigned long blinkPrevioustime=0;
    bool blinkState=false;
public:
    Led_Indicator();
    void setColor(byte boxNo, char color);
    void initializeLed();
    void ledTestFunction(unsigned int delay_);
    void blink(byte boxNo,char color);
};
#endif //LED_INDICATOR_UNIT_H
