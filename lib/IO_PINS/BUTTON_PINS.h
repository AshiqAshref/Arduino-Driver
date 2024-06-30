#ifndef BUTTON_PINS_H
#define BUTTON_PINS_H
#include <Arduino.h>

enum class BUTTON_PINS:uint8_t{
        enterButton=2,
        leftButton=3,
        rightButton=5,
        upButton=6,
        downButton=4,
        frontButton=46,
        backButton=47,
        buttonDelay=0,
};

#endif //BUTTON_PINS_H
