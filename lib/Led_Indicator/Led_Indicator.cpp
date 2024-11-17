#include "Led_indicator.h"

Led_Indicator::Led_Indicator() {
    initializeLed();
}

void Led_Indicator::initializeLed(){
    boxLed.shutdown(0,false);
    boxLed.shutdown(1,false);
    boxLed.setIntensity(0,8);
    boxLed.setIntensity(1,8);
    boxLed.clearDisplay(0);
    boxLed.clearDisplay(1);
}

void Led_Indicator::ledTestFunction(const unsigned int delay_){
    for(byte color = 0;color<3;color++){
        for(byte i=1;i<=16;i++){
            if(color==0)
                setColor(i,COLOR_GREEN);
            else if(color==1)
                setColor(i,COLOR_BLUE);
            else if(color==2)
                setColor(i,COLOR_RED);
            delay(delay_);
        }
    }
}


void Led_Indicator::blink(byte const boxNo, const COLOR color) {
    if((millis()-blinkPrevioustime)>500){
        blinkPrevioustime=millis();
        if(blinkState)
            setColor(boxNo, color);
        else
            setColor(boxNo, COLOR_CLEAR);
        blinkState=!blinkState;
    }
}


void Led_Indicator::setColor(byte boxNo, const COLOR *color) {
    int adr=0;
    boxNo--;
    int const row=boxNo%4;
    int col=0;
    for(int i=0;i<2;i++) {
        if(boxNo<=7){
            adr=0;
            if(color[i] == COLOR_RED || color[i]==COLOR_CLEAR){
                if(boxNo<=3) col=0;
                else        col=3;
            }else if(color[i]==COLOR_BLUE){
                if(boxNo<=3) col=1;
                else        col=4;
            }else if(color[i]==COLOR_GREEN){
                if(boxNo<=3) col=2;
                else        col=5;
            }
        }else if(boxNo>7){
            adr=1;
            if(color[i]==COLOR_RED || color[i]==COLOR_CLEAR){
                if(boxNo<12) col=0;
                else         col=3;
            }else if(color[i]==COLOR_BLUE ){
                if(boxNo<12) col=1;
                else         col=4;
            }else if(color[i]==COLOR_GREEN ){
                if(boxNo<12) col=2;
                else         col=5;
            }
        }

        if(i==0){
            if(col<3) for(byte j=0;j<3;j++)
                boxLed.setLed(adr, row, j, false);
            else for(byte j=3;j<6;j++)
                boxLed.setLed(adr, row, j, false);
        }
        if(color[i]!=COLOR_CLEAR) boxLed.setLed(adr, row, col, true);
    }
}


void Led_Indicator::setColor(byte boxNo, const COLOR color){  //color{r=red,g=green,b=blue,c=clear}
    int adr=0;
    boxNo--;
    int const row=boxNo%4;
    int col=0;

    if(boxNo<=7){
        adr=0;
        if(color==COLOR_RED || color==COLOR_CLEAR){
            if(boxNo<=3) col=0;
            else        col=3;
        }else if(color==COLOR_BLUE){
            if(boxNo<=3) col=1;
            else        col=4;
        }else if(color==COLOR_GREEN){
            if(boxNo<=3) col=2;
            else        col=5;
        }
    }else if(boxNo>7){
        adr=1;
        if(color==COLOR_RED || color==COLOR_CLEAR){
            if(boxNo<12) col=0;
            else         col=3;
        }else if(color==COLOR_BLUE ){
            if(boxNo<12) col=1;
            else         col=4;
        }else if(color==COLOR_GREEN ){
            if(boxNo<12) col=2;
            else         col=5;
        }
    }

    if(col<3) for(byte j=0;j<3;j++)
            boxLed.setLed(adr, row, j, false);
    else for(byte j=3;j<6;j++)
            boxLed.setLed(adr, row, j, false);

    if(color!=COLOR_CLEAR) boxLed.setLed(adr, row, col, true);
}
