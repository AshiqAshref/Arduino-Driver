#include "Mech_Arm.h"

#include <AV_Functions.h>
#include <MultiStepper.h>
#include <IO_PINS.h>
#include <LiquidCrystal_I2C.h>
#include <Led_Indicator.h>
#include <Lcd_Menu.h>
#include <Blink_Array.h>
#include <ModeB/Box.h>

// Z=1/2 step

extern Led_Indicator led_indicator;
extern LiquidCrystal_I2C lcd;
extern Lcd_Menu lcd_menu;
extern Blink_Array blink_array;

float xSpeed=1000;
float ySpeed=1000;
float zSpeed=1000;

Mech_Arm::Mech_Arm() {
    stepperX = new AccelStepper(1, STEPPER_PIN_XSTEP, STEPPER_PIN_XDIR);
    stepperY = new AccelStepper(1, STEPPER_PIN_YSTEP, STEPPER_PIN_YDIR);
    stepperZ = new AccelStepper(1, STEPPER_PIN_ZSTEP, STEPPER_PIN_ZDIR);

    initializeSteppers();
}


void Mech_Arm::initializeSteppers() const {
    pinMode(STEPPER_PIN_XENABLE, OUTPUT);
    pinMode(STEPPER_PIN_YENABLE, OUTPUT);
    pinMode(STEPPER_PIN_ZENABLE, OUTPUT);
    pinMode(STEPPER_PIN_ZENABLE, OUTPUT);

    pinMode(BUTTON_ENTER,INPUT);
    pinMode(BUTTON_LEFT,INPUT);
    pinMode(BUTTON_RIGHT,INPUT);
    pinMode(BUTTON_UP,INPUT);
    pinMode(BUTTON_DOWN,INPUT);

    // stepperX->setEnablePin(xEnable));
    // stepperY->setEnablePin(yEnable));
    // stepperZ->setEnablePin(zEnable));

    stepperX->setAcceleration(3500);
    
    stepperX->setMaxSpeed(9000);
    stepperY->setMaxSpeed(1600);
    stepperZ->setMaxSpeed(1600);

    stepperX->setSpeed(2500);
    stepperY->setSpeed(1600);
    stepperZ->setSpeed(1600);

    stepperState(STEPPER_PIN_XENABLE, false);
    stepperState(STEPPER_PIN_YENABLE, false);
    stepperState(STEPPER_PIN_ZENABLE, false);
}


void Mech_Arm::moveStepper(){
    Serial.println("Moving..");
    Lcd_Menu::lcdClear(0);
    lcd.print("Moving..");

    // unsigned long positions[2]={xAxis,yAxis};
    stepperX->moveTo(static_cast<long>(xAxis));
    stepperY->moveTo(static_cast<long>(yAxis));
    stepperZ->moveTo(static_cast<long>(zAxis));

    stepperY->setSpeed(ySpeed);
    stepperZ->setSpeed(zSpeed);

    stepperState(STEPPER_PIN_YENABLE, true);
    while(stepperY->distanceToGo()!=0){
        stepperY->runSpeedToPosition();
        if(digitalRead(STEPPER_PIN_YLIMIT_SW)==HIGH){
            Serial.println("EMERGENCY STOP");
            resetPosition(1);
        }
    }

    stepperState(STEPPER_PIN_XENABLE, true);
    while(stepperX->distanceToGo()!=0){
        stepperX->run();
        if(digitalRead(STEPPER_PIN_XLIMIT_SW)==HIGH){
            Serial.println("EMERGENCY STOP");
            resetPosition(0);
        }
    }

    stepperState(STEPPER_PIN_ZENABLE, true);
    while(stepperZ->distanceToGo()!=0){
        stepperZ->runSpeedToPosition();
        if(digitalRead(STEPPER_PIN_ZLIMIT_SW)==HIGH){
            Serial.println("EMERGENCY STOP");
            resetPosition(2);
        }
    }
    stepperState(STEPPER_PIN_ZENABLE, false);

    Serial.println("[X: "+static_cast<String>(xAxis)+"]");
    Serial.println("[Y: "+static_cast<String>(yAxis)+"]");
    Serial.println("[Z: "+static_cast<String>(zAxis)+"]");

    lcd.clear();
    Serial.println("Done");
    Serial.println();
}

