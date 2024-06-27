#include "Main.h"

void setup() {
  Serial.begin(115200);

  delay(100);
  initializePins();
  initializeEspCommunicator();
  initializeLed();
  initializeLcd();
  initializeSteppers();
  // bringEmHome();

  delay(350);
  beepFor(500);

  Serial.println();
  ledTestFunction();

  lcdClear(0);
  lcd.print("Press to Start");
  while(!digitalRead(enterButton));
  delay(500);

  // unlockAllBox();
  // bringEmHome();
  // badPosition();
  // unlockBox(14);
}


boolean mainDisplayFlag=1;
boolean startFlag=false;
void loop() {
  if(mainDisplayFlag==1){
    lcd.clear();
    lcd.setCursor(0,0);//(16,2)
    lcd.print("Upcomming: 09:36");
    lcd.setCursor(0,1);
    lcd.print("BoxNo: 07");
    mainDisplayFlag=0;
  }

  boolean enterPressed=0;
  while(digitalRead(enterButton)) enterPressed=1;
  if(enterPressed){
    menuPage();
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



void initializePins(){
  pinMode(xEnable, OUTPUT);
  pinMode(yEnable, OUTPUT);
  pinMode(zEnable, OUTPUT);

  pinMode(enterButton,INPUT);
  pinMode(leftButton,INPUT);
  pinMode(rightButton,INPUT);
  pinMode(upButton,INPUT);
  pinMode(downButton,INPUT);

  pinMode(beeper, OUTPUT);
}



void initializeLed(){
  boxLed.shutdown(0,false);
  boxLed.shutdown(1,false);
  boxLed.setIntensity(0,8);
  boxLed.setIntensity(1,8);
  boxLed.clearDisplay(0);
  boxLed.clearDisplay(1);
}



void initializeLcd(){
  lcd.init();
  lcd.backlight();
	lcd.clear();
	lcd.setCursor(2,0);
	lcd.print("Initialized");
}



void initializeSteppers(){
  stepperX.setAcceleration(3500);
  stepperY.setAcceleration(1500);
  stepperZ.setAcceleration(8000);

  stepperX.setEnablePin(xEnable);
  stepperY.setEnablePin(yEnable);
  stepperZ.setEnablePin(zEnable);

  stepperX.setMaxSpeed(2500);
  stepperY.setMaxSpeed(8000);
  stepperZ.setMaxSpeed(12000);

  stepperX.setSpeed(2500);
  stepperY.setSpeed(3999);
  stepperZ.setSpeed(8000);

  stepperState(xEnable, false);
  stepperState(yEnable, false);
  stepperState(zEnable, false);
}


void menuPage(){
  boolean idleFlag=1;
  byte totalCase=2;
  boolean setupFlag=0;
  byte caseInt=0;
  boolean menuFlag=1;

  boolean enterState=0;
  boolean upState=0;
  boolean downState=0;

  while(menuFlag==1){
    while(digitalRead(upButton))    upState=1;
    while(digitalRead(downButton))  downState=1;
    while(digitalRead(enterButton)) enterState=1;

    if(upState==1){
      if(caseInt<=0){
        caseInt=totalCase;
      }else{
        caseInt--;
      }

      idleFlag=1;
      upState=0;
    }

    if(downState==1){
      if(caseInt>=totalCase){
        caseInt=0;
      }else{
        caseInt++;
      }

      idleFlag=1;
      downState=0;
    }

    if(enterState==1){
      if(caseInt==totalCase){
        menuFlag=0;
        mainDisplayFlag=1;
      }else if(caseInt==0){
        setupFlag=1;
        idleFlag=1;
      }
      enterState=0;

    }

    if(idleFlag==1){
      switch(caseInt){

        case 0:
          if(setupFlag==1){
            setupPage();
          }

          lcd.clear();
          lcd.setCursor(5,0);
          lcd.print(">Setup<");
          lcd.setCursor(6,1);
          lcd.print("Help");

          setupFlag=0;
          idleFlag=0;
          break;

        case 1:
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("Setup");
          lcd.setCursor(5,1);
          lcd.print(">Help<");
          idleFlag=0;
          break;

        case 2:
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("Help");
          lcd.setCursor(5,1);
          lcd.print(">Back<");
          idleFlag=0;
          break;

      }
      delay(1);
    }
  }
}



void setupPage(){
  boolean idleFlag=1;
  byte totalCase=1;
  byte caseInt=0;
  boolean ipFlag=0;
  boolean setupFlag=1;

  boolean enterState=0;
  boolean upState=0;
  boolean downState=0;

  while(setupFlag==1){

    while(digitalRead(upButton))    upState=1;
    while(digitalRead(downButton))  downState=1;
    while(digitalRead(enterButton)) enterState=1;

    if(upState==1){
      if(caseInt<=0){
        caseInt=totalCase;
      }else{
        caseInt--;
      }

      idleFlag=1;
      upState=0;
    }

    if(downState==1){
      if(caseInt>=totalCase){
        caseInt=0;
      }else{
        caseInt++;
      }

      idleFlag=1;
      downState=0;
    }

    if(enterState==1){

      if(caseInt==totalCase){
        setupFlag=0;
        idleFlag=0;
      }else if(caseInt==0){
        ipFlag=1;
        idleFlag=1;
      }
      enterState=0;
    }

    if(idleFlag==1){
      switch(caseInt){

        case 0:
          if(ipFlag==1){
            ipMenu();
            ipFlag=0;
          }
          lcd.clear();
          lcd.setCursor(2,0);
          lcd.print(">Ip address<");
          lcd.setCursor(6,1);
          lcd.print("Back");

          idleFlag=0;
          break;

        case 1:
          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("Ip address");
          lcd.setCursor(5,1);
          lcd.print(">Back<");
          idleFlag=0;
          break;
      }
      delay(1);
    }
  }
}



void ipMenu(){

  byte ip[8]={
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b10111,
    0b10101,
    0b10111,
    0b10100
  };
  byte sb[8]={
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11010,
    0b10010,
    0b01011,
    0b11011
  };

  lcd.createChar(2, ip);
  lcd.createChar(3, sb);

  boolean idleFlag=1;
  byte totalCase=3;
  byte caseInt=0;
  boolean showIpFlag=0;
  boolean setIpFlag=0;
  boolean setSbFlag=0;
  boolean ipFlag=1;

  boolean enterState=0;
  boolean upState=0;
  boolean downState=0;

  while(ipFlag==1){

    byte temp_SYS_IP[4];
    byte temp_SYS_SB[4];

    for(int i=0;i<4;i++){
      temp_SYS_IP[i]=SYSTEM_IP[i];
      temp_SYS_SB[i]=SYSTEM_SB[i];
    }

    while(digitalRead(upButton))    upState=1;
    while(digitalRead(downButton))  downState=1;
    while(digitalRead(enterButton)) enterState=1;

    if(upState==1){
      if(caseInt<=0){
        caseInt=totalCase;
      }else{
        caseInt--;
      }

      idleFlag=1;
      upState=0;
    }

    if(downState==1){
      if(caseInt>=totalCase){
        caseInt=0;
      }else{
        caseInt++;
      }

      idleFlag=1;
      downState=0;
    }

    if(enterState==1){

      if(caseInt==totalCase){
        ipFlag=0;
        idleFlag=0;
      }else if(caseInt==0){
        showIpFlag=1;
        idleFlag=1;
      }else if(caseInt==1){
        setIpFlag=1;
        idleFlag=1;
      }else if(caseInt==2){
        setSbFlag=1;
        idleFlag=1;
      }
      enterState=0;
    }

    if(idleFlag==1){
      switch(caseInt){
        case 0:
          while(showIpFlag==1){
            if(idleFlag==1){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.write(2);
              lcd.print(getIp(SYSTEM_IP));

              lcd.setCursor(0,1);
              lcd.write(3);
              lcd.print(getIp(SYSTEM_SB));

              idleFlag=0;
            }
            while(digitalRead(enterButton))enterState=1;
            if(enterState==1){
              enterState=0;
              showIpFlag=0;
            }
          }
          lcd.clear();
          lcd.setCursor(4,0);
          lcd.print(">Show Ip<");
          lcd.setCursor(5,1);
          lcd.print("Set Ip");
          idleFlag=0;
          break;

        case 1:
          while(setIpFlag==1){
            setIp(temp_SYS_IP);
            if(confirm(temp_SYS_IP)){
              for(int i=0;i<4;i++)
                SYSTEM_IP[i]=temp_SYS_IP[i];
            }

            setIpFlag=0;
          }
          lcd.clear();
          lcd.setCursor(5,0);
          lcd.print("Show Ip");
          lcd.setCursor(4,1);
          lcd.print(">Set Ip<");
          idleFlag=0;
          break;

        case 2:
          while(setSbFlag==1){
            setIp(temp_SYS_SB);
            if(confirm(temp_SYS_SB)){
              for(int i=0;i<4;i++)
                SYSTEM_SB[i]=temp_SYS_SB[i];
            }
            setSbFlag=0;
          }
          lcd.clear();
          lcd.setCursor(5,0);
          lcd.print("Set Ip");
          lcd.setCursor(2,1);
          lcd.print(">Set Subnet<");
          idleFlag=0;
          break;

        case 3:

          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("Set Subnet");
          lcd.setCursor(5,1);
          lcd.print(">back<");
          idleFlag=0;
          break;
      }
      delay(1);
    }
  }
}



boolean confirm(byte *iP){
  boolean rightState=0;
  boolean leftState=0;
  boolean enterState=0;

  boolean idleFlag=1;
  boolean YoN=0;
  while(1==1){
    while(digitalRead(enterButton)) enterState=1;
    while(digitalRead(leftButton)) leftState=1;
    while(digitalRead(rightButton)) rightState=1;

    if(leftState==1){
      leftState=0;
      if(YoN)
        YoN=0;
      else
        YoN=1;
      idleFlag=1;

    }else if(rightState==1){
      rightState=0;
      if(YoN)
        YoN=0;
      else
        YoN=1;
      idleFlag=1;

    }else if(enterState==1){
      enterState=0;
      if(YoN)
        return true;
      else{
        return false;
      }
    }

    if(idleFlag==1){
      if(YoN){
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print(getIp(iP));
        lcd.setCursor(0,1);
        lcd.print("Confirm?  Y >N");
        idleFlag=0;
      }else{
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print(getIp(iP));
        lcd.setCursor(0,1);
        lcd.print("Confirm? >Y  N");
        idleFlag=0;
      }
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
  byte boxNo=dat.substring((dat.indexOf(':')+2),(dat.indexOf(',')-1)).toInt();

  return ReminderA(new DateTime(0,0,0,H,M,0),&boxNo,&id, false);
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
  return ReminderB(new DateTime(0, 0, 0, H, M, 0), &boxesString, &suc);
}



String getIp(byte *a){
  String temp;
  for(int i=0;i<4;i++){
    temp+=String(a[i]);
    if(i!=3)
      temp+='.';
  }
  return temp;
}



String getIpBig(byte *a){
  String temp;
  for(int i=0;i<12;i++){
    temp+=String(a[i]);
    if(i!=11 && (i+1)%3==0)
      temp+='.';
  }
  return temp;
}



void setIp(byte *temp_SYS_IP){
  byte arrow[8]={
    0b00100,
    0b00100,
    0b01010,
    0b01010,
    0b10001,
    0b10001,
    0b00000,
    0b00000
  };
  byte blank[8]={
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };
  lcd.createChar(0, arrow);
  lcd.createChar(1, blank);

  byte temp_IP[12];
  smallToBig(temp_SYS_IP,temp_IP);
  Serial.println(getIpBig(temp_IP));

  boolean runFlag=1;
  boolean idleFlag=1;
  byte curPos=0;
  unsigned long blinkTime=0;
  boolean curStat=0;
  byte dataPos=0;

  boolean enterState=0;
  boolean upState=0;
  boolean downState=0;
  boolean leftState=0;
  boolean rightState=0;

  while(runFlag==1){
    while(digitalRead(enterButton)) enterState=1;
    while(digitalRead(upButton))    upState=1;
    while(digitalRead(downButton))  downState=1;
    while(digitalRead(leftButton))    leftState=1;
    while(digitalRead(rightButton))  rightState=1;

    if(enterState==1){
      enterState=0;
      runFlag=0;
      idleFlag=0;
    }else if(rightState==1){
      lcd.setCursor(curPos, 1);
      lcd.write(1);

      if(curPos>=14){
        curPos=0;
        dataPos=0;
      }else if((curPos+2)%4==0){
        curPos+=2;
        dataPos++;
      }else{
        curPos++;
        dataPos++;
      }

      rightState=0;
    }else if(leftState==1){
      lcd.setCursor(curPos, 1);
      lcd.write(1);

      if(curPos<=0){
        curPos=14;
        dataPos=12;
      }else if((curPos+4)%4==0){
        curPos-=2;
        dataPos--;
      }else{
        curPos--;
        dataPos--;
      }

      leftState=0;
    }
    else if(upState==1){
      if(temp_IP[dataPos]>=9)
        temp_IP[dataPos]=0;
      else
        temp_IP[dataPos]++;

      idleFlag=1;
      upState=0;
    }else if(downState==1){
      if(temp_IP[dataPos]<=0)
        temp_IP[dataPos]=9;
      else
        temp_IP[dataPos]--;

      idleFlag=1;
      downState=0;
    }

    if(millis()-blinkTime>=500){
      if(curStat==0){
        curStat=1;
        lcd.setCursor(curPos, 1);
        lcd.write(0);
      }else{
        curStat=0;
        lcd.setCursor(curPos, 1);
        lcd.write(1);
      }
      blinkTime=millis();
    }

    if(idleFlag==1){
      lcd.clear();
      byte cursor=0;

      for(int i=0;i<12;i++){
        if(i>0 && i%3==0){
          cursor++;
          lcd.setCursor(cursor,0);
          lcd.print('.');
        }

        if(i!=0)
          cursor++;

        lcd.setCursor(cursor,0);
        lcd.print(String(temp_IP[i]));
      }
      idleFlag=0;
    }
  }

  bigToSmall(temp_SYS_IP,temp_IP);
  Serial.println(getIp(temp_SYS_IP));
}



// Converts 12 byte IP into 4 Byte
// STILL TEST
void bigToSmall(byte *temp_SYS_IP,byte *bigIp){

  String temp;
  byte i=0;
  byte count=0;
  byte pass=0;
  while(i<=12){
    temp+=String(bigIp[i]);
    count++;

      if(count==3){
        temp_SYS_IP[pass]=temp.toInt();
        Serial.println(temp.toInt());
        pass++;
        count=0;
        temp="";
      }
    i++;
  }
}



// Converts 4 byte IP into 12 Byte
// STILL TEST
void smallToBig(byte *temp_SYS_IP, byte *bigIp){

  byte bigIpCounter=0;
  for(int i=0;i<4;i++){
    String temp;
    temp=String(temp_SYS_IP[i]);

    for(int k=temp.length();(k-3)!=0;k++){
      bigIp[bigIpCounter]=0;
      bigIpCounter++;
    }
    for(unsigned int j=0;j<temp.length();j++){
      bigIp[bigIpCounter]=temp.charAt(j)-'0';
      bigIpCounter++;
    }
  }

}



//GETS RID OF WHITE ZEROES
// FOR EXAMPLE
// 192.003.045.200 WILL BE CONVERTED INTO
//192.3.45.200
String ipSort(String ip){

  String sortedIp;
  for(int i=0;i<=14;i++){

    if(ip.charAt(i)!='0'&& ip.charAt(i)!='.'){
      for(int k=i; (k+1)%4!=0; k++){
        sortedIp+=ip.charAt(k);
        i=k;
      }
      if(!(i>=14)) sortedIp+='.';

    }
  }
  return sortedIp;

}



void beepFor(int time){
  digitalWrite(beeper,HIGH);
  delay(time);
  digitalWrite(beeper,LOW);
}



void offsetCoordinate(char cordinate, byte offsetPosition, int offSetBy, boolean applyToAll=true){
  if(cordinate=='X'){
    if(applyToAll==false){
      xCordinate[offsetPosition]=xCordinate[offsetPosition]+offSetBy;
      return;
    }


  }else if(cordinate=='Y'){
    if(applyToAll==false){
      yCordinate[offsetPosition]=yCordinate[offsetPosition]+offSetBy;
      return;
    }
  }else if (cordinate=='Z'){
    if(applyToAll==false){
      zCordinate[offsetPosition]=zCordinate[offsetPosition]+offSetBy;
      return;
    }
  }
}



void stepperState(byte enable, boolean state){
  if(enable==32) digitalWrite(enable, state);
  else digitalWrite(enable, !state);
}


unsigned long blinkPrevioustime=0;
bool blinkStat=0;
void blink(byte boxNo,char color){
  if((millis()-blinkPrevioustime)>500){
    blinkPrevioustime=millis();
    if(blinkStat)
      setColor(boxNo, color);
    else
      setColor(boxNo, 'c');
    blinkStat=!blinkStat;
  }
}



boolean unlockBox(byte boxNo){
  if(boxNo<0 || boxNo>15)
    return 0;

  const float stepperXSpeed=stepperX.speed();
  const float stepperYSpeed=stepperY.speed();
  const float stepperZSpeed=stepperZ.speed();

  MultiStepper steppers;
  steppers.addStepper(stepperX);
  steppers.addStepper(stepperY);

  stepperState(xEnable, true);
  stepperState(yEnable, true);
  stepperState(zEnable, true);
  lcdClear(1);
  lcd.print("BoxNo: "+String(boxNo));

  xAxis=xCordinate[boxNo];
  yAxis=yCordinate[boxNo];
  long positions[2]={(long)xAxis, (long)yAxis};
  steppers.moveTo(positions);
  while(steppers.run()){
    blink(boxNo, 'g');
  }

  zAxis=zCordinate[boxNo];//extend Arm
  stepperZ.moveTo(zAxis);
  stepperZ.setSpeed(8000);
  while(stepperZ.distanceToGo()!=0){
    stepperZ.runSpeedToPosition();
    blink(boxNo, 'g');
  }

  yAxis=yAxis+5000;//unlock Box
  stepperY.moveTo(yAxis);
  stepperY.setSpeed(3999);
  while(stepperY.distanceToGo()!=0){
    stepperY.runSpeedToPosition();
    blink(boxNo, 'g');
  }

  zAxis=zAxis+7000;//push it out
  stepperZ.moveTo(zAxis);
  stepperZ.setSpeed(8000);
  while(stepperZ.distanceToGo()!=0){
    stepperZ.runSpeedToPosition();
    blink(boxNo, 'g');
  }

  yAxis=yCordinate[boxNo];
  zAxis=zCordinate[boxNo];
  stepperY.moveTo(yAxis);
  stepperZ.moveTo(zAxis);
  stepperY.setSpeed(3999);
  stepperZ.setSpeed(3500);
  while((stepperY.distanceToGo()!=0) && (stepperZ.distanceToGo()!=0)){
    stepperY.runSpeedToPosition();
    stepperZ.runSpeedToPosition();
    blink(boxNo, 'g');
  }
  setColor(boxNo, 'b');


  zAxis=0;//Return to position
  stepperZ.moveTo(zAxis);
  stepperZ.setSpeed(8000);
  while(stepperZ.distanceToGo()!=0)
    stepperZ.runSpeedToPosition();

  stepperX.setSpeed(stepperXSpeed);
  stepperY.setSpeed(stepperYSpeed);
  stepperZ.setSpeed(stepperZSpeed);

  return 1;
}



void unlockAllBox(){
  for(int i=0;i<16;i++){
    unlockBox(i);
  }
}



boolean resetPosition(byte axis){
  AccelStepper *stepper;
  byte limitSwitch;
  byte enable;
  float speed;
  float maxSpeed;
  char ch;

  if(axis==0){
    ch='X';
    stepper=&stepperX;
    limitSwitch=xLimitSwitch;
    enable=xEnable;
    speed=stepper->speed();
    maxSpeed=stepper->maxSpeed();
    stepper->setMaxSpeed(5000);
    stepper->setSpeed(-5000);
  }else if(axis==1){
    ch='Y';
    stepper=&stepperY;
    limitSwitch=yLimitSwitch;
    enable=yEnable;
    speed=stepper->speed();
    stepper->setSpeed(-8000);
  }else if(axis==2){
    ch='Z';
    stepper=&stepperZ;
    limitSwitch=zLimitSwitch;
    enable=zEnable;
    speed=stepper->speed();
    stepper->setSpeed(-12000);
  }else
    return 0;

  beepFor(500);
  lcdClear(0);
  lcd.setCursor(0,0);
  lcd.print(String(ch)+" Axis Reset");
  Serial.println("Reset Pos "+ String(ch));

  stepperState(enable, 1);
  while(digitalRead(limitSwitch)==LOW){
    stepper->runSpeed();
    if(digitalRead(limitSwitch)==HIGH){
      stepperState(enable, 0);
    }
  }

  if(axis==0){
    stepper->setSpeed(1000);
    stepper->setMaxSpeed(maxSpeed);
    xAxis=0;
  }else if(axis==1){
    stepper->setSpeed(1000);
    yAxis=0;
  }else if(axis==2){
    stepper->setSpeed(1000);
    zAxis=0;
  }

  stepperState(enable, 1);
  while(digitalRead(limitSwitch)==HIGH){
    stepper->runSpeed();
    if(digitalRead(limitSwitch)==LOW){
      stepperState(enable, 0);
    }
  }
  stepper->setCurrentPosition(0);
  stepper->setSpeed(speed);
  stepperState(enable, 0);
  lcdClear(0);
  lcd.setCursor(0,0);
  lcd.print(String(ch)+" Reset Done");
  beepFor(500);
  delay(500);
  beepFor(500);
  return 0;
}



void moveStepper(){
  Serial.println("Moving..");
  // lcdClear(0);
  // lcd.print("Moving..");

  // unsigned long positions[2]={xAxis,yAxis};

  stepperX.moveTo(xAxis);
  stepperY.moveTo(yAxis);
  stepperZ.moveTo(zAxis);

  stepperY.setSpeed(stepperY.maxSpeed());
  stepperZ.setSpeed(stepperZ.maxSpeed());

  stepperState(yEnable, 1);
  while(stepperY.distanceToGo()!=0){
    stepperY.runSpeedToPosition();
    if(digitalRead(yLimitSwitch)==HIGH){
      Serial.println("EMERGENCY STOP");
      resetPosition(1);
    }
  }

  stepperState(xEnable, 1);
  while(stepperX.distanceToGo()!=0){
    stepperX.run();
    if(digitalRead(xLimitSwitch)==HIGH){
      Serial.println("EMERGENCY STOP");
      resetPosition(0);
    }
  }

  stepperState(zEnable, 1);
  while(stepperZ.distanceToGo()!=0){
    stepperZ.runSpeedToPosition();
    if(digitalRead(zLimitSwitch)==HIGH){
      Serial.println("EMERGENCY STOP");
      resetPosition(2);
    }
  }
  stepperState(zEnable, 0);

  // beepFor(500);
  // delay(100);
  // beepFor(1000);

  Serial.println("[X: "+String(xAxis)+"]");
  Serial.println("[Y: "+String(yAxis)+"]");
  Serial.println("[Z: "+String(zAxis)+"]");

  Serial.println("Done");
  Serial.println();
}



void bringEmHome(){
  String message="STEPPER RESET";
  Serial.println(message);
  lcdClear(1);
  lcd.print(message);

  resetPosition(2);
  resetPosition(0);
  resetPosition(1);

  message="RESET DONE";
  lcdClear(1);
  lcd.print(message);
  Serial.println(message);
}



void setColor(byte boxNo, char color){  //color{r=red,g=green,b=blue,c=clear}
  int adr=0;
  int row=boxNo%4;
  int col=0;

  if(boxNo<8){
    adr=0;
    if(color=='r' || color=='c'){
      if(boxNo<4)
        col=0;
      else
        col=3;
    }else if(color=='b'){
      if(boxNo<4)
        col=1;
      else
        col=4;
    }else if(color=='g'){
      if(boxNo<4)
        col=2;
      else
        col=5;
    }

  }else if(boxNo>7){
    adr=1;
    if(color=='r' || color=='c'){
      if(boxNo<12)
        col=0;
      else
        col=3;
    }else if(color=='b' ){
      if(boxNo<12)
        col=1;
      else
        col=4;
    }else if(color=='g' ){
      if(boxNo<12)
        col=2;
      else
        col=5;
    }

  }

  for(byte j=0;j<6;j++){
    if(col>=0 && col<3){
      boxLed.setLed(adr, row, j, false);
      if(j==2) j=6;
    }else if(col>2 && col<6){
      if(j==0) j=3;
      boxLed.setLed(adr, row, j, false);
    }
  }
  if(color!='c'){
    boxLed.setLed(adr, row, col, true);
  }

}



void ledTestFunction(){
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



void lcdClear(byte col){
  lcd.setCursor(0, col);
  lcd.print("                ");
  lcd.setCursor(0, col);
}



bool isOpen(byte boxNo){
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
  for(int i=0;i<16;i++){
    openBoxes[i]=0;
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
      Serial.println("BOX "+String(i)+"\n\n");
      for(int j=0;j<16;j++){
        if(openBoxes[j]==0){
          openBoxes[j]=i+1;
          break;
        }
      }
      isOpen=true;
    }
    if(digitalRead(X)){
      Serial.println("BOX "+String(i+9)+"\n\n");
      for(int j=0;j<16;j++){
        if(openBoxes[j]==0){
          openBoxes[j]=i+9;
          break;
        }
      }
    }
    isOpen=true;
  }
  return isOpen;
}



void boxMarker(){
  byte currentBox=16;
  double steps=1000;
  lcdClear(0);
  unsigned long Xcord[16];
  unsigned long Ycord[16];
  unsigned long Zcord[16];

  for(int i=0;i<16;i++){
    Xcord[i]=0;
    Ycord[i]=0;
    Zcord[i]=0;
  }

  bool run=true;
  while(run){
    boolean change=false;

    while(Serial.available()){
      String ch=Serial.readStringUntil('\n');
      if(ch.toDouble()==0){
        run=false;
      }else if(ch.toDouble()<=16){
        currentBox=ch.toDouble();
        lcdClear(0);
        lcd.print("box: "+String(currentBox));
        Serial.println("Loaded Box: "+String(currentBox));
      }else{
        steps=ch.toDouble();
        lcdClear(1);
        lcd.print("stp: "+String(steps));
        Serial.println("Loaded Step: "+String(steps));
      }
    }

    bool enterPressed=false;
    while(digitalRead(enterButton)){
      enterPressed=true;
    }if(enterPressed){
      Xcord[currentBox-1]=xAxis;
      Ycord[currentBox-1]=yAxis;
      Zcord[currentBox-1]=zAxis;

      currentBox=currentBox-1;
      lcdClear(0);
      lcd.print("box: "+String(currentBox));

      Serial.print("XCoord= { \n");
      for(int i=0;i<16;i++){
        Serial.print(String(Xcord[i]) + " ");
        if((i+1)%4==0)
          Serial.print("\n\t");
      }
      Serial.println("\n}");

      Serial.print("YCoord= { \n");
      for(int i=0;i<16;i++){
        Serial.print(String(Ycord[i]) + " ");
        if((i+1)%4==0)
          Serial.print("\n\t");
      }
      Serial.println("\n}");

      Serial.print("ZCoord= { \n");
      for(int i=0;i<16;i++){
        Serial.print(String(Zcord[i]) + " ");
        if((i+1)%4==0)
          Serial.print("\n\t");
      }
      Serial.println("\n}\n\n");
    }


    while(digitalRead(rightButton)==HIGH){
      delay(buttonDelay);
      if(!change)
        xAxis=xAxis+steps;
      change=true;
    }while(digitalRead(leftButton)==HIGH){
      delay(buttonDelay);
      if(!change){
        if(xAxis<=steps)
          xAxis=0;
        else
          xAxis=xAxis-steps;
      }
      change=true;
    }


    while(digitalRead(upButton)==HIGH){
      delay(buttonDelay);
      if(!change){
        yAxis=yAxis+steps;
      }
      change=true;
    }while(digitalRead(downButton)==HIGH){
      delay(buttonDelay);
      if(!change){
        if(yAxis<=steps)
          yAxis=0;
        else
          yAxis=yAxis-steps;
      }
      change=true;
    }


    while(digitalRead(frontButton)==HIGH){
      delay(buttonDelay);
      if(!change)
        zAxis=zAxis+steps;
      change=true;
    }while(digitalRead(backButton)==HIGH){
      delay(buttonDelay);
      if(!change){
        if(zAxis<=steps)
          zAxis=0;
        else
          zAxis=zAxis-steps;
      }
      change=true;
    }

    if(change){
      moveStepper();
    }
  }

  Serial.print("XCoord= { \n");
  for(int i=0;i<16;i++){
    Serial.print(String(Xcord[i]) + " ");
    if((i+1)%4==0)
      Serial.print("\n\t");
  }
  Serial.println("\n}");

  Serial.print("YCoord= { \n");
  for(int i=0;i<16;i++){
    Serial.print(String(Ycord[i]) + " ");
    if((i+1)%4==0)
      Serial.print("\n\t");;
  }
  Serial.println("\n}");

  Serial.print("ZCoord= { \n");
  for(int i=0;i<16;i++){
    Serial.print(String(Zcord[i]) + " ");
    if((i+1)%4==0)
      Serial.print("\n\t");
  }
  Serial.println("\n}\n\n");
}




