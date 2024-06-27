#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ReminderA.h>
#include <ReminderB.h>
#include <Led_Indicator.h>
#include <Mech_Arm.h>


#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#include <Box.h>

#include <Wire.h>
#include <SPI.h>
Mech_Arm * mech_arm;
Led_Indicator * led_indicator;
SoftwareSerial espPort(11, 12); //(Rx, Tx)
Lcd_Menu * lcd_menu;

//         LED_PINS
constexpr byte dataPin= 9; //  pin 9 is connected to the DataIn
constexpr byte clkPin=8; //  pin 8 is connected to the CLK
constexpr byte csPin= 7; //  pin 7 is connected to LOAD
//        -LED_PINS-

//      SENSOR PINS
constexpr byte X=40;
constexpr byte Xa=34;
constexpr byte Xb=35;
constexpr byte Xc=36;
constexpr byte Y=41;
constexpr byte Ya=37;
constexpr byte Yb=38;
constexpr byte Yc=39;
//      SENSOR PINS

//      BUTTON_PINS
constexpr byte enterButton=2;
constexpr byte leftButton=3;
constexpr byte downButton=4;
constexpr byte rightButton=5;
constexpr byte upButton=6;
constexpr byte frontButton=46;
constexpr byte backButton=47;
//      BUTTON_PINS

//     STEPPER_PINS
constexpr byte xEnable=32;
constexpr byte xLimitSwitch=42;
constexpr byte X_step_pin=28;
constexpr byte X_dir_pin=29;

constexpr byte yEnable=31;
constexpr byte yLimitSwitch=44;
constexpr byte Y_step_pin=26;
constexpr byte Y_dir_pin=27;

constexpr byte zEnable=30;
constexpr byte zLimitSwitch=43;
constexpr byte Z_step_pin=24;
constexpr byte Z_dir_pin=25;
//     STEPPER_PINS

constexpr byte beeper=10;

int openBoxes[16]={};

static ReminderA current;
static ReminderA upcomming;
static ReminderB currentB;
static ReminderB upcommingB;


void initializeSteppers();
void initializeBoxSensor();
boolean initializeEspCommunicator();

ReminderA jsonToClass(String& dat);
ReminderB jsonToClassB(String& dat);


void blink(byte boxNo,char color);

void beepFor(int time);
bool checkAllBoxStatus();
bool isOpen(byte boxNo);





#endif //MAIN_H
