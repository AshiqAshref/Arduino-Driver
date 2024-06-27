#include "Mech_Arm.h"
#include <MultiStepper.h>

Mech_Arm::Mech_Arm(const byte & X_step_pin, const byte & X_dir_pin, const byte & xEnable_, const byte & xLimitSwitch_,
             const byte & Y_step_pin, const byte & Y_dir_pin, const byte & yEnable_, const byte & yLimitSwitch_,
             const byte & Z_step_pin, const byte & Z_dir_pin, const byte & zEnable_, const byte & zLimitSwitch_,
             const byte & beeper_pin_, Lcd_Menu *lcd_menu_, Led_Indicator * led_indicator_
        ) {
    stepperX = new AccelStepper(1, X_step_pin, X_dir_pin);
    stepperY = new AccelStepper(1, Y_step_pin, Y_dir_pin);
    stepperZ = new AccelStepper(1, Z_step_pin, Z_dir_pin);
    lcd_menu = lcd_menu_;
    led_indicator = led_indicator_;


    xEnable = xEnable_;
    yEnable = yEnable_;
    zEnable = zEnable_;
    beeper_pin = beeper_pin_;

    xLimitSwitch = xLimitSwitch_;
    yLimitSwitch = yLimitSwitch_;
    zLimitSwitch = zLimitSwitch_;

    initializeSteppers();
}

void Mech_Arm::initializeSteppers() const {
    pinMode(xEnable, OUTPUT);
    pinMode(yEnable, OUTPUT);
    pinMode(zEnable, OUTPUT);
    pinMode(beeper_pin, OUTPUT);

    pinMode(enterButton,INPUT);
    pinMode(leftButton,INPUT);
    pinMode(rightButton,INPUT);
    pinMode(upButton,INPUT);
    pinMode(downButton,INPUT);

    stepperX->setEnablePin(xEnable);
    stepperY->setEnablePin(yEnable);
    stepperZ->setEnablePin(zEnable);

    stepperX->setAcceleration(3500);
    stepperY->setAcceleration(1500);
    stepperZ->setAcceleration(8000);

    stepperX->setMaxSpeed(2500);
    stepperY->setMaxSpeed(8000);
    stepperZ->setMaxSpeed(12000);

    stepperX->setSpeed(2500);
    stepperY->setSpeed(3999);
    stepperZ->setSpeed(8000);

    stepperState(xEnable, false);
    stepperState(yEnable, false);
    stepperState(zEnable, false);
}


void Mech_Arm::moveStepper(){
    Serial.println("Moving..");
    lcd_menu->lcdClear(0);
    lcd_menu->getLcd().print("Moving..");

    // unsigned long positions[2]={xAxis,yAxis};
    stepperX->moveTo(static_cast<long>(xAxis));
    stepperY->moveTo(static_cast<long>(yAxis));
    stepperZ->moveTo(static_cast<long>(zAxis));

    stepperY->setSpeed(stepperY->maxSpeed());
    stepperZ->setSpeed(stepperZ->maxSpeed());

    stepperState(yEnable, true);
    while(stepperY->distanceToGo()!=0){
        stepperY->runSpeedToPosition();
        if(digitalRead(yLimitSwitch)==HIGH){
            Serial.println("EMERGENCY STOP");
            resetPosition(1);
        }
    }

    stepperState(xEnable, true);
    while(stepperX->distanceToGo()!=0){
        stepperX->run();
        if(digitalRead(xLimitSwitch)==HIGH){
            Serial.println("EMERGENCY STOP");
            resetPosition(0);
        }
    }

    stepperState(zEnable, true);
    while(stepperZ->distanceToGo()!=0){
        stepperZ->runSpeedToPosition();
        if(digitalRead(zLimitSwitch)==HIGH){
            Serial.println("EMERGENCY STOP");
            resetPosition(2);
        }
    }
    stepperState(zEnable, false);

    // beepFor(500);
    // delay(100);
    // beepFor(1000);

    Serial.println("[X: "+String(xAxis)+"]");
    Serial.println("[Y: "+String(yAxis)+"]");
    Serial.println("[Z: "+String(zAxis)+"]");

    Serial.println("Done");
    Serial.println();
}


