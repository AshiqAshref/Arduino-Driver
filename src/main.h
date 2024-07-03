#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>



boolean initializeEspCommunicator();
void initializePins();
//
// ReminderA jsonToClass(String& dat);
// ReminderB jsonToClassB(const String& dat);
//
void blink(byte boxNo,char color);
void checkEspForRequest();


#endif //MAIN_H
