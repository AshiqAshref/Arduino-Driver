#include "Led_indicator.h"

Led_Indicator::Led_Indicator(const byte &dataPin, const byte &clkPin, const byte &csPin, const byte numDevices) {
    boxLed = new LedControl(dataPin, clkPin, csPin, numDevices);
    initializeLed();
}

void Led_Indicator::initializeLed()const{
    boxLed->shutdown(0,false);
    boxLed->shutdown(1,false);
    boxLed->setIntensity(0,8);
    boxLed->setIntensity(1,8);
    boxLed->clearDisplay(0);
    boxLed->clearDisplay(1);
}

void Led_Indicator::ledTestFunction()const{
    int count=0;
    while(count<3){
        for(int i=0;i<16;i++){
            if(count==0)
                setColor(i,'g');
            else if(count==1)
                setColor(i,'b');
            else if(count==2)
                setColor(i,'r');
            delay(100);
        }
        count++;
    }
}


void Led_Indicator::blink(byte const boxNo,char const color) {
    if((millis()-blinkPrevioustime)>500){
        blinkPrevioustime=millis();
        if(blinkState)
            setColor(boxNo, color);
        else
            setColor(boxNo, 'c');
        blinkState=!blinkState;
    }
}

void Led_Indicator::setColor(const byte boxNo, const char color)const{  //color{r=red,g=green,b=blue,c=clear}
    int adr=0;
    int const row=boxNo%4;
    int col=0;

    if(boxNo<8){
        adr=0;
        if(color=='r' || color=='c'){
            if(boxNo<4) col=0;
            else        col=3;
        }else if(color=='b'){
            if(boxNo<4) col=1;
            else        col=4;
        }else if(color=='g'){
            if(boxNo<4) col=2;
            else        col=5;
        }
    }else if(boxNo>7){
        adr=1;
        if(color=='r' || color=='c'){
            if(boxNo<12) col=0;
            else         col=3;
        }else if(color=='b' ){
            if(boxNo<12) col=1;
            else         col=4;
        }else if(color=='g' ){
            if(boxNo<12) col=2;
            else         col=5;
        }
    }

    for(byte j=0;j<6;j++){
        if(col>=0 && col<3){
            boxLed->setLed(adr, row, j, false);
            if(j==2) j=6;
        }else if(col>2 && col<6){
            if(j==0) j=3;
            boxLed->setLed(adr, row, j, false);
        }
    }
    // for(byte j=0;j<6;j++){
    //   if(col>=0 && col<3){
    //     boxLed.setLed(adr, row, j, false);
    //     if(j==2) j=6;
    //   }else if(col>2 && col<6){
    //     if(j==0) j=3;
    //     boxLed.setLed(adr, row, j, false);
    //   }
    // }
    if(color!='c') boxLed->setLed(adr, row, col, true);
}
