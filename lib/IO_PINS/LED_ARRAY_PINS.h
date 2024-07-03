//
// Created by user on 3/07/2024.
//

#ifndef LED_ARRAY_PINS_H
#define LED_ARRAY_PINS_H
#include <Arduino.h>
enum class LED_ARRAY_PINS:uint8_t{
     dataPin= 9, //  pin 9 is connected to the DataIn
     clkPin= 8, //  pin 8 is connected to the CLK
     csPin=  7, //  pin 7 is connected to LOAD
};
#endif //LED_ARRAY_PINS_H
