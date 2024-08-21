#ifndef LED_INDICATOR_UNIT_H
#define LED_INDICATOR_UNIT_H
#include <LedControl.h>
#include <Arduino.h>
#include <LED_ARRAY_PINS.h>
#include <COLOR.h>

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
    void initializeLed();
    void ledTestFunction(unsigned int delay_);
    void blink(byte boxNo, COLOR color);
    void setColor(byte boxNo, const COLOR *color);
    void setColor(byte boxNo, COLOR color);
};
#endif //LED_INDICATOR_UNIT_H
