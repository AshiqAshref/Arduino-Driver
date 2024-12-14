#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <ModeB/Box.h>
#include <Pos_Coordinate.h>
#include <ModeB/ReminderB.h>
#include <RTClib.h>

uint32_t xCordinate[16]={
    5300, 4000, 2600, 1200,
    5400, 4100, 2500, 1100,
    5300, 3900, 2600, 1100,
    5400, 4000, 2600, 1200
};

uint32_t yCordinate[16]={
    8450, 8450, 8550, 8350,
    5650, 5650, 5650, 5650,
    2950, 2850, 2850, 2850,
     100,    0,    0,    0
};

uint32_t zCordinate[16]={
    2000, 2100, 2300, 2200,
    2300, 2150, 2250, 2250,
    2150, 1950, 2250, 2100,
    2000, 2100, 2300, 2200
};

byte STAT_LED_PIN = 52;

// boolean initializeEspCommunicator();
void initializePins();

void addBoxes();
String get_formated_Time(const DateTime &curr_time, TimeMode mode = TIME_MODE_12);
String beautifyTime(uint8_t h_m_s);

unsigned long get_current_unix_time();
void print_lcd_time(const DateTime &current_time, TimeMode mode=TIME_MODE_12);


// void initializeBoxes();
// void addReminders();
#endif //MAIN_H
