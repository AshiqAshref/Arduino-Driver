#include "Mech_Arm.h"

#include <AV_Functions.h>
#include <MultiStepper.h>
#include <BUTTON_PINS.h>
#include <STEPPER_PINS.h>
// Z=1/2 step

extern Led_Indicator led_indicator;
extern LiquidCrystal_I2C lcd;
extern Lcd_Menu lcd_menu;
float xSpeed=1000;
float ySpeed=1000;
float zSpeed=1000;

Mech_Arm::Mech_Arm() {
// Mech_Arm::Mech_Arm(const Lcd_Menu &lcd_menu, Led_Indicator *led_indicator) {
    stepperX = new AccelStepper(1, static_cast<uint8_t>(STEPPER_PINS::X_step_pin), static_cast<uint8_t>(STEPPER_PINS::X_dir_pin));
    stepperY = new AccelStepper(1, static_cast<uint8_t>(STEPPER_PINS::Y_step_pin), static_cast<uint8_t>(STEPPER_PINS::Y_dir_pin));
    stepperZ = new AccelStepper(1, static_cast<uint8_t>(STEPPER_PINS::Z_step_pin), static_cast<uint8_t>(STEPPER_PINS::Z_dir_pin));

    // this->lcd_menu = lcd_menu;
    // this->led_indicator = led_indicator;
    initializeSteppers();
}


void Mech_Arm::initializeSteppers() const {
    pinMode(static_cast<uint8_t>(STEPPER_PINS::xEnable), OUTPUT);
    pinMode(static_cast<uint8_t>(STEPPER_PINS::yEnable), OUTPUT);
    pinMode(static_cast<uint8_t>(STEPPER_PINS::zEnable), OUTPUT);
    pinMode(static_cast<uint8_t>(STEPPER_PINS::zEnable), OUTPUT);

    pinMode(static_cast<uint8_t>(BUTTON_PINS::enterButton),INPUT);
    pinMode(static_cast<uint8_t>(BUTTON_PINS::leftButton),INPUT);
    pinMode(static_cast<uint8_t>(BUTTON_PINS::rightButton),INPUT);
    pinMode(static_cast<uint8_t>(BUTTON_PINS::upButton),INPUT);
    pinMode(static_cast<uint8_t>(BUTTON_PINS::downButton),INPUT);

    // stepperX->setEnablePin(static_cast<uint8_t>(STEPPER_PINS::xEnable));
    // stepperY->setEnablePin(static_cast<uint8_t>(STEPPER_PINS::yEnable));
    // stepperZ->setEnablePin(static_cast<uint8_t>(STEPPER_PINS::zEnable));

    stepperX->setAcceleration(3500);
    
    stepperX->setMaxSpeed(9000);
    stepperY->setMaxSpeed(1600);
    stepperZ->setMaxSpeed(1600);

    stepperX->setSpeed(2500);
    stepperY->setSpeed(1600);
    stepperZ->setSpeed(1600);

    stepperState(static_cast<byte>(STEPPER_PINS::xEnable), false);
    stepperState(static_cast<byte>(STEPPER_PINS::yEnable), false);
    stepperState(static_cast<byte>(STEPPER_PINS::zEnable), false);
}


void Mech_Arm::moveStepper(){
    Serial.println("Moving..");
    lcd_menu.lcdClear(0);
    lcd.print("Moving..");

    // unsigned long positions[2]={xAxis,yAxis};
    stepperX->moveTo(static_cast<long>(xAxis));
    stepperY->moveTo(static_cast<long>(yAxis));
    stepperZ->moveTo(static_cast<long>(zAxis));

    stepperY->setSpeed(ySpeed);
    stepperZ->setSpeed(zSpeed);

    stepperState(static_cast<byte>(STEPPER_PINS::yEnable), true);
    while(stepperY->distanceToGo()!=0){
        stepperY->runSpeedToPosition();
        if(digitalRead(static_cast<uint8_t>(STEPPER_PINS::yLimitSwitch))==HIGH){
            Serial.println("EMERGENCY STOP");
            resetPosition(1);
        }
    }

    stepperState(static_cast<byte>(STEPPER_PINS::xEnable), true);
    while(stepperX->distanceToGo()!=0){
        stepperX->run();
        if(digitalRead(static_cast<uint8_t>(STEPPER_PINS::xLimitSwitch))==HIGH){
            Serial.println("EMERGENCY STOP");
            resetPosition(0);
        }
    }

    stepperState(static_cast<byte>(STEPPER_PINS::zEnable), true);
    while(stepperZ->distanceToGo()!=0){
        stepperZ->runSpeedToPosition();
        if(digitalRead(static_cast<uint8_t>(STEPPER_PINS::zLimitSwitch))==HIGH){
            Serial.println("EMERGENCY STOP");
            resetPosition(2);
        }
    }
    stepperState(static_cast<byte>(STEPPER_PINS::zEnable), false);

    Serial.println("[X: "+static_cast<String>(xAxis)+"]");
    Serial.println("[Y: "+static_cast<String>(yAxis)+"]");
    Serial.println("[Z: "+static_cast<String>(zAxis)+"]");

    lcd.setCursor(0,0);
    lcd.print("Done....");
    Serial.println("Done");
    Serial.println();
}


