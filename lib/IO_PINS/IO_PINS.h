#ifndef IO_PINS_H
#define IO_PINS_H
#include <Arduino.h>

enum STEPPER_PIN:uint8_t {
    STEPPER_PIN_XENABLE=32,
    STEPPER_PIN_XLIMIT_SW=42,
    STEPPER_PIN_XSTEP=28,
    STEPPER_PIN_XDIR=29,

    STEPPER_PIN_YENABLE=31,
    STEPPER_PIN_YLIMIT_SW=44,
    STEPPER_PIN_YSTEP=26,
    STEPPER_PIN_YDIR=27,

    STEPPER_PIN_ZENABLE=30,
    STEPPER_PIN_ZLIMIT_SW=43,
    STEPPER_PIN_ZSTEP=24,
    STEPPER_PIN_ZDIR=25
};

enum SENSOR_PIN:uint8_t {
    SENSOR_PIN_X=40,
    SENSOR_PIN_XA=34,
    SENSOR_PIN_XB=35,
    SENSOR_PIN_XC=36,

    SENSOR_PIN_Y=41,
    SENSOR_PIN_YA=37,
    SENSOR_PIN_YB=38,
    SENSOR_PIN_YC=39
};

enum OUTPUT_PIN{
    OUTPUT_PIN_BEEPER = 10
};

enum BUTTON_PIN:uint8_t{
    BUTTON_ENTER=2,
    BUTTON_LEFT=3,
    BUTTON_RIGHT=5,
    BUTTON_UP=6,
    BUTTON_DOWN=4,
    BUTTON_FORWARD=46,
    BUTTON_BACKWARD=47,
    BUTTON_DELAY=0,
};

enum LED_MATRIX_PIN:uint8_t{
    LED_MATRIX_PIN_DATAIN= 9, //  pin 9 is connected to the DataIn
    LED_MATRIX_PIN_CLK= 8, //  pin 8 is connected to the CLK
    LED_MATRIX_PIN_CS=  7, //  pin 7 is connected to LOAD
};


#endif //IO_PINS_H