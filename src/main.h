#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Box.h>
#include <Pos_Coordinate.h>
unsigned long xCordinate[16]={
    4200, 3000, 1500, 0000,
    4200, 2900, 1400, 0000,
    4300, 2900, 1300, 0000,
    4300, 2900, 1400, 0000
};

unsigned long yCordinate[16]={
    8800, 8900, 8700, 8700,
    5900, 5900, 5900, 5900,
    3200, 3200, 3200, 3200,
    0600, 0500, 0400, 0400
};

unsigned long zCordinate[16]={
    1700, 1800, 2000, 2200,
    1600, 1700, 1900, 2100,
    1700, 1800, 1800, 2100,
    1500, 1600, 1800, 1900
};

byte STAT_LED_PIN = 52;

// boolean initializeEspCommunicator();
void initializePins();

void addBoxes();
String get_formated_Time(byte mode = 24);
String beautifyTime(uint8_t h_m_s);

unsigned long get_current_plain_unix_time();


// ReminderA jsonToClass(String& dat);
// ReminderB jsonToClassB(const String& dat);
//

// void checkEspForRequest();
// void initializeBoxes();
// void addReminders();
#endif //MAIN_H
