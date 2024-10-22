#include "AV_Functions.h"

#include <AV_PINS.h>

void AV_Functions::beepFor(const int time) {
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),LOW);
    delay(time);
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),HIGH);
}


void AV_Functions::beepFor(const int delay1, const int delay2) {
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),LOW);
    delay(delay1);
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),HIGH);
    delay(delay2);
}


void AV_Functions::beepFor(const int delay1, const int delay2, const byte repeat) {
    for(int i=0;i<repeat;i++) {
        digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),LOW);
        delay(delay1);
        digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),HIGH);
        delay(delay2);
    }
}


 void AV_Functions::waitForInput() {
    flush_();
    while(!Serial.available()) {}
    Serial.flush();
    while(Serial.available())Serial.read();
}


COLOR AV_Functions::char_to_enum(const char a) {
    if (a=='r')
        return COLOR::RED;
    if (a=='g')
        return COLOR::GREEN;
    if (a=='b')
        return COLOR::BLUE;
    if (a=='c')
        return COLOR::CLEAR;
    Serial.println("NO VAL");
    return COLOR::CLEAR;
}

char AV_Functions::enum_to_char(const COLOR color) {
    if(color == COLOR::RED)
        return 'r';
    if(color == COLOR::GREEN)
        return 'g';
    if(color == COLOR::BLUE)
        return 'b';
    if(color == COLOR::CLEAR)
        return 'c';
    Serial.println("NO VAL");
    return 'c';
}

String colorToString();

 void AV_Functions::flush_() {
    Serial.flush();
    while(Serial.available())Serial.read();
}









