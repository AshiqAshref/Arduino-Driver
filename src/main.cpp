// #include <AV_Functions.h>
#include "Main.h"
#include <Led_indicator.h>
#include <Mech_Arm.h>
// #include <Led_Coordinate.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <Blink.h>

// #include <ReminderA.h>
// #include <ReminderB.h>
#include <BUTTON_PINS.h>
#include <AV_PINS.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
// #include <TimeLib.h>
#include <Blink_Array.h>
#include <Lcd_Menu.h>
// #include <AV_Functions.h>

// static auto reminderA = ReminderA();
// static auto reminderB = ReminderB();

SoftwareSerial espPort(11, 12); //(Rx, Tx)
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
auto led_indicator = Led_Indicator();
auto mech_arm = Mech_Arm();
auto blink_array = Blink_Array();
extern Lcd_Menu lcd_menu;
//y=1/2
//x=1/4
//z=1/2



void setup() {
    initializePins();
    // led_indicator.ledTestFunction(500);
    for(int i=1;i<=16;i++)
        led_indicator.setColor(i,'b');
    led_indicator.setColor(4,'g');
    led_indicator.setColor(7,'g');
    led_indicator.setColor(13,'g');

    lcd_menu.initializeLcd();
    rtc.begin();
    Serial.begin(9600);
    espPort.begin(9600);
    Serial.println("Ready");


    // mech_arm.bringEmHome();
    // mech_arm.boxMarker();
    // while(!Serial.available()){}
    // while(Serial.available()){char c = Serial.read();}
    // mech_arm.unlockBox(1);
    // while(!Serial.available()){}
    // while(Serial.available()){char c = Serial.read();}
    // mech_arm.unlockBox(16);
    // while(!Serial.available()){}
    // while(Serial.available()){char c = Serial.read();}
    // mech_arm.unlockBox(13);
    // while(!Serial.available()){}
    // while(Serial.available()){char c = Serial.read();}

    blink_array.add(1,'r');
    blink_array.add(11,'r');
    blink_array.add(6,'r');
    blink_array.add(16,'r');



    unsigned long prevTime=millis();
    while(!Serial.available()) {
        if (millis()-prevTime>1000) {
            prevTime=millis();
            lcd.setCursor(4,1);
            lcd.print(beautifyTime(rtc.now().hour())+":"
                +beautifyTime(rtc.now().minute())+":"
                +beautifyTime(rtc.now().second())
                );
        }
        blink_array.blinkAll();
    }
    Serial.println("Starting");
}


uint8_t last_second;
void loop() {
    if(rtc.now().second()!=last_second) {
        last_second = rtc.now().second();
        lcd.setCursor(4,1);
        lcd.print(beautifyTime(rtc.now().hour())+":"
            +beautifyTime(rtc.now().minute())+":"
            +beautifyTime(rtc.now().second())
            );
    }

    while (espPort.available()) {
        while (espPort.available())
            Serial.print(static_cast<char>(espPort.read()));
        Serial.println();
        espPort.flush();
        AV_Functions::beepFor(100);
    }
    // JsonDocument doc;
    // while(espPort.available()) {
    //     DeserializationError error = deserializeJson(doc, espPort);
    //     if(error) Serial.println(error.f_str());
    //     // else serializeJson(doc, Serial);
    //     serializeJson(doc, Serial);
    //
    //     Serial.println("Caught One!\n\n");
    //     espPort.flush();
    //     AV_Functions::beepFor(100);
    // }
    delay(10000);
}

String beautifyTime(const uint8_t h_m_s) {
    if(h_m_s<9)
        return '0'+static_cast<String>(h_m_s);
    return static_cast<String>(h_m_s);
}



