
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


    static void send_command_get_reminder_B();
    static bool get_reminder_b_response_handler(unsigned long get_reminder_time_key) ;

    static void send_command_deactivate_ap();
    static bool deactivate_AP_response_handler();
    static bool deactivate_AP_request_handler();

    static void send_command_get_network_inf();
    static bool get_network_inf_response_handler();
    static bool get_network_inf_request_handler();

    static void send_command_daylight_sav();
    static bool daylight_sav_response_handler(bool daylight_sav, bool command_type);
    static bool daylight_sav_request_handler();
    static bool daylight_sav_receive_dls();
    static bool daylight_sav_send_dls(bool daylight_sav);

    static void send_command_server_ip();
    static bool server_ip_response_handler(const IPAddress& server_ip, bool command_type);
    static bool server_ip_request_handler();
    static bool server_ip_receive_ip();
    static bool server_ip_send_ip(const IPAddress& server_ip);

    static void send_command_reminderB_change();
    static bool reminderB_change_response_handler();
    static bool reminderB_change_request_handler();

    static void send_command_reminderB_send_log();
    static bool reminderB_send_log_response_handler(const JsonDocument &last_reminder);
    static bool reminderB_send_log_request_handler();

    static void send_command_activate_ap();
    static bool activate_AP_response_handler();

    static void send_command_get_time();
    static bool NTP_response_handler();

};



#endif //COMMUNICATIONHANDLER_H