boolean Mech_Arm::unlockBoxDummy(Box *box) {
    Serial.print("Unlocking box: ");
    Serial.print(box->box_no());
    box->unlocking(true);
    const uint32_t last_millis = millis();
    // while(millis()-last_millis<4000) {
    //     blink_array.blinkAll();
    // }
    box->unlocking(false);
    Serial.println(" DONE");

    return true;
}


extern Box boxes[];
boolean Mech_Arm::unlockBox(Box *box){
    return unlockBoxDummy(box); //TEST_ONLY
    box->unlocking(true);
    constexpr  uint16_t unlock_distance_step=1000;
    constexpr uint16_t push_box_distance_step=1000;

    const float stepperXSpeed=stepperX->speed();
    const float stepperYSpeed=stepperY->speed();
    const float stepperZSpeed=stepperZ->speed();

    MultiStepper steppers;
    steppers.addStepper(*stepperX);
    steppers.addStepper(*stepperY);

    stepperState(STEPPER_PIN_XENABLE, true);
    stepperState(STEPPER_PIN_YENABLE, true);
    stepperState(STEPPER_PIN_ZENABLE, true);
    Lcd_Menu::lcdClear(1);
    lcd.print("BoxNo: "+static_cast<String>(box->box_no()));

    xAxis=box->coordinate().x();
    yAxis=box->coordinate().y();
    long positions[2]={static_cast<long>(xAxis), static_cast<long>(yAxis)};


    steppers.moveTo(positions);
    while(steppers.run()){
        blink_array.blinkAll();
    }
    zAxis=box->coordinate().z();
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperZ->setSpeed(zSpeed);
    while(stepperZ->distanceToGo()!=0){
        stepperZ->runSpeedToPosition();
        blink_array.blinkAll();

    }
    yAxis=yAxis+unlock_distance_step;//unlock Box
    stepperY->moveTo(static_cast<long>(yAxis));
    stepperY->setSpeed(ySpeed);
    while(stepperY->distanceToGo()!=0){
        stepperY->runSpeedToPosition();
        blink_array.blinkAll();
    }

    zAxis=zAxis+push_box_distance_step;//push it out
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperZ->setSpeed(zSpeed);
    while(stepperZ->distanceToGo()!=0){
        stepperZ->runSpeedToPosition();
        blink_array.blinkAll();
    }

    yAxis=box->coordinate().y();
    zAxis=box->coordinate().z();
    stepperY->moveTo(static_cast<long>(yAxis));
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperY->setSpeed(ySpeed);
    stepperZ->setSpeed(zSpeed);
    while((stepperY->distanceToGo()!=0) && (stepperZ->distanceToGo()!=0)){
        stepperY->runSpeedToPosition();
        stepperZ->runSpeedToPosition();
        blink_array.blinkAll();
    }
    // led_indicator.setColor(box->box_no()-1, COLOR_BLUE);

    zAxis=0;//Return to position
    stepperZ->moveTo(static_cast<long>(zAxis));
    stepperZ->setSpeed(zSpeed);
    while(stepperZ->distanceToGo()!=0)
        stepperZ->runSpeedToPosition();

    stepperX->setSpeed(stepperXSpeed);
    stepperY->setSpeed(stepperYSpeed);
    stepperZ->setSpeed(stepperZSpeed);

    box->unlocking(false);

    lcd.clear();
    return true;
}


void Mech_Arm::unlockAllBox(){
    for(int i=1;i<=16;i++) unlockBox(&boxes[i]);
}

void Mech_Arm::bringEmHomeDummy() {
    Serial.println("STEPPERS RESET");
}


