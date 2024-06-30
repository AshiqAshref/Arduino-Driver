#include <AV_Functions.h>

#include "Main.h"
#include <BUTTON_PINS.h>
#include <AV_PINS.h>

#include <Wire.h>
#include <SPI.h>

void setup() {
  Serial.begin(9600);
  // while(!Serial.available()){}

  initializePins();
  led_indicator=new Led_Indicator(dataPin, clkPin, csPin, 2);
  lcd_menu = new Lcd_Menu();
  mech_arm = new Mech_Arm(lcd_menu, led_indicator);


  initializeEspCommunicator();

  // mech_arm->bringEmHome();

  // AV_Functions::beepFor(500);

  Serial.println();
  // led_indicator->ledTestFunction();
  // lcd_menu->lcdClear(0);
  // lcd.print("Press to Start");
  // while(!digitalRead(static_cast<uint8_t>(button_pins::enterButton))) {}
  // delay(500);

  // unlockAllBox();
  // bringEmHome();
  // badPosition();
  // unlockBox(14);
}

void loop() {
  while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::enterButton))) {
    while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::enterButton))){delay(100);}
    AV_Functions::beepFor(100);
    lcd_menu->menuPage();
    lcd_menu->getLcd().clear();
    lcd_menu->getLcd().print("MAIN PAGE");
  }
  // checkEspForRequest();
}


boolean initializeEspCommunicator(){
  espPort.begin(9600);
  delay(50);
  if(!espPort.isListening())
    return false;
  return true;
}


void checkEspForRequest() {
  String identifier="";
  while(espPort.available()){//...............................SOFT_SERIAL
    espPort.readStringUntil('{');

    identifier=espPort.readStringUntil(':');
    if(identifier=="ALM1"){ //...........................Mode_B(Default)
      Serial.print("Alarm1= ");
      String alm=espPort.readStringUntil('}')+'}';
      currentB=jsonToClassB(alm);
      Serial.println(currentB.toString());
    }else if(identifier=="UPC1"){
      Serial.print("Upcomming1= ");
      String upc=espPort.readStringUntil('}')+'}';
      upcommingB=jsonToClassB(upc);
      Serial.println(upcommingB.toString());

    }else if(identifier=="ALM0"){//.......................Mode_A
      Serial.print("Alarm0= ");
      String alm=espPort.readStringUntil('}')+'}';
      current=jsonToClass(alm);
      Serial.println(current.toString());
    }else if(identifier=="UPC0"){
      Serial.print("Upcomming0= ");
      String upc=espPort.readStringUntil('}')+'}';
      upcomming=jsonToClass(upc);
      Serial.println(upcomming.toString());
    }
  }

}


ReminderA jsonToClass(String& dat){
  unsigned int id=dat.substring((dat.indexOf(':')+1),dat.indexOf(',')).toInt();

  dat=dat.substring(dat.indexOf(',')+1);
  String date=dat.substring(dat.indexOf(':')+1,dat.indexOf(','));

  byte H=date.substring(1,3).toInt();
  byte M=date.substring(4,6).toInt();

  dat=dat.substring(dat.indexOf(',')+1);
  unsigned int boxNo=dat.substring((dat.indexOf(':')+2),(dat.indexOf(',')-1)).toInt();

  return {new DateTime(0,0,0,H,M,0),&boxNo,&id, new boolean(false)};
}


ReminderB jsonToClassB(String& dat) {
  byte H = dat.substring((dat.indexOf('=') + 1), dat.indexOf(':')).toInt();
  byte M = dat.substring((dat.indexOf(':') + 1), dat.indexOf(',')).toInt();

  String boxesString = dat.substring((dat.indexOf(',') + 1));
  boxesString = boxesString.substring(boxesString.indexOf('=') + 1);
  String success=boxesString.substring(boxesString.indexOf('=')+1, boxesString.length()-1);
  boxesString = boxesString.substring(0, boxesString.indexOf(']')+1);

  boolean suc=false;
  if(success.toInt()==1){
    suc=true;
  }else if(success.toInt()==0){
    suc=false;
  }
  boxesString.trim();
  return {new DateTime(0, 0, 0, H, M, 0), &boxesString, &suc};
}


unsigned long blinkPrevioustime=0;
bool blinkState=false;
void blink(byte const boxNo,char const color){
  if((millis()-blinkPrevioustime)>500){
    blinkPrevioustime=millis();
    if(blinkState)
      led_indicator->setColor(boxNo, color);
    else
      led_indicator->setColor(boxNo, 'c');
    blinkState=!blinkState;
  }
}


void initializePins(){
  pinMode(static_cast<uint8_t>(AV_PINS::beeper), OUTPUT);
  pinMode(static_cast<uint8_t>(BUTTON_PINS::enterButton),INPUT);
  pinMode(static_cast<uint8_t>(BUTTON_PINS::leftButton),INPUT);
  pinMode(static_cast<uint8_t>(BUTTON_PINS::rightButton),INPUT);
  pinMode(static_cast<uint8_t>(BUTTON_PINS::upButton),INPUT);
  pinMode(static_cast<uint8_t>(BUTTON_PINS::downButton),INPUT);
  pinMode(static_cast<uint8_t>(BUTTON_PINS::frontButton),INPUT);
  pinMode(static_cast<uint8_t>(BUTTON_PINS::backButton),INPUT);
}

