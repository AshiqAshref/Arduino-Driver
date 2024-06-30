#ifndef SENSOR_UNIT_H
#define SENSOR_UNIT_H
#include <Arduino.h>
#include <SENSOR_PINS.h>



class Sensor_unit {
private:
    SENSOR_PINS sensor_pins;
public:
    explicit Sensor_unit(SENSOR_PINS sensor_pins_);
    static void initializeBoxSensor();
    static bool isOpen(byte boxNo);
    static bool checkAllBoxStatus();
};

#endif //SENSOR_UNIT_H
