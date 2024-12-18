#include <AV_Functions.h>

#include "Main.h"
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

#include <Led_indicator.h>
#include <Mech_Arm.h>
#include <IO_PINS.h>
#include <Blink_Array.h>
#include <CommunicationHandler.h>
#include <Lcd_Menu.h>
#include <ModeB/ReminderB.h>
#include <Network_info.h>
#include <sensor_unit.h>
#include <Status_Directive.h>
#include <Command_activate_AP.h>
#include <Command_deactivate_ap.h>
#include <Command_get_reminderB.h>
#include <Command_get_time.h>
#include <Command_daylight_sav.h>
#include <Command_get_box_inf.h>
#include <Command_get_network_inf.h>
#include <Command_server_ip.h>
#include <Command_reminderB_change.h>
#include <Command_reminderB_send_log.h>
#include <ReminderBRunner.h>


// #include <MemoryFree.h>
// #include <pgmStrToRAM.h>


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
auto command_server_ip = Command_server_ip(
        CommunicationHandler::send_command_server_ip,
        CommunicationHandler::server_ip_response_handler,
        CommunicationHandler::server_ip_request_handler,
        6000);
auto command_get_reminder_b = Command_get_reminderB (
        CommunicationHandler::send_command_get_reminder_B,
        CommunicationHandler::get_reminder_b_response_handler,
        [](){return true;},
        2000);
auto command_reminderB_change = Command_reminderB_change(
        CommunicationHandler::send_command_reminderB_change,
        CommunicationHandler::reminderB_change_response_handler,
        CommunicationHandler::reminderB_change_request_handler,
        4000,180000);
auto command_reminderB_send_log = Command_reminderB_send_log(
        CommunicationHandler::send_command_reminderB_send_log,
        CommunicationHandler::reminderB_send_log_response_handler,
        CommunicationHandler::reminderB_send_log_request_handler,
        4000);
auto command_get_box_inf = Command_get_box_inf(
        CommunicationHandler::send_command_get_box_inf,
        CommunicationHandler::get_box_inf_response_handler,
        CommunicationHandler::get_box_inf_request_handler,
        4000);


RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
auto lcd_menu = Lcd_Menu();
auto led_indicator = Led_Indicator();
auto mech_arm = Mech_Arm();
auto blink_array = Blink_Array();
auto sensor_unit = Sensor_unit();
auto upcommingReminderB=  ReminderB();
auto reminderBRunner=  ReminderBRunner();
auto network_info= Network_info();


void setup() {
    Serial.begin(9600);
    Serial2.begin(115200);

    pinMode(STAT_LED_PIN,OUTPUT);
    digitalWrite(STAT_LED_PIN,HIGH);
    initializePins();
    Lcd_Menu::initializeLcd();
    rtc.begin();

    // led_indicator.ledTestFunction(100);
    for(auto &box : boxes)
        box.set_box_status(BOX_STATUS_DEFAULT);

    auto tmpdt = DateTime(2020,12,12,13,38); //TEST_ONLY
    command_get_reminder_b.send_request(tmpdt.unixtime()); //TEST_ONLY
    rtc.adjust(DateTime(2020,12,12,13,38,39)); //TEST_ONLY
    // command_get_reminder_b.send_request(get_current_unix_time());
    // mech_arm.boxMarker();
    // mech_arm.unlockAllBox();
}


unsigned long prevTime=0;
void loop() {
    if (millis()-prevTime>1000) {
        prevTime=millis();
        const DateTime current_time = rtc.now();
        if(upcommingReminderB.check_for_alarm(current_time)) {
            Serial.println("Setting UPC");
            reminderBRunner.set_current_reminder(upcommingReminderB);
        }
        print_lcd_time(current_time);
        Sensor_unit::check_if_any_box_open();
    }
    if (command_get_reminder_b.status()==COMPLETED && !upcommingReminderB.isValid()) {
        command_get_reminder_b.send_request(get_current_unix_time());
    }

    reminderBRunner.handleReminder();
    blink_array.blinkAll();
    CommunicationHandler::handle_communications();
    Lcd_Menu::handleMenu();
}


void print_lcd_time(const DateTime &current_time, const TimeMode mode) {
    lcd.setCursor(1,0);
    lcd.print("next: "+upcommingReminderB.display_time());

    lcd.setCursor(4,1);
    lcd.print(get_formated_Time(current_time,mode));
}


unsigned long get_current_unix_time() {
    const DateTime curr_time = rtc.now();
    Serial.print("TIME_NOW: ");
    Serial.println(get_formated_Time(curr_time));
    const auto temp = DateTime(2020,12,12,curr_time.hour(),curr_time.minute(),curr_time.second());
    return temp.unixtime();
}


String get_formated_Time(const DateTime &curr_time, const TimeMode mode) {
    if(mode == TIME_MODE_12)
        return
            AV_Functions::beautifyTime(curr_time.twelveHour())+":"
            +AV_Functions::beautifyTime(curr_time.minute())+":"
            +AV_Functions::beautifyTime(curr_time.second())+" "
            +(curr_time.isPM()? "p": " ");
    return
        AV_Functions::beautifyTime(rtc.now().hour())+":"
        +AV_Functions::beautifyTime(rtc.now().minute()) +":"
        +AV_Functions::beautifyTime(rtc.now().second());
}



void initializePins(){
    pinMode(OUTPUT_PIN_BEEPER, OUTPUT);
    pinMode(BUTTON_ENTER,INPUT);
    pinMode(BUTTON_LEFT,INPUT);
    pinMode(BUTTON_RIGHT,INPUT);
    pinMode(BUTTON_UP,INPUT);
    pinMode(BUTTON_DOWN,INPUT);
    pinMode(BUTTON_FORWARD,INPUT);
    pinMode(BUTTON_BACKWARD,INPUT);
    digitalWrite(OUTPUT_PIN_BEEPER,HIGH);
}


