#include <Main.h>
#include "sensor_unit.h"

Sensor_unit::Sensor_unit(const SENSOR_PINS sensor_pins_) {
    initializeBoxSensor();
    sensor_pins=sensor_pins_;
}

void Sensor_unit::initializeBoxSensor(){
    pinMode(static_cast<uint8_t>(SENSOR_PINS::X), INPUT);
    pinMode(static_cast<uint8_t>(SENSOR_PINS::Xa), OUTPUT);
    pinMode(static_cast<uint8_t>(SENSOR_PINS::Xb), OUTPUT);
    pinMode(static_cast<uint8_t>(SENSOR_PINS::Xc), OUTPUT);

    pinMode(static_cast<uint8_t>(SENSOR_PINS::Y), INPUT);
    pinMode(static_cast<uint8_t>(SENSOR_PINS::Ya), OUTPUT);
    pinMode(static_cast<uint8_t>(SENSOR_PINS::Yb), OUTPUT);
    pinMode(static_cast<uint8_t>(SENSOR_PINS::Yc), OUTPUT);
}

bool Sensor_unit::isOpen(byte const boxNo){
    if(boxNo<=8){
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Ya), (HIGH &&((boxNo-1) & 0b000000001)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Yb), (HIGH &&((boxNo-1) & 0b000000010)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Yc), (HIGH &&((boxNo-1) & 0b000000100)));
        delay(70);
        if(digitalRead(static_cast<uint8_t>(SENSOR_PINS::Y)))
            return true;
    }else{
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xa), (HIGH &&((boxNo-9) & 0b000000001)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xb), (HIGH &&((boxNo-9) & 0b000000010)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xc), (HIGH &&((boxNo-9) & 0b000000100)));
        delay(70);
        if(digitalRead(static_cast<uint8_t>(SENSOR_PINS::X)))
            return true;
    }
    return false;
}


bool Sensor_unit::checkAllBoxStatus(){
    bool isOpen=false;
    for(int & openBoxe : openBoxes){
        openBoxe=0;
    }
    for(byte i=0;i<8;i++){
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Ya), (HIGH &&(i & 0b000000001)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Yb), (HIGH &&(i & 0b000000010)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Yc), (HIGH &&(i & 0b000000100)));

        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xa), (HIGH &&(i & 0b000000001)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xb), (HIGH &&(i & 0b000000010)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xc), (HIGH &&(i & 0b000000100)));
        delay(70);

        if(digitalRead(static_cast<uint8_t>(SENSOR_PINS::Y))){
            isOpen=true;
            Serial.println("BOX "+String(i)+"\n\n");
            for(int & openBoxe : openBoxes){
                if(openBoxe==0){
                    openBoxe=i+1;
                    break;
                }
            }
        }
        if(digitalRead(static_cast<uint8_t>(SENSOR_PINS::X))){
            isOpen=true;
            Serial.println("BOX "+String(i+9)+"\n\n");
            for(int & openBoxe : openBoxes){
                if(openBoxe==0){
                    openBoxe=i+9;
                    break;
                }
            }
        }
    }
    return isOpen;
}
