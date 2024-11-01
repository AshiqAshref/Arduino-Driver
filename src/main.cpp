// #include <AV_Functions.h>
#include "Main.h"
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

#include <Led_indicator.h>
#include <Mech_Arm.h>
#include <BUTTON_PINS.h>
#include <AV_PINS.h>
#include <Blink_Array.h>
#include <CommunicationHandler.h>
#include <Lcd_Menu.h>
#include <ReminderB.h>
#include <Network_info.h>
#include <sensor_unit.h>
#include <Status_Directive.h>
#include <Command_activate_AP.h>
#include <Command_deactivate_ap.h>
#include <Command_get_reminderB.h>
#include <Command_get_time.h>
#include <Command_daylight_sav.h>
#include <Command_get_network_inf.h>

constexpr byte box_size = 16;
Box boxes[box_size] = {
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

auto command_get_time = Command_get_time(
        CommunicationHandler::send_command_get_time,
        CommunicationHandler::NTP_response_handler,
        [](){return true;},
        20000, 3600000);
auto command_get_reminder_b = Command_get_reminderB (
        CommunicationHandler::resend_command_get_reminder_B,
        CommunicationHandler::get_reminder_b_response_handler,
        [](){return true;},
        2000);
auto command_activate_AP = Command_activate_AP (
        CommunicationHandler::send_command_activate_ap,
        CommunicationHandler::activate_AP_response_handler,
        [](){return true;},
        2000);
auto command_deactivate_ap = Command_deactivate_ap(
        CommunicationHandler::send_command_deactivate_ap,
        CommunicationHandler::deactivate_AP_response_handler,
        CommunicationHandler::deactivate_AP_request_handler,
        2000);
auto command_get_network_inf = Command_get_network_inf(
        CommunicationHandler::send_command_get_network_inf,
        CommunicationHandler::get_network_inf_response_handler,
        CommunicationHandler::get_network_inf_request_handler,
        2000);
auto command_daylight_sav = Command_daylight_sav(
        CommunicationHandler::send_command_daylight_sav,
        CommunicationHandler::daylight_sav_response_handler,
        CommunicationHandler::daylight_sav_request_handler,
        10000);


RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
auto lcd_menu = Lcd_Menu();
auto led_indicator = Led_Indicator();
auto mech_arm = Mech_Arm();
auto blink_array = Blink_Array();
auto sensor_unit = Sensor_unit();
auto upcommingReminderB=  ReminderB();
auto network_info= Network_info(adjust_daylight_saving);



void setup() {
    pinMode(STAT_LED_PIN,OUTPUT);
    digitalWrite(STAT_LED_PIN,HIGH);
    initializePins();
    Lcd_Menu::initializeLcd();

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
        const DateTime current_time = rtc.now();
        if(upcommingReminderB.check_for_alarm(current_time))
            Serial.println("ALARM");
        print_lcd_time(current_time,12);
        Sensor_unit::check_if_any_box_open();

    }
    // if (millis()-prevTime_since_reminder_request>4000 && !upcommingReminderB.isValid()) {
    //     CommunicationHandler::send_command_get_reminder_B(get_current_plain_unix_time());
    //     prevTime_since_reminder_request=millis();
    //
    // }

    blink_array.blinkAll();
    CommunicationHandler::handle_communications();
    Lcd_Menu::handleMenu();
}

void print_lcd_time(const DateTime &current_time, const byte mode) {
    lcd.setCursor(4,1);
    lcd.print(get_formated_Time(current_time,mode));
}

unsigned long get_current_plain_unix_time() {
    const DateTime curr_time = rtc.now();
    const auto temp = DateTime(0,0,0,curr_time.hour(),curr_time.minute(),curr_time.second());
    return temp.unixtime();
}

String get_formated_Time(const DateTime &curr_time, const byte mode) {
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
void adjust_daylight_saving(const bool dls) {
    dls?
        rtc.adjust(DateTime(rtc.now().unixtime()+3600)):
        rtc.adjust(DateTime(rtc.now().unixtime()-3600));
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
    pinMode(BUTTON_ENTER,INPUT);
    pinMode(BUTTON_LEFT,INPUT);
    pinMode(BUTTON_RIGHT,INPUT);
    pinMode(BUTTON_UP,INPUT);
    pinMode(BUTTON_DOWN,INPUT);
    pinMode(BUTTON_FORWARD,INPUT);
    pinMode(BUTTON_BACKWARD,INPUT);
    digitalWrite(static_cast<uint8_t>(AV_PINS::beeper),HIGH);
}


