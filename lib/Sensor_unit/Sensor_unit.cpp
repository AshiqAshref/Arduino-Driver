#include "sensor_unit.h"
#include <Box.h>
#include <SENSOR_PINS.h>


Sensor_unit::Sensor_unit() {
    initializeBoxSensor();
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



extern Box boxes[];
bool Sensor_unit::check_if_any_box_open() {
    boolean open=false;
    for(int i=1;i<=16;i++) {
        if(isOpen(i)) {
            Serial.print("open: ");
            Serial.println(i);
            boxes[i-1].isOpen(true);
            open = true;
        }else{
            boxes[i-1].isOpen(false);
        }
    }
    return open;
}


bool Sensor_unit::isOpen(byte boxNo){
    if(boxNo<1||boxNo>16) return false;
    boxNo= boxNo-1;
    if(boxNo<8){
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xa), (HIGH &&(boxNo & 0b000000001)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xb), (HIGH &&(boxNo & 0b000000010)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xc), (HIGH &&(boxNo & 0b000000100)));
        if(digitalRead(static_cast<uint8_t>(SENSOR_PINS::X)))
            return true;
    }else{
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Ya), (HIGH &&(boxNo & 0b000000001)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Yb), (HIGH &&(boxNo & 0b000000010)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Yc), (HIGH &&(boxNo & 0b000000100)));
        if(digitalRead(static_cast<uint8_t>(SENSOR_PINS::Y)))
            return true;
    }
    return false;
}

bool Sensor_unit::checkAllBoxStatus(){
    bool isOpen=false;
    for(byte i=0;i<8;i++){
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Ya), (HIGH &&(i & 0b000000001)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Yb), (HIGH &&(i & 0b000000010)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Yc), (HIGH &&(i & 0b000000100)));

        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xa), (HIGH &&(i & 0b000000001)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xb), (HIGH &&(i & 0b000000010)));
        digitalWrite(static_cast<uint8_t>(SENSOR_PINS::Xc), (HIGH &&(i & 0b000000100)));
        // delay(70);

        if(digitalRead(static_cast<uint8_t>(SENSOR_PINS::Y))){
            isOpen=true;
            Serial.println("Y-BOX "+String(i+9));
        }
        if(digitalRead(static_cast<uint8_t>(SENSOR_PINS::X))){
            isOpen=true;
            Serial.println("X-BOX "+String(i));
        }
    }
    return isOpen;
}


// for(int i=0; i<20; i++) {
//     Serial.print("integer:  ");
//     Serial.println(i);
//     Serial.print((HIGH &&(i & 0b000000100)));Serial.print(" ");
//     Serial.print((HIGH &&(i & 0b000000010)));Serial.print(" ");
//     Serial.print((HIGH &&(i & 0b000000001)));
//     Serial.println();
//     Serial.println();
// }