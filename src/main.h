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



static Box boxes[16] = {
    Box(1 , Pos_Coordinate(xCordinate[0] ,yCordinate[0] , zCordinate[0])) ,
    Box(2 , Pos_Coordinate(xCordinate[1] ,yCordinate[1] , zCordinate[1])) ,
    Box(3 , Pos_Coordinate(xCordinate[2] ,yCordinate[2] , zCordinate[2])) ,
    Box(4 , Pos_Coordinate(xCordinate[3] ,yCordinate[3] , zCordinate[3])) ,
    Box(5 , Pos_Coordinate(xCordinate[4] ,yCordinate[4] , zCordinate[4])) ,
    Box(6 , Pos_Coordinate(xCordinate[5] ,yCordinate[5] , zCordinate[5])) ,
    Box(7 , Pos_Coordinate(xCordinate[6] ,yCordinate[6] , zCordinate[6])) ,
    Box(8 , Pos_Coordinate(xCordinate[7] ,yCordinate[7] , zCordinate[7])) ,
    Box(9 , Pos_Coordinate(xCordinate[8] ,yCordinate[8] , zCordinate[8])) ,
    Box(10, Pos_Coordinate(xCordinate[9] ,yCordinate[9] , zCordinate[9])) ,
    Box(11, Pos_Coordinate(xCordinate[10],yCordinate[10], zCordinate[10])),
    Box(12, Pos_Coordinate(xCordinate[11],yCordinate[11], zCordinate[11])),
    Box(13, Pos_Coordinate(xCordinate[12],yCordinate[12], zCordinate[12])),
    Box(14, Pos_Coordinate(xCordinate[13],yCordinate[13], zCordinate[13])),
    Box(15, Pos_Coordinate(xCordinate[14],yCordinate[14], zCordinate[14])),
    Box(16, Pos_Coordinate(xCordinate[15],yCordinate[15], zCordinate[15]))
};
// boolean initializeEspCommunicator();
void initializePins();
//
// ReminderA jsonToClass(String& dat);
// ReminderB jsonToClassB(const String& dat);
//

// void checkEspForRequest();
// void initializeBoxes();

void addBoxes();
void addReminders();

String beautifyTime(uint8_t h_m_s);
#endif //MAIN_H
