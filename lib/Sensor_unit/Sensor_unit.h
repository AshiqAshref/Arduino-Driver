#ifndef SENSOR_UNIT_H
#define SENSOR_UNIT_H
#include <Arduino.h>

class Sensor_unit {
public:
    explicit Sensor_unit();
    static void initializeBoxSensor();
    static bool isOpen(byte boxNo);
    static bool check_if_any_box_open();
    static bool checkAllBoxStatus();
};

#endif //SENSOR_UNIT_H
