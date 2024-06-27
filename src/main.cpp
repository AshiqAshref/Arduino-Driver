#include <Lcd_Menu.h>

#include "Main.h"
#include <IO_pins_struct.h>

void setup() {
  Serial.begin(115200);
  led_indicator=new Led_Indicator(dataPin, clkPin, csPin, 2);
  lcd_menu = new Lcd_Menu(enterButton, leftButton, downButton,
    rightButton, upButton, frontButton, backButton);
  mech_arm = new Mech_Arm(X_step_pin,X_dir_pin,xEnable,xLimitSwitch,
                          Y_step_pin,Y_dir_pin,yEnable,yLimitSwitch,
                          Z_step_pin,Z_dir_pin,zEnable,zLimitSwitch,
                          beeper, lcd_menu, led_indicator
    );

  pinMode(beeper, OUTPUT);
  initializeEspCommunicator();
  // initializeSteppers();
  // bringEmHome();

  delay(350);
  beepFor(500);

  Serial.println();
  led_indicator->ledTestFunction();
  lcd_menu->lcdClear(0);
  // lcd.print("Press to Start");
  // while(!digitalRead(enterButton)) {}
  // delay(500);

  // unlockAllBox();
  // bringEmHome();
  // badPosition();
  // unlockBox(14);
}


void beepFor(const int time){
  digitalWrite(beeper, HIGH);
  delay(time);
  digitalWrite(beeper, LOW);
}


boolean startFlag=false;
void loop() {
  while(digitalRead(enterButton)) {
    lcd_menu->menuPage();
    break;
  }

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

  //END LOOP
}


void initializeBoxSensor(){
    pinMode(X, INPUT);
    pinMode(Xa, OUTPUT);
    pinMode(Xb, OUTPUT);
    pinMode(Xc, OUTPUT);

    pinMode(Y, INPUT);
    pinMode(Ya, OUTPUT);
    pinMode(Yb, OUTPUT);
    pinMode(Yc, OUTPUT);
}


boolean initializeEspCommunicator(){
  espPort.begin(9600);
  delay(50);
  if(!espPort.isListening())
    return false;
  return true;
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


bool isOpen(byte const boxNo){
  if(boxNo<=8){
    digitalWrite(Ya, (HIGH &&((boxNo-1) & 0b000000001)));
    digitalWrite(Yb, (HIGH &&((boxNo-1) & 0b000000010)));
    digitalWrite(Yc, (HIGH &&((boxNo-1) & 0b000000100)));
    delay(70);
    if(digitalRead(Y))
      return true;
  }else{
    digitalWrite(Xa, (HIGH &&((boxNo-9) & 0b000000001)));
    digitalWrite(Xb, (HIGH &&((boxNo-9) & 0b000000010)));
    digitalWrite(Xc, (HIGH &&((boxNo-9) & 0b000000100)));
    delay(70);
    if(digitalRead(X))
      return true;
  }
  return false;
}


bool checkAllBoxStatus(){
  bool isOpen=false;
  for(int & openBoxe : openBoxes){
    openBoxe=0;
  }

  for(byte i=0;i<8;i++){
    digitalWrite(Ya, (HIGH &&(i & 0b000000001)));
    digitalWrite(Yb, (HIGH &&(i & 0b000000010)));
    digitalWrite(Yc, (HIGH &&(i & 0b000000100)));

    digitalWrite(Xa, (HIGH &&(i & 0b000000001)));
    digitalWrite(Xb, (HIGH &&(i & 0b000000010)));
    digitalWrite(Xc, (HIGH &&(i & 0b000000100)));
    delay(70);

    if(digitalRead(Y)){
      isOpen=true;
      Serial.println("BOX "+String(i)+"\n\n");
      for(int & openBoxe : openBoxes){
        if(openBoxe==0){
          openBoxe=i+1;
          break;
        }
      }
    }
    if(digitalRead(X)){
      isOpen=true;
      Serial.println("BOX "+String(i+9)+"\n\n");
      for(int & openBoxe : openBoxes){
        if(openBoxe==0){
          openBoxe=i+9;
          break;
        }
      }
    }
  }
  return isOpen;
}