void Mech_Arm::bringEmHome(){
    bringEmHomeDummy();
    return;
    String message="STEPPER RESET";
    Serial.println(message);
    Lcd_Menu::lcdClear(1);
    lcd.print(message);

    resetPosition(2);
    resetPosition(0);
    resetPosition(1);

    lcd.clear();
    Serial.println(message + " DONE");
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
        limitSwitch= STEPPER_PIN_XLIMIT_SW;
        enable= STEPPER_PIN_XENABLE;
        speed=stepper.speed();
        stepper.setSpeed(-500);
    }else if(axis==1){
        ch='Y';
        stepper=*stepperY;
        limitSwitch= STEPPER_PIN_YLIMIT_SW;
        enable= STEPPER_PIN_YENABLE;
        speed=stepper.speed();
        stepper.setSpeed(-800);
    }else if(axis==2){
        ch='Z';
        stepper=*stepperZ;
        limitSwitch= STEPPER_PIN_ZLIMIT_SW;
        enable= STEPPER_PIN_ZENABLE;
        speed=stepper.speed();
        stepper.setSpeed(-1200);
    }else
        return false;

    Lcd_Menu::lcdClear(0);
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

    AV_Functions::beepFor(500);
    lcd.clear();
    return true;
}


void Mech_Arm::boxMarker(){
    double steps=1000;
    Lcd_Menu::lcdClear(1);
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
        led_indicator.blink(currentBox,COLOR_RED);
        while(Serial.available()) {
            String ch=Serial.readStringUntil('\n');
            if(ch.charAt(0)=='N') { //EXITS
                run=false;
            }else if(ch.charAt(0)=='r'||ch.charAt(0)=='R') { //RESETS POSITION
                resetPosition(2);
                resetPosition(0);
                resetPosition(1);
            }else if(ch.charAt(0)=='t'||ch.charAt(0)=='T') { //TURNS ON STEPPER
                Serial.print("\nTurning on ");
                if(ch.charAt(1)=='x'||ch.charAt(1)=='X') { //TURNS STEPPER X
                    Serial.print('x');
                    stepperState(STEPPER_PIN_XENABLE, true);
                    AV_Functions::waitForInput();
                    stepperState(STEPPER_PIN_XENABLE, false);
                    digitalWrite(STEPPER_PIN_XSTEP,LOW);
                }else if(ch.charAt(1)=='y'||ch.charAt(1)=='Y') { // TURNS STEPPER Y
                    Serial.print('y');
                    stepperState(STEPPER_PIN_YENABLE, true);
                    AV_Functions::waitForInput();
                    stepperState(STEPPER_PIN_YENABLE, false);
                    digitalWrite(STEPPER_PIN_YSTEP,LOW);
                }else if(ch.charAt(1)=='z'||ch.charAt(1)=='Z') { //TURNS STEPPER Z
                    Serial.print('z');
                    stepperState(STEPPER_PIN_ZENABLE, true);
                    AV_Functions::waitForInput();
                    stepperState(STEPPER_PIN_ZENABLE, false);
                    digitalWrite(STEPPER_PIN_ZSTEP,LOW);
                }
                Serial.println();
                Serial.println("off");
            }else if(ch.charAt(0)=='s'||ch.charAt(0)=='S') { //SPECIFY SPEED
                Serial.print("\nEnter Speed For ");
                String enteredSpeed="10";
                if(ch.charAt(1)=='x'||ch.charAt(1)=='X') { //FOR X
                    Serial.print("x from "+ static_cast<String>(stepperX->maxSpeed()) + " to : ");
                    while(!Serial.available()){}
                    while(Serial.available()) enteredSpeed = Serial.readStringUntil('\n');
                    xSpeed=enteredSpeed.toFloat();
                    stepperX->setMaxSpeed(xSpeed);
                    stepperX->setSpeed(xSpeed);
                }else if(ch.charAt(1)=='y'||ch.charAt(1)=='Y') { //FOR Y
                    Serial.print("y from "+ static_cast<String>(stepperY->speed()) + " to : ");
                    while(!Serial.available()){}
                    while(Serial.available()) enteredSpeed = Serial.readStringUntil('\n');
                    ySpeed=enteredSpeed.toFloat();
                    stepperY->setMaxSpeed(ySpeed);
                    stepperY->setSpeed(ySpeed);
                }else if(ch.charAt(1)=='z'||ch.charAt(1)=='Z') { //FOR Z
                    Serial.print("z from "+ static_cast<String>(stepperZ->speed()) + " to : ");
                    while(!Serial.available()){}
                    while(Serial.available()) enteredSpeed = Serial.readStringUntil('\n');
                    zSpeed =enteredSpeed.toFloat();
                    stepperZ->setMaxSpeed(zSpeed);
                    stepperZ->setSpeed(zSpeed);
                }
                Serial.println("\n Speed Set to : "+static_cast<String>(enteredSpeed.toDouble()));
            }else if(ch.toDouble()<=16 && ch.toDouble()>=1){ //SELECT A BOX 1-16
                led_indicator.setColor(currentBox,COLOR_BLUE);
                currentBox=ch.toInt();
                Lcd_Menu::lcdClear(0);
                lcd.print("box: "+static_cast<String>(currentBox));
                Serial.println("Loaded Box: "+static_cast<String>(currentBox));
            }else{ // LOAD STEP
                steps=ch.toDouble();
                Lcd_Menu::lcdClear(1);
                lcd.print("stp: "+static_cast<String>(steps));
                Serial.println("Loaded Step: "+static_cast<String>(steps));
            }
        }

        bool enterPressed=false;
        while(digitalRead(BUTTON_ENTER)){
            enterPressed=true;
        }if(enterPressed){
            Xcord[currentBox-1]=xAxis;
            Ycord[currentBox-1]=yAxis;
            Zcord[currentBox-1]=zAxis;
            if(zAxis>1000){ zAxis=zAxis-1000; change = true;}

            led_indicator.setColor(currentBox,COLOR_GREEN);
            currentBox=currentBox-1;
            if(currentBox<=1) currentBox=16;
            Lcd_Menu::lcdClear(0);
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

        while(digitalRead(BUTTON_RIGHT)==HIGH){
            delay(BUTTON_DELAY);
            if(!change)
                xAxis=xAxis+static_cast<long>(steps);
            change=true;
        }while(digitalRead(BUTTON_LEFT)==HIGH){
            delay(BUTTON_DELAY);
            if(!change){
                if(static_cast<double>(xAxis)<=steps)
                    xAxis=0;
                else
                    xAxis= xAxis - static_cast<long>(steps);
            }
            change=true;
        }

        while(digitalRead(BUTTON_UP)==HIGH){
            delay(BUTTON_DELAY);
            if(!change){
                yAxis= yAxis + static_cast<unsigned long>(steps);
            }
            change=true;
        }while(digitalRead(BUTTON_DOWN)==HIGH){
            delay(BUTTON_DELAY);
            if(!change){
                if(static_cast<double>(yAxis)<=steps)
                    yAxis=0;
                else
                    yAxis=yAxis-static_cast<long>(steps);
            }
            change=true;
        }

        while(digitalRead(BUTTON_FORWARD)==HIGH){
            delay(BUTTON_DELAY);
            if(!change)
                zAxis=zAxis+static_cast<long>(steps);
            change=true;
        }while(digitalRead(BUTTON_BACKWARD)==HIGH){
            delay(BUTTON_DELAY);
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
    lcd.clear();
}


void Mech_Arm::stepperState(const byte enable, const boolean state){
    if(enable==32) digitalWrite(enable, state);
    else digitalWrite(enable, !state);
}


void Mech_Arm::offsetCoordinate(char const cordinate, byte const offsetPosition, int const offSetBy, boolean const applyToAll=true){
    // if(cordinate=='X'){
    //     if(applyToAll==false){
    //         xCordinate[offsetPosition]=xCordinate[offsetPosition]+offSetBy;
    //     }
    // }else if(cordinate=='Y'){
    //     if(applyToAll==false){
    //         yCordinate[offsetPosition]=yCordinate[offsetPosition]+offSetBy;
    //     }
    // }else if (cordinate=='Z'){
    //     if(applyToAll==false){
    //         zCordinate[offsetPosition]=zCordinate[offsetPosition]+offSetBy;
    //     }
    // }
}


