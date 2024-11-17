#include "sensor_unit.h"
#include <Box.h>
#include <IO_PINS.h>


Sensor_unit::Sensor_unit() {
    initializeBoxSensor();
}

void Sensor_unit::initializeBoxSensor(){
    pinMode(SENSOR_PIN_X , INPUT);
    pinMode(SENSOR_PIN_XA, OUTPUT);
    pinMode(SENSOR_PIN_XB, OUTPUT);
    pinMode(SENSOR_PIN_XC, OUTPUT);

    pinMode(SENSOR_PIN_Y , INPUT);
    pinMode(SENSOR_PIN_YA, OUTPUT);
    pinMode(SENSOR_PIN_YB, OUTPUT);
    pinMode(SENSOR_PIN_YC, OUTPUT);
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
        digitalWrite(SENSOR_PIN_XA, (HIGH &&(boxNo & 0b000000001)));
        digitalWrite(SENSOR_PIN_XB, (HIGH &&(boxNo & 0b000000010)));
        digitalWrite(SENSOR_PIN_XC, (HIGH &&(boxNo & 0b000000100)));
        if(digitalRead(SENSOR_PIN_X))
            return true;
    }else{
        digitalWrite(SENSOR_PIN_YA, (HIGH &&(boxNo & 0b000000001)));
        digitalWrite(SENSOR_PIN_YB, (HIGH &&(boxNo & 0b000000010)));
        digitalWrite(SENSOR_PIN_YC, (HIGH &&(boxNo & 0b000000100)));
        if(digitalRead(SENSOR_PIN_Y))
            return true;
    }
    return false;
}

bool Sensor_unit::checkAllBoxStatus(){
    bool isOpen=false;
    for(byte i=0;i<8;i++){
        digitalWrite(SENSOR_PIN_YA, (HIGH &&(i & 0b000000001)));
        digitalWrite(SENSOR_PIN_YB, (HIGH &&(i & 0b000000010)));
        digitalWrite(SENSOR_PIN_YC, (HIGH &&(i & 0b000000100)));

        digitalWrite(SENSOR_PIN_XA, (HIGH &&(i & 0b000000001)));
        digitalWrite(SENSOR_PIN_XB, (HIGH &&(i & 0b000000010)));
        digitalWrite(SENSOR_PIN_XC, (HIGH &&(i & 0b000000100)));
        // delay(70);

        if(digitalRead(SENSOR_PIN_Y)){
            isOpen=true;
            Serial.println("Y-BOX "+String(i+9));
        }
        if(digitalRead(SENSOR_PIN_X)){
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