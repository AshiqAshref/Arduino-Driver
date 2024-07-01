#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ReminderA.h>
#include <ReminderB.h>
#include <Led_Indicator.h>
#include <Mech_Arm.h>

#include <Wire.h>
#include <SPI.h>

#include <HardwareSerial.h>
#include <SoftwareSerial.h>





//         LED_PINS
constexpr byte dataPin= 9; //  pin 9 is connected to the DataIn
constexpr byte clkPin=8; //  pin 8 is connected to the CLK
constexpr byte csPin= 7; //  pin 7 is connected to LOAD
//        -LED_PINS-


int openBoxes[16]={};

// static ReminderA current;
// static ReminderA upcomming;
// static ReminderB currentB;
// static ReminderB upcommingB;


boolean initializeEspCommunicator();
void initializePins();

ReminderA jsonToClass(String& dat);
ReminderB jsonToClassB(const String& dat);

void blink(byte boxNo,char color);
void checkEspForRequest();


#endif //MAIN_H
