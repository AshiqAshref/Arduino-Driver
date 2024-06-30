#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ReminderA.h>
#include <ReminderB.h>
#include <Led_Indicator.h>
#include <Mech_Arm.h>
#include <Lcd_Menu.h>

#include <HardwareSerial.h>
#include <SoftwareSerial.h>


Mech_Arm * mech_arm;
static Led_Indicator * led_indicator;
SoftwareSerial espPort(11, 12); //(Rx, Tx)

static Lcd_Menu *lcd_menu;


//         LED_PINS
constexpr byte dataPin= 9; //  pin 9 is connected to the DataIn
constexpr byte clkPin=8; //  pin 8 is connected to the CLK
constexpr byte csPin= 7; //  pin 7 is connected to LOAD
//        -LED_PINS-


int openBoxes[16]={};

static ReminderA current;
static ReminderA upcomming;
static ReminderB currentB;
static ReminderB upcommingB;


boolean initializeEspCommunicator();
void initializePins();

ReminderA jsonToClass(String& dat);
ReminderB jsonToClassB(String& dat);

void blink(byte boxNo,char color);
void checkEspForRequest();


#endif //MAIN_H