boolean Mech_Arm::unlockBox(byte const boxNo){
    if(boxNo<1 || boxNo>16)
        return false;
    const float stepperXSpeed=stepperX->speed();
    const float stepperYSpeed=stepperY->speed();
    const float stepperZSpeed=stepperZ->speed();

    MultiStepper steppers;
    steppers.addStepper(*stepperX);
    steppers.addStepper(*stepperY);

    stepperState(static_cast<byte>(STEPPER_PINS::xEnable), true);
    stepperState(static_cast<byte>(STEPPER_PINS::yEnable), true);
    stepperState(static_cast<byte>(STEPPER_PINS::zEnable), true);
    lcd_menu.lcdClear(1);
    lcd.print("BoxNo: "+static_cast<String>(boxNo));

    xAxis=xCordinate[boxNo-1];
    yAxis=yCordinate[boxNo-1];
    long positions[2]={static_cast<long>(xAxis), static_cast<long>(yAxis)};
    steppers.moveTo(positions);
    while(steppers.run()){
        led_indicator.blink(boxNo, COLOR::GREEN);
    }
    zAxis=zCordinate[boxNo];//extend Arm
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperZ->setSpeed(zSpeed);
    while(stepperZ->distanceToGo()!=0){
        stepperZ->runSpeedToPosition();
        led_indicator.blink(boxNo, COLOR::GREEN);
    }
    yAxis=yAxis+5000;//unlock Box
    stepperY->moveTo(static_cast<long>(yAxis));
    stepperY->setSpeed(ySpeed);
    while(stepperY->distanceToGo()!=0){
        stepperY->runSpeedToPosition();
        led_indicator.blink(boxNo, COLOR::GREEN);
    }

    zAxis=zAxis+7000;//push it out
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperZ->setSpeed(zSpeed);
    while(stepperZ->distanceToGo()!=0){
        stepperZ->runSpeedToPosition();
        led_indicator.blink(boxNo, COLOR::GREEN);
    }

    yAxis=yCordinate[boxNo];
    zAxis=zCordinate[boxNo];
    stepperY->moveTo(static_cast<long>(yAxis));
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperY->setSpeed(ySpeed);
    stepperZ->setSpeed(zSpeed);
    while((stepperY->distanceToGo()!=0) && (stepperZ->distanceToGo()!=0)){
        stepperY->runSpeedToPosition();
        stepperZ->runSpeedToPosition();
        led_indicator.blink(boxNo, COLOR::GREEN);
    }
    led_indicator.setColor(boxNo, COLOR::BLUE);

    zAxis=0;//Return to position
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperZ->setSpeed(zSpeed);
    while(stepperZ->distanceToGo()!=0)
        stepperZ->runSpeedToPosition();

    stepperX->setSpeed(stepperXSpeed);
    stepperY->setSpeed(stepperYSpeed);
    stepperZ->setSpeed(stepperZSpeed);

    return true;
}


void Mech_Arm::unlockAllBox(){
    for(int i=1;i<=16;i++) unlockBox(i);
}


void Mech_Arm::bringEmHome(){
    String message="STEPPER RESET";
    Serial.println(message);
    lcd_menu.lcdClear(1);
    lcd.print(message);

    resetPosition(2);
    resetPosition(0);
    resetPosition(1);

    message="RESET DONE";
    lcd_menu.lcdClear(1);
    lcd.print(message);
    Serial.println(message);
}