boolean Mech_Arm::unlockBox(byte const boxNo){
    if(boxNo<0 || boxNo>15)
        return false;

    const float stepperXSpeed=stepperX->speed();
    const float stepperYSpeed=stepperY->speed();
    const float stepperZSpeed=stepperZ->speed();

    MultiStepper steppers;
    steppers.addStepper(*stepperX);
    steppers.addStepper(*stepperY);

    stepperState(xEnable, true);
    stepperState(yEnable, true);
    stepperState(zEnable, true);
    lcd_menu->lcdClear(1);
    lcd_menu->getLcd().print("BoxNo: "+String(boxNo));

    xAxis=xCordinate[boxNo];
    yAxis=yCordinate[boxNo];
    long positions[2]={static_cast<long>(xAxis), static_cast<long>(yAxis)};
    steppers.moveTo(positions);
    while(steppers.run()){
        led_indicator->blink(boxNo, 'g');
    }
    zAxis=zCordinate[boxNo];//extend Arm
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperZ->setSpeed(8000);
    while(stepperZ->distanceToGo()!=0){
        stepperZ->runSpeedToPosition();
        led_indicator->blink(boxNo, 'g');
    }
    yAxis=yAxis+5000;//unlock Box
    stepperY->moveTo(static_cast<long>(yAxis));
    stepperY->setSpeed(3999);
    while(stepperY->distanceToGo()!=0){
        stepperY->runSpeedToPosition();
        led_indicator->blink(boxNo, 'g');
    }

    zAxis=zAxis+7000;//push it out
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperZ->setSpeed(8000);
    while(stepperZ->distanceToGo()!=0){
        stepperZ->runSpeedToPosition();
        led_indicator->blink(boxNo, 'g');
    }

    yAxis=yCordinate[boxNo];
    zAxis=zCordinate[boxNo];
    stepperY->moveTo(static_cast<long>(yAxis));
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperY->setSpeed(3999);
    stepperZ->setSpeed(3500);
    while((stepperY->distanceToGo()!=0) && (stepperZ->distanceToGo()!=0)){
        stepperY->runSpeedToPosition();
        stepperZ->runSpeedToPosition();
        led_indicator->blink(boxNo, 'g');
    }
    led_indicator->setColor(boxNo, 'b');

    zAxis=0;//Return to position
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperZ->setSpeed(8000);
    while(stepperZ->distanceToGo()!=0)
        stepperZ->runSpeedToPosition();

    stepperX->setSpeed(stepperXSpeed);
    stepperY->setSpeed(stepperYSpeed);
    stepperZ->setSpeed(stepperZSpeed);

    return true;
}


void Mech_Arm::unlockAllBox(){
    for(int i=0;i<16;i++) unlockBox(i);
}


void Mech_Arm::bringEmHome(){
    String message="STEPPER RESET";
    Serial.println(message);
    lcd_menu->lcdClear(1);
    lcd_menu->getLcd().print(message);

    resetPosition(2);
    resetPosition(0);
    resetPosition(1);

    message="RESET DONE";
    lcd_menu->lcdClear(1);
    lcd_menu->getLcd().print(message);
    Serial.println(message);
}


