#ifndef MECH_ARM_H
#define MECH_ARM_H
#include <Arduino.h>
#include <AccelStepper.h>
#include <Led_Indicator.h>
#include <IO_pins_struct.h>

#include <Lcd_Menu.h>

class Mech_Arm {
private:
    AccelStepper *stepperX; //(1,step,dir)
    AccelStepper *stepperY;
    AccelStepper *stepperZ;
    Lcd_Menu * lcd_menu;
    Led_Indicator * led_indicator;

    byte beeper_pin;

    byte xLimitSwitch;
    byte yLimitSwitch;
    byte zLimitSwitch;

    byte xEnable;
    byte yEnable;
    byte zEnable;

    unsigned long xAxis=0;
    unsigned long yAxis=0;
    unsigned long zAxis=0;

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
    const byte enterButton=2;
    const byte leftButton=3;
    const byte downButton=4;
    const byte rightButton=5;
    const byte upButton=6;
    const byte frontButton=46;
    const byte backButton=47;
    const byte buttonDelay=0;




public:
    Mech_Arm(const byte & X_step_pin, const byte & X_dir_pin, const byte & xEnable_, const byte & xLimitSwitch_,
             const byte & Y_step_pin, const byte & Y_dir_pin, const byte & yEnable_, const byte & yLimitSwitch_,
             const byte & Z_step_pin, const byte & Z_dir_pin, const byte & zEnable_, const byte & zLimitSwitch_,
             const byte & beeper_pin_ ,Lcd_Menu * lcd_menu_, Led_Indicator * led_indicator_
        );
    void initializeSteppers() const;
    static void stepperState(byte enable, boolean state);
    void moveStepper();
    boolean resetPosition(byte axis);
    boolean unlockBox(byte boxNo);
    void unlockAllBox();
    void bringEmHome();
    void offsetCoordinate(char cordinate, byte offsetPosition, int offSetBy, boolean applyToAll);
    void boxMarker();
    void beepFor(int time) const;










};
#endif //MECH_ARM_H
