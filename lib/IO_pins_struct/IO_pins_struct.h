//
// Created by user on 28/06/2024.
//

#ifndef IO_PINS_STRUCT_H
#define IO_PINS_STRUCT_H
#include <Arduino.h>

struct IO_pins_struct{

    struct Button_pins{
        constexpr byte enterButton=2;
        constexpr byte leftButton=3;
        constexpr byte downButton=4;
        constexpr byte rightButton=5;
        constexpr byte upButton=6;
        constexpr byte frontButton=46;
        constexpr byte backButton=47;
    };
    struct Stepper_Pins{
        constexpr byte xEnable=32;
        constexpr byte xLimitSwitch=42;
        constexpr byte X_step_pin=28;
        constexpr byte X_dir_pin=29;

        constexpr byte yEnable=31;
        constexpr byte yLimitSwitch=44;
        constexpr byte Y_step_pin=26;
        constexpr byte Y_dir_pin=27;

        constexpr byte zEnable=30;
        constexpr byte zLimitSwitch=43;
        constexpr byte Z_step_pin=24;
        constexpr byte Z_dir_pin=25;
    };
    Stepper_Pins stepper_pins;
    Button_pins button_pins;
};
#endif //IO_PINS_STRUCT_H
