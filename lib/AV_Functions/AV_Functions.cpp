#include "AV_Functions.h"

#include <AV_PINS.h>

void AV_Functions::beepFor(const int time) {
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),HIGH);
    delay(time);
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),LOW);
}

void AV_Functions::beepFor(const int delay1, const int delay2) {
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),HIGH);
    delay(delay1);
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),LOW);
    delay(delay2);
}

void AV_Functions::beepFor(const int delay1, const int delay2, const byte repeat) {
    for(int i=0;i<repeat;i++) {
        digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),HIGH);
        delay(delay1);
        digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),LOW);
        delay(delay2);
    }
}


 void AV_Functions::waitForInput() {
    flush_();
    while(!Serial.available()) {}
    Serial.flush();
    while(Serial.available())Serial.read();
}


 void AV_Functions::flush_() {
    Serial.flush();
    while(Serial.available())Serial.read();
}