// void addBoxes() {
//     String json = R"(
//     [
//         {
//             "med_id": 1,
//             "med_box_no": 7,
//             "med_amount": 25,
//             "med_name": "NEPAFENAC"
//         },
//         {
//             "med_id": 2,
//             "med_box_no": 14,
//             "med_amount": 56,
//             "med_name": "AMNEAL BIOSCIENCES"
//         },
//         {
//             "med_id": 20,
//             "med_box_no": 9,
//             "med_amount": 50,
//             "med_name": "TRIMETHOPRIM HYDROCHLORIDE"
//         }
//     ])";
//     JsonDocument doc;
//     DeserializationError error = deserializeJson(doc,json);
//     if(error) {
//         Serial.print("Error..: ");
//         Serial.println(error.f_str());
//         Serial.println();
//     }
//     serializeJson(doc, Serial);
//     Serial.print("boxes size: ");
//     Serial.println(doc.size());
//
//     for(size_t i=0;i<doc.size();i++) {
//         // auto med_id = doc[i]["med_id"].as<unsigned int>();
//         // byte med_box_no = doc[i]["med_box_no"].as<byte>();
//         // byte med_amount = doc[i]["med_amount"].as<byte>();
//         String med_name = doc[i]["med_name"].as<String>();
//
//
//
//     }
//
//     Serial.println(doc["times"][0]["timeb_time"].as<String>());
//     doc.clear();
// }



// void loop() {
//   // Serial.println("Mamaaa");
//
//   // while(Serial.available()) {
//   //   JsonArray array = doc.to<JsonArray>();
//   //
//   //   // add some values
//   //   array.add("hello");
//   //   array.add(42);
//   //   array.add(3.14);
//   //
//   //   // serialize the array and send the result to Serial
//   //   serializeJson(doc, Serial);
//   // }
//   while (!Serial.available()){}
//
//   // JsonArray array = doc.to<JsonArray>();
//   //
//   // // add some values
//   // array.add("hello");
//   // array.add(42);
//   // array.add(3.14);
//   //
//   // // serialize the array and send the result to Serial
//   // // serializeJson(doc, Serial);
//   // while(Serial.available()) {
//   // }
// }
//

// void loop() {
//   if(digitalRead(static_cast<uint8_t>(BUTTON_PINS::enterButton))) {
//     AV_Functions::beepFor(100);
//     while(digitalRead(static_cast<uint8_t>(BUTTON_PINS::enterButton))){delay(100);}
//     lcd_menu.menuPage();
//     lcd_menu.getLcd().clear();
//     lcd_menu.getLcd().print("MAIN PAGE");
//   }
//   // checkEspForRequest();
// }

//
// boolean initializeEspCommunicator(){
//   espPort.begin(9600);
//   delay(50);
//   if(!espPort.isListening())
//     return false;
//   return true;
// }
//
//
// void checkEspForRequest() {
//   String identifier="";
//   while(espPort.available()){//...............................SOFT_SERIAL
//     espPort.readStringUntil('{');
//
//     identifier=espPort.readStringUntil(':');
//     if(identifier=="ALM1"){ //...........................Mode_B(Default)
//       Serial.print("Alarm1= ");
//       String alm=espPort.readStringUntil('}')+'}';
//       // currentB=jsonToClassB(alm);
//       Serial.println(currentB.toString());
//     }else if(identifier=="UPC1"){
//       Serial.print("Upcomming1= ");
//       String upc=espPort.readStringUntil('}')+'}';
//       // upcommingB=jsonToClassB(upc);
//       Serial.println(upcommingB.toString());
//
//     }else if(identifier=="ALM0"){//.......................Mode_A
//       Serial.print("Alarm0= ");
//       String alm=espPort.readStringUntil('}')+'}';
//       // currentA=jsonToClass(alm);
//       Serial.println(currentA.toString());
//     }else if(identifier=="UPC0"){
//       Serial.print("Upcomming0= ");
//       String upc=espPort.readStringUntil('}')+'}';
//       // upcommingA=jsonToClass(upc);
//       Serial.println(upcommingA.toString());
//     }
//   }
//
// }


