
#ifndef COMMUNICATIONHANDLER_H
#define COMMUNICATIONHANDLER_H
#include <Arduino.h>
#include <Communication_protocols.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <Lcd_Menu.h>




constexpr unsigned long get_reminder_refresh_rate_ = 10000;


class CommunicationHandler: public Communication_protocols{
    static bool get_reminder_b_response_handler(Commands command) ;

    static void send_command_deactivate_ap();

    static bool deactivate_AP_response_handler(Commands command);

    static bool deactivate_AP_request_handler(Commands command);

    static bool activate_AP_response_handler(Commands command);

    static void reminder_b_success(bool success);
    static void NTP_success(bool success);
    static void setTime(unsigned long ux_time);
    static void add_reminderb_to_class(JsonDocument doc);
    static bool NTP_response_handler(Commands command) ;
    static void handle_header(byte response_header) ;
public:
    static void get_next_reminder_B(unsigned long epoch);
    static void handle_communications();
    static void get_time();



};



#endif //COMMUNICATIONHANDLER_H