boolean Mech_Arm::resetPosition(byte const axis){
    AccelStepper stepper;
    byte limitSwitch;
    byte enable;
    float speed;
    char ch;
    if(axis==0){
        ch='X';
        stepper=*stepperX;
        limitSwitch= static_cast<byte>(STEPPER_PINS::xLimitSwitch);
        enable= static_cast<byte>(STEPPER_PINS::xEnable);
        speed=stepper.speed();
        stepper.setSpeed(-500);
    }else if(axis==1){
        ch='Y';
        stepper=*stepperY;
        limitSwitch= static_cast<byte>(STEPPER_PINS::yLimitSwitch);
        enable= static_cast<byte>(STEPPER_PINS::yEnable);
        speed=stepper.speed();
        stepper.setSpeed(-800);
    }else if(axis==2){
        ch='Z';
        stepper=*stepperZ;
        limitSwitch= static_cast<byte>(STEPPER_PINS::zLimitSwitch);
        enable= static_cast<byte>(STEPPER_PINS::zEnable);
        speed=stepper.speed();
        stepper.setSpeed(-1200);
    }else
        return false;

    // AV_Functions::beepFor(500);
    lcd_menu.lcdClear(0);
    lcd.setCursor(0,0);
    lcd.print(static_cast<String>(ch)+" Axis Reset");
    Serial.println("Reset Pos "+ static_cast<String>(ch));

    stepperState(enable, true);
    while(digitalRead(limitSwitch)==LOW){
        stepper.runSpeed();
        if(digitalRead(limitSwitch)==HIGH){
            stepperState(enable, false);
        }
    }

    if(axis==0){
        stepper.setSpeed(1000);
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
    lcd_menu.lcdClear(0);
    lcd.setCursor(0,0);
    lcd.print(static_cast<String>(ch)+" Reset Done");
    AV_Functions::beepFor(500);
    delay(500);
    return true;
}



void Mech_Arm::boxMarker(){
    double steps=1000;
    lcd_menu.lcdClear(1);
    lcd.print("Marker");
    Serial.println("Marker");

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
        led_indicator.blink(currentBox,COLOR::RED);
        while(Serial.available()) {
            String ch=Serial.readStringUntil('\n');
            if(ch.charAt(0)=='N') {
                run=false;
            }else if(ch.charAt(0)=='r'||ch.charAt(0)=='R') {
                resetPosition(2);
                resetPosition(0);
                resetPosition(1);
            }else if(ch.charAt(0)=='t'||ch.charAt(0)=='T') {
                Serial.print("\nTurning on ");
                if(ch.charAt(1)=='x'||ch.charAt(1)=='X') {
                    Serial.print('x');
                    stepperState(static_cast<byte>(STEPPER_PINS::xEnable), true);
                    AV_Functions::waitForInput();
                    stepperState(static_cast<byte>(STEPPER_PINS::xEnable), false);
                    digitalWrite(static_cast<uint8_t>(STEPPER_PINS::X_step_pin),LOW);
                }else if(ch.charAt(1)=='y'||ch.charAt(1)=='Y') {
                    Serial.print('y');
                    stepperState(static_cast<byte>(STEPPER_PINS::yEnable), true);
                    AV_Functions::waitForInput();
                    stepperState(static_cast<byte>(STEPPER_PINS::yEnable), false);
                    digitalWrite(static_cast<uint8_t>(STEPPER_PINS::Y_step_pin),LOW);
                }else if(ch.charAt(1)=='z'||ch.charAt(1)=='Z') {
                    Serial.print('z');
                    stepperState(static_cast<byte>(STEPPER_PINS::zEnable), true);
                    AV_Functions::waitForInput();
                    stepperState(static_cast<byte>(STEPPER_PINS::zEnable), false);
                    digitalWrite(static_cast<uint8_t>(STEPPER_PINS::Z_step_pin),LOW);
                }
                Serial.println();
                Serial.println("off");
            }else if(ch.charAt(0)=='s'||ch.charAt(0)=='S') {
                Serial.print("\nEnter Speed For ");
                String enteredSpeed="10";
                if(ch.charAt(1)=='x'||ch.charAt(1)=='X') {
                    Serial.print("x from "+ static_cast<String>(stepperX->maxSpeed()) + " to : ");
                    while(!Serial.available()){}
                    while(Serial.available()) enteredSpeed = Serial.readStringUntil('\n');
                    xSpeed=enteredSpeed.toFloat();
                    stepperX->setMaxSpeed(xSpeed);
                    stepperX->setSpeed(xSpeed);
                }else if(ch.charAt(1)=='y'||ch.charAt(1)=='Y') {
                    Serial.print("y from "+ static_cast<String>(stepperY->speed()) + " to : ");
                    while(!Serial.available()){}
                    while(Serial.available()) enteredSpeed = Serial.readStringUntil('\n');
                    ySpeed=enteredSpeed.toFloat();
                    stepperY->setMaxSpeed(ySpeed);
                    stepperY->setSpeed(ySpeed);
                }else if(ch.charAt(1)=='z'||ch.charAt(1)=='Z') {
                    Serial.print("z from "+ static_cast<String>(stepperZ->speed()) + " to : ");
                    while(!Serial.available()){}
                    while(Serial.available()) enteredSpeed = Serial.readStringUntil('\n');
                    zSpeed =enteredSpeed.toFloat();
                    stepperZ->setMaxSpeed(zSpeed);
                    stepperZ->setSpeed(zSpeed);
                }
                Serial.println("\n Speed Set to : "+static_cast<String>(enteredSpeed.toDouble()));
            }else if(ch.toDouble()<=16 && ch.toDouble()>=1){
                led_indicator.setColor(currentBox,COLOR::BLUE);
                currentBox=ch.toInt();
                lcd_menu.lcdClear(0);
                lcd.print("box: "+static_cast<String>(currentBox));
                Serial.println("Loaded Box: "+static_cast<String>(currentBox));
            }else{
                steps=ch.toDouble();
                lcd_menu.lcdClear(1);
                lcd.print("stp: "+static_cast<String>(steps));
                Serial.println("Loaded Step: "+static_cast<String>(steps));
            }
        }

        bool enterPressed=false;
        while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::enterButton))){
            enterPressed=true;
        }if(enterPressed){
            Xcord[currentBox-1]=xAxis;
            Ycord[currentBox-1]=yAxis;
            Zcord[currentBox-1]=zAxis;
            if(zAxis>1000){ zAxis=zAxis-1000; change = true;}

            led_indicator.setColor(currentBox,COLOR::GREEN);
            currentBox=currentBox-1;
            if(currentBox<=1) currentBox=16;
            lcd_menu.lcdClear(0);
            lcd.print("box: "+ static_cast<String>(currentBox));

            Serial.println("XCoord= {");
            for(int i=0;i<16;i++){
                Serial.print(Xcord[i]);
                Serial.print(", ");
                if((i+1)%4==0)
                    Serial.println("\t");
            }
            Serial.println("\n}");

            Serial.println("YCoord= { ");
            for(int i=0;i<16;i++){
                Serial.print(Ycord[i]);
                Serial.print(", ");
                if((i+1)%4==0)
                    Serial.println("\t");
            }
            Serial.println("\n}");

            Serial.println("ZCoord= { ");
            for(int i=0;i<16;i++){
                Serial.print(Zcord[i]);
                Serial.print(", ");
                if((i+1)%4==0)
                    Serial.println("\t");
            }
            Serial.println("\n}\n\n");
        }

        while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::rightButton))==HIGH){
            delay(static_cast<uint8_t>(BUTTON_PINS::buttonDelay));
            if(!change)
                xAxis=xAxis+static_cast<long>(steps);
            change=true;
        }while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::leftButton))==HIGH){
            delay(static_cast<uint8_t>(BUTTON_PINS::buttonDelay));
            if(!change){
                if(static_cast<double>(xAxis)<=steps)
                    xAxis=0;
                else
                    xAxis= xAxis - static_cast<long>(steps);
            }
            change=true;
        }

        while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::upButton))==HIGH){
            delay(static_cast<uint8_t>(BUTTON_PINS::buttonDelay));
            if(!change){
                yAxis= yAxis + static_cast<unsigned long>(steps);
            }
            change=true;
        }while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::downButton))==HIGH){
            delay(static_cast<uint8_t>(BUTTON_PINS::buttonDelay));
            if(!change){
                if(static_cast<double>(yAxis)<=steps)
                    yAxis=0;
                else
                    yAxis=yAxis-static_cast<long>(steps);
            }
            change=true;
        }

        while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::frontButton))==HIGH){
            delay(static_cast<uint8_t>(BUTTON_PINS::buttonDelay));
            if(!change)
                zAxis=zAxis+static_cast<long>(steps);
            change=true;
        }while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::backButton))==HIGH){
            delay(static_cast<uint8_t>(BUTTON_PINS::buttonDelay));
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
        Serial.print(static_cast<String>(Xcord[i]) + " ");
        if((i+1)%4==0)
            Serial.print("\n\t");
    }
    Serial.println("\n}");

    Serial.print("YCoord= { \n");
    for(int i=0;i<16;i++){
        Serial.print(static_cast<String>(Ycord[i]) + " ");
        if((i+1)%4==0)
            Serial.print("\n\t");
    }
    Serial.println("\n}");

    Serial.print("ZCoord= { \n");
    for(int i=0;i<16;i++){
        Serial.print(static_cast<String>(Zcord[i]) + " ");
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