// ReminderA jsonToClass(String& dat){
//   unsigned int id=dat.substring((dat.indexOf(':')+1),dat.indexOf(',')).toInt();
//
//   dat=dat.substring(dat.indexOf(',')+1);
//   const String date=dat.substring(dat.indexOf(':')+1,dat.indexOf(','));
//
//   const byte H=date.substring(1,3).toInt();
//   const byte M=date.substring(4,6).toInt();
//
//   dat=dat.substring(dat.indexOf(',')+1);
//   unsigned int boxNo=dat.substring((dat.indexOf(':')+2),(dat.indexOf(',')-1)).toInt();
//
//   return {new DateTime(0,0,0,H,M,0),&boxNo,&id, new boolean(false)};
// }


// ReminderB jsonToClassB(const String& dat) {
//   const byte H = dat.substring((dat.indexOf('=') + 1), dat.indexOf(':')).toInt();
//   const byte M = dat.substring((dat.indexOf(':') + 1), dat.indexOf(',')).toInt();
//
//   String boxesString = dat.substring((dat.indexOf(',') + 1));
//   boxesString = boxesString.substring(boxesString.indexOf('=') + 1);
//   const String success=boxesString.substring(boxesString.indexOf('=')+1, boxesString.length()-1);
//   boxesString = boxesString.substring(0, boxesString.indexOf(']')+1);
//
//   boolean suc=false;
//   if(success.toInt()==1){
//     suc=true;
//   }else if(success.toInt()==0){
//     suc=false;
//   }
//   boxesString.trim();
//   return {new DateTime(0, 0, 0, H, M, 0), &boxesString, &suc};
// }


//

//
//

// void setup() {
//   Serial.begin(9600);
//   Serial.println("Ready");
//   while(!Serial.available()) {
//     Serial.println("not avail");
//
//   }
//   while(Serial.available()) {
//     Serial.println("avail");
//   }
//
//   // initializePins();
//   // lcd_menu = Lcd_Menu();
//   // led_indicator=new Led_Indicator(
//   //   static_cast<byte>(LED_ARRAY_PINS::dataPin),
//   //   static_cast<byte>(LED_ARRAY_PINS::clkPin),
//   //   static_cast<byte>(LED_ARRAY_PINS::csPin),
//   //   2
//   // );
//   // mech_arm = new Mech_Arm(lcd_menu, led_indicator);
//
//   // initializeEspCommunicator();
//   // mech_arm->bringEmHome();
//
//   // AV_Functions::beepFor(500);
//
//   Serial.println();
//   // led_indicator->ledTestFunction();
//   // lcd_menu->lcdClear(0);
//   // lcd.print("Press to Start");
//   // while(!digitalRead(static_cast<uint8_t>(button_pins::enterButton))) {}
//   // delay(500);
//
//   // unlockAllBox();
//   // bringEmHome();
//   // badPosition();
//   // unlockBox(14);
// }

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

void addRemindes() {
    String json =R"(
    [
        {
            "timeId": 24,
            "time": "00:10",
            "medicines": [
                {
                    "medBox": 9,
                    "dosage": 1,
                    "success": false
                }
            ]
        },
        {
            "timeId": 6,
            "time": "00:20",
            "medicines": [
                {
                    "medBox": 14,
                    "dosage": 2,
                    "success": false
                },
                {
                    "medBox": 9,
                    "dosage": 3,
                    "success": false
                }
            ]
        },
        {
            "timeId": 4,
            "time": "10:05",
            "medicines": [
                {
                    "medBox": 7,
                    "dosage": 3,
                    "success": false
                }
            ]
        },
        {
            "timeId": 3,
            "time": "20:45",
            "medicines": [
                {
                    "medBox": 7,
                    "dosage": 2,
                    "success": false
                },
                {
                    "medBox": 14,
                    "dosage": 4,
                    "success": false
                }
            ]
        }
    ])";
}