boolean Mech_Arm::resetPosition(byte const axis){
    AccelStepper stepper;
    byte limitSwitch;
    byte enable;
    float speed;
    float maxSpeed= 0;
    char ch;
    if(axis==0){
        ch='X';
        stepper=*stepperX;
        limitSwitch=xLimitSwitch;
        enable=xEnable;
        speed=stepper.speed();
        maxSpeed=stepper.maxSpeed();
        stepper.setMaxSpeed(5000);
        stepper.setSpeed(-5000);
    }else if(axis==1){
        ch='Y';
        stepper=*stepperY;
        limitSwitch=yLimitSwitch;
        enable=yEnable;
        speed=stepper.speed();
        stepper.setSpeed(-8000);
    }else if(axis==2){
        ch='Z';
        stepper=*stepperZ;
        limitSwitch=zLimitSwitch;
        enable=zEnable;
        speed=stepper.speed();
        stepper.setSpeed(-12000);
    }else
        return false;

    beepFor(500);
    lcd_menu->lcdClear(0);
    lcd_menu->getLcd().setCursor(0,0);
    lcd_menu->getLcd().print(String(ch)+" Axis Reset");
    Serial.println("Reset Pos "+ String(ch));

    stepperState(enable, true);
    while(digitalRead(limitSwitch)==LOW){
        stepper.runSpeed();
        if(digitalRead(limitSwitch)==HIGH){
            stepperState(enable, false);
        }
    }

    if(axis==0){
        stepper.setSpeed(1000);
        stepper.setMaxSpeed(maxSpeed);
        xAxis=0;
    }else if(axis==1){
        stepper.setSpeed(1000);
        yAxis=0;
    }else if(axis==2){
        stepper.setSpeed(1000);
        zAxis=0;
    }

    stepperState(enable, true);
    while(digitalRead(limitSwitch)==HIGH){
        stepper.runSpeed();
        if(digitalRead(limitSwitch)==LOW){
            stepperState(enable, false);
        }
    }
    stepper.setCurrentPosition(0);
    stepper.setSpeed(speed);
    stepperState(enable, false);
    lcd_menu->lcdClear(0);
    lcd_menu->getLcd().setCursor(0,0);
    lcd_menu->getLcd().print(String(ch)+" Reset Done");
    beepFor(500);
    delay(500);
    beepFor(500);
    return true;
}


void Mech_Arm::beepFor(const int time) const{
    digitalWrite(beeper_pin,HIGH);
    delay(time);
    digitalWrite(beeper_pin,LOW);
}


void Mech_Arm::boxMarker(){
  double steps=1000;
  lcd_menu->lcdClear(0);
  unsigned long Xcord[16];
  unsigned long Ycord[16];
  unsigned long Zcord[16];

  for(int i=0;i<16;i++){
    Xcord[i]=0;
    Ycord[i]=0;
    Zcord[i]=0;
  }

  long currentBox=16;
  bool run=true;
  while(run){
    boolean change=false;
    while(Serial.available()){
      String ch=Serial.readStringUntil('\n');
      if(ch.toDouble()==0){
        run=false;
      }else if(ch.toDouble()<=16){
        currentBox=ch.toInt();
        lcd_menu->lcdClear(0);
        lcd_menu->getLcd().print("box: "+String(currentBox));
        Serial.println("Loaded Box: "+String(currentBox));
      }else{
        steps=ch.toDouble();
        lcd_menu->lcdClear(1);
        lcd_menu->getLcd().print("stp: "+String(steps));
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
      lcd_menu->lcdClear(0);
      lcd_menu->getLcd().print("box: "+String(currentBox));

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
        xAxis=xAxis+static_cast<long>(steps);
      change=true;
    }while(digitalRead(leftButton)==HIGH){
      delay(buttonDelay);
      if(!change){
        if(static_cast<double>(xAxis)<=steps)
          xAxis=0;
        else
          xAxis= xAxis - static_cast<unsigned long>(steps);
      }
      change=true;
    }


    while(digitalRead(upButton)==HIGH){
      delay(buttonDelay);
      if(!change){
        yAxis= yAxis + static_cast<unsigned long>(steps);
      }
      change=true;
    }while(digitalRead(downButton)==HIGH){
      delay(buttonDelay);
      if(!change){
        if(static_cast<double>(yAxis)<=steps)
          yAxis=0;
        else
          yAxis=yAxis-static_cast<long>(steps);
      }
      change=true;
    }


    while(digitalRead(frontButton)==HIGH){
      delay(buttonDelay);
      if(!change)
        zAxis=zAxis+static_cast<long>(steps);
      change=true;
    }while(digitalRead(backButton)==HIGH){
      delay(buttonDelay);
      if(!change){
        if(static_cast<double>(zAxis)<=steps)
          zAxis=0;
        else
          zAxis=zAxis-static_cast<long>(steps);
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


void Mech_Arm::stepperState(const byte enable, const boolean state){
    if(enable==32) digitalWrite(enable, state);
    else digitalWrite(enable, !state);
}


void Mech_Arm::offsetCoordinate(char const cordinate, byte const offsetPosition, int const offSetBy, boolean const applyToAll=true){
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


