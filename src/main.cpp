// #include <AV_Functions.h>
#include "Main.h"
#include <Led_indicator.h>
#include <Mech_Arm.h>
#include <COLOR.h>

// #include <Led_Coordinate.h>
// #include <SoftwareSerial.h>
#include <BUTTON_PINS.h>
#include <AV_PINS.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Blink_Array.h>
#include <Lcd_Menu.h>
#include <sensor_unit.h>
#include <Status_Directive.h>
#include <RTClib.h>
#include <Communication_protocols.h>
#include <ReminderB.h>

// #include <AV_Functions.h>

byte box_size = 16;
Box boxes[16] = {
    Box(1 , Pos_Coordinate(xCordinate[0] ,yCordinate[0] , zCordinate[0])) ,
    Box(2 , Pos_Coordinate(xCordinate[1] ,yCordinate[1] , zCordinate[1])) ,
    Box(3 , Pos_Coordinate(xCordinate[2] ,yCordinate[2] , zCordinate[2])) ,
    Box(4 , Pos_Coordinate(xCordinate[3] ,yCordinate[3] , zCordinate[3])) ,
    Box(5 , Pos_Coordinate(xCordinate[4] ,yCordinate[4] , zCordinate[4])) ,
    Box(6 , Pos_Coordinate(xCordinate[5] ,yCordinate[5] , zCordinate[5])) ,
    Box(7 , Pos_Coordinate(xCordinate[6] ,yCordinate[6] , zCordinate[6])) ,
    Box(8 , Pos_Coordinate(xCordinate[7] ,yCordinate[7] , zCordinate[7])) ,
    Box(9 , Pos_Coordinate(xCordinate[8] ,yCordinate[8] , zCordinate[8])) ,
    Box(10, Pos_Coordinate(xCordinate[9] ,yCordinate[9] , zCordinate[9])) ,
    Box(11, Pos_Coordinate(xCordinate[10],yCordinate[10], zCordinate[10])),
    Box(12, Pos_Coordinate(xCordinate[11],yCordinate[11], zCordinate[11])),
    Box(13, Pos_Coordinate(xCordinate[12],yCordinate[12], zCordinate[12])),
    Box(14, Pos_Coordinate(xCordinate[13],yCordinate[13], zCordinate[13])),
    Box(15, Pos_Coordinate(xCordinate[14],yCordinate[14], zCordinate[14])),
    Box(16, Pos_Coordinate(xCordinate[15],yCordinate[15], zCordinate[15]))
};

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
extern Lcd_Menu lcd_menu;
auto led_indicator = Led_Indicator();
auto mech_arm = Mech_Arm();
auto blink_array = Blink_Array();
auto sensor_unit = Sensor_unit();
auto comms = Communication_protocols();
ReminderB *upcommingReminderB=nullptr;



void setup() {
    pinMode(STAT_LED_PIN,OUTPUT);
    digitalWrite(STAT_LED_PIN,HIGH);
    initializePins();
    lcd_menu.initializeLcd();

    Serial.begin(9600);
    Serial1.begin(115200);
    rtc.begin();
    // led_indicator.ledTestFunction(100);
    for(auto &box : boxes)
        box.set_status(DEFAULT_);

}

unsigned long prevTime=0;
unsigned long prevTime_since_reminder_request=0;

void loop() {
    if (millis()-prevTime>1000) {
        prevTime=millis();
        lcd.setCursor(4,1);
        lcd.print(get_formated_Time(12));
        Sensor_unit::check_if_any_box_open();
    }
    if (millis()-prevTime_since_reminder_request>4000 && !upcommingReminderB) {
        comms.get_next_reminder_B(get_current_plain_unix_time());
    }

    blink_array.blinkAll();
    comms.handle_communications();
}

unsigned long get_current_plain_unix_time() {
    const DateTime curr_time = rtc.now();
    const auto temp = DateTime(0,0,0,curr_time.hour(),curr_time.minute(),curr_time.second());
    return temp.unixtime();

}

String get_formated_Time(const byte mode) {
    const DateTime curr_time = rtc.now();
    // Serial.println(curr_time.unixtime());
    if(mode == 12)
        return
            beautifyTime(curr_time.twelveHour())+":"
            +beautifyTime(curr_time.minute())+":"
            +beautifyTime(curr_time.second())+" "
            +(curr_time.isPM()? "p": " ");
    return
        beautifyTime(rtc.now().hour())+":"
        +beautifyTime(rtc.now().minute()) +":"
        +beautifyTime(rtc.now().second());
}

String beautifyTime(const uint8_t h_m_s) {
    if(h_m_s<10)
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
// delay(10000);
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
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),HIGH);
}

// void addRemindes() {
//     String json =R"(
//     [
//         {
//             "timeId": 24,
//             "time": "00:10",
//             "medicines": [
//                 {
//                     "medBox": 9,
//                     "dosage": 1,
//                     "success": false
//                 }
//             ]
//         },
//         {
//             "timeId": 6,
//             "time": "00:20",
//             "medicines": [
//                 {
//                     "medBox": 14,
//                     "dosage": 2,
//                     "success": false
//                 },
//                 {
//                     "medBox": 9,
//                     "dosage": 3,
//                     "success": false
//                 }
//             ]
//         },
//         {
//             "timeId": 4,
//             "time": "10:05",
//             "medicines": [
//                 {
//                     "medBox": 7,
//                     "dosage": 3,
//                     "success": false
//                 }
//             ]
//         },
//         {
//             "timeId": 3,
//             "time": "20:45",
//             "medicines": [
//                 {
//                     "medBox": 7,
//                     "dosage": 2,
//                     "success": false
//                 },
//                 {
//                     "medBox": 14,
//                     "dosage": 4,
//                     "success": false
//                 }
//             ]
//         }
//     ])";
// }
