#ifndef MECH_ARM_H
#define MECH_ARM_H
#include <Arduino.h>
#include <AccelStepper.h>
#include <ModeB/Box.h>


class Mech_Arm {
    //(1,step,dir)
    AccelStepper stepperX; //x=1/4 step
    AccelStepper stepperY; //y=1/2 step
    AccelStepper stepperZ; //z=1/2 step
    // Lcd_Menu lcd_menu;
    // Led_Indicator *led_indicator;

    uint32_t xAxis=0;
    uint32_t yAxis=0;
    uint32_t zAxis=0;

    // unsigned long xCordinate[16]={
    //     17500, 12500, 6500, 0,
    //     17000, 12000, 6000, 0,
    //     17000, 11500, 5500, 0,
    //     17500, 11500, 5500, 0
    // };
    //
    // unsigned long yCordinate[16]={
    //     71000, 71000, 70000, 70000,
    //     48000, 48000, 47500, 47000,
    //     26000, 26500, 25500, 26500,
    //     4500 , 4000 , 4000 , 4000
    // };
    //
    // unsigned long zCordinate[16]={
    //     12000, 12000, 13000, 15500,
    //     11500, 12500, 13500, 15500,
    //     11500, 12500, 12500, 14500,
    //     12000, 12000, 13500, 14000
    // };

    static void bringEmHomeDummy();

    // static boolean unlockBoxDummy(Box *box);


public:
    // Mech_Arm(const Lcd_Menu &lcd_menu, Led_Indicator *led_indicator);
    Mech_Arm();
    void initializeSteppers() ;
    static void stepperState(byte enable, boolean state);
    void moveStepper();
    boolean resetPosition(byte axis);

    void unlockBox(byte box_positions_[], byte box_size);

    void unlockBox(Box *box);
    void unlockAllBox();
    void bringEmHome();
    void flush_();
    void offsetCoordinate(char cordinate, byte offsetPosition, int offSetBy, boolean applyToAll);
    void boxMarker();


};
#endif //MECH_ARM_H
