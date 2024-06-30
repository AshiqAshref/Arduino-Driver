#ifndef STEPPER_PINS_H
#define STEPPER_PINS_H
#include <Arduino.h>

enum class STEPPER_PINS:uint8_t {
    xEnable=32,
    xLimitSwitch=42,
    X_step_pin=28,
    X_dir_pin=29,

    yEnable=31,
    yLimitSwitch=44,
    Y_step_pin=26,
    Y_dir_pin=27,

    zEnable=30,
    zLimitSwitch=43,
    Z_step_pin=24,
    Z_dir_pin=25
};

#endif //STEPPER_PINS_H
