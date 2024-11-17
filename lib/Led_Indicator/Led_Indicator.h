#ifndef LED_INDICATOR_UNIT_H
#define LED_INDICATOR_UNIT_H
#include <LedControl.h>
#include <Arduino.h>
#include <IO_PINS.h>

enum COLOR:char{
    COLOR_RED   = 'r',
    COLOR_GREEN = 'g',
    COLOR_BLUE  = 'b',
    COLOR_CLEAR = 'c',
};


class Led_Indicator {
    LedControl boxLed = LedControl(
     LED_MATRIX_PIN_DATAIN,
     LED_MATRIX_PIN_CLK,
     LED_MATRIX_PIN_CS,
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
