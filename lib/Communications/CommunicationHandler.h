
#ifndef COMMUNICATIONHANDLER_H
#define COMMUNICATIONHANDLER_H
#include <Arduino.h>
#include <Communication_protocols.h>
// #include <ArduinoJson.h>


struct command_details {
    Command_enum command{};
    CommandStatus status = COMPLETED;
    const unsigned long retry_interval_on_fail=5000;
    unsigned long current_millis=0;
    void (*send_request)(){};
    bool (*request_handler)(){};
    bool (*response_handler)(){};
};

constexpr unsigned long get_reminder_refresh_rate_ = 10000;

class CommunicationHandler: public Communication_protocols{
    static void setTime(unsigned long ux_time);
    static void add_reminderb_to_class(JsonDocument doc);
    static void handle_header(byte response_header);
static bool get_network_inf_response_handler_local();
public:
    static void handle_communications();

    static void send_command_get_time();
    static void send_command_activate_ap();
    static void send_command_deactivate_ap();
    static void send_command_get_reminder_B(unsigned long epoch);
    static void resend_command_get_reminder_B();

    static bool get_reminder_b_response_handler(unsigned long get_reminder_time_key) ;
    static bool deactivate_AP_response_handler();
    static bool deactivate_AP_request_handler();

    static void send_command_get_network_inf();
    static bool get_network_inf_response_handler();
    static bool get_network_inf_request_handler();

    static void send_command_daylight_sav();
    static bool daylight_sav_response_handler(bool daylight_sav);

    static bool activate_AP_response_handler();
    static bool NTP_response_handler();

};



#endif //COMMUNICATIONHANDLER_H
