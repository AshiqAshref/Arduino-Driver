#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ReminderA.h>
#include <ReminderB.h>
#include <Box.h>

#include <HardwareSerial.h>
#include <LedControl.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#include <Wire.h>
#include <SPI.h>

//  pin 9 is connected to the DataIn
//  pin 8 is connected to the CLK
//  pin 7 is connected to LOAD
LedControl boxLed = LedControl(9, 8, 7, 2); //(DIN, CLK, LOAD, no_of_chained_devices)
LiquidCrystal_I2C lcd(0x27, 16, 2); //(addr, row, col)
SoftwareSerial espPort(11, 12); //(Rx, Tx)

AccelStepper stepperX(1,28,29); //(1,step,dir)
AccelStepper stepperY(1,26,27);
AccelStepper stepperZ(1,24,25);



const byte X=40;
const byte Xa=34;
const byte Xb=35;
const byte Xc=36;

const byte Y=41;
const byte Ya=37;
const byte Yb=38;
const byte Yc=39;

const byte enterButton=2;
const byte leftButton=3;
const byte downButton=4;
const byte rightButton=5;
const byte upButton=6;
const byte frontButton=46;
const byte backButton=47;

const byte zEnable=30;
const byte yEnable=31;
const byte xEnable=32;

const byte xLimitSwitch=42;
const byte zLimitSwitch=43;
const byte yLimitSwitch=44;

const byte beeper=10;
const byte buttonDelay=0;

unsigned long xAxis=0;
unsigned long yAxis=0;
unsigned long zAxis=0;

byte SYSTEM_IP[4]={ 192,168,4,28 };
byte SYSTEM_SB[4]={ 255,255,255,0 };

unsigned long xCordinate[16]={
    17500, 12500, 6500, 0,
    17000, 12000, 6000, 0,
    17000, 11500, 5500, 0,
    17500, 11500, 5500, 0
};

unsigned long yCordinate[16]={
    71000, 71000, 70000, 70000,
    48000, 48000, 47500, 47000,
    26000, 26500, 25500, 26500,
    4500 , 4000 , 4000 , 4000
};

unsigned long zCordinate[16]={
    12000, 12000, 13000, 15500,
    11500, 12500, 13500, 15500,
    11500, 12500, 12500, 14500,
    12000, 12000, 13500, 14000
};


int openBoxes[16]={};

static ReminderA current;
static ReminderA upcomming;
static ReminderB currentB;
static ReminderB upcommingB;


void initializePins();
void initializeSteppers();
void initializeLed();
void initializeLcd();
void initializeBoxSensor();
boolean initializeEspCommunicator();

ReminderA jsonToClass(String& dat);
ReminderB jsonToClassB(String& dat);
void menuPage();
void setupPage();
void ipMenu();
boolean confirm(byte *iP);
String getIp(byte *a);
String getIpBig(byte *a);
void setIp(byte *temp_SYS_IP);
void bigToSmall(byte *temp_SYS_IP,byte *bigIp);
void smallToBig(byte *temp_SYS_IP, byte *bigIp);
String ipSort(String ip);

void moveStepper();
void unlockAllBox();
void offsetCoordinate(char cordinate, byte offsetPosition, int offSetBy, boolean applyToAll);
boolean unlockBox(byte boxNo);
boolean resetPosition(byte axis);
void bringEmHome();
void stepperState(byte enable, boolean state);
void boxMarker();

void setColor(byte boxNo, char color);
void ledTestFunction();
void blink(byte boxNo,char color);

void beepFor(int time);
void lcdClear(byte col);

bool checkAllBoxStatus();
bool isOpen(byte boxNo);





#endif //MAIN_H
