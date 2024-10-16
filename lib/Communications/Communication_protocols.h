#ifndef COMMUNICATION_PROTOCOLS_H
#define COMMUNICATION_PROTOCOLS_H
#include <Arduino.h>
#include <ReminderB.h>
#include <ArduinoJson/Document/JsonDocument.hpp>


enum COMM_PROTOCOL:byte {
    SYN =             0b10000000,
    SYN_ACK =         0b01000000,
    ACK =             0b11000000,
    FIN =             0b00100000,
    UNKW_ERR=         0b01010000,
    RETRY =           0b01100000,
    SUCCESS=          0b01110000,
    TIMEOUT=          0b10100000,
    TCP =             0b10010000,
    ACK_TCP =         0b10110000,
    PROTOCOL_FILTER = 0b11110000
};

constexpr byte funct_id_getTime_ =   0b00001000;
constexpr byte funct_id_get_reminderB_ = 0b00001100;
constexpr byte function_id_filter_ = 0b00001111;
constexpr unsigned long time_out_ = 10000;


class Communication_protocols {
    const unsigned long retry_interval_onSucc_= 3600000;
    const unsigned long retry_interval_onFail_= 30000;
    unsigned long NTP_refresh_rate_ = 0;
    unsigned long get_next_time_ = 0;

    bool get_next_reminder_status=false;


    static byte getProtocol(byte b);
    static COMM_PROTOCOL get_response(byte function_id, bool clear_buffer = true);

    static byte getFunction_id(byte response_header);
    static COMM_PROTOCOL send_response_SYN_ACK(byte function_id, bool clear_buffer = true) ;
    static void send_request_SYN(byte function_id);
    static void send_response_ACK(byte function_id);
    static void send_request_RETRY(byte function_id);
    static void send_status_SUCCESS(byte function_id);
    static void send_status_UNKW_ERROR(byte function_id);
    static void send_status_TIMEOUT(byte function_id);
    static void send_header(byte function_id,  byte protocol_id);
    static void close_session(byte function_id);


    static COMM_PROTOCOL sendJsonDocument(const ArduinoJson::JsonDocument &doc, byte function_id, byte max_retries=20);

    static ArduinoJson::JsonDocument receive_jsonDocument(byte function_id, byte max_retries=20);
    static unsigned long getLongFromBuffer(byte function_id);

    static void send_tcp_ack(byte function_id, byte recived_squence);
    static void clear_receive_buffer();


    void setTime(unsigned long ux_time);
    // void handleResponse();
    // void handleRequest();
    void handle_header(byte response_header) ;
    bool NTP_response_handler(const byte function_id) ;
    bool get_reminder_b_response_handler(const byte function_id) const;


    static bool wait_for_response();

    static bool wait_for_response(byte function_id);

    void NTP_success(bool success);
    static bool add_reminderb_to_class(ArduinoJson::JsonDocument doc);

public:
    bool status_led_state_ = false;
    Communication_protocols()=default;
    void handle_communications() ;
    void invert_stat_led();

    static byte *longToByte(unsigned long long_);
    static unsigned long bytesToLong(const byte *byte_);
    static void printProtocol(COMM_PROTOCOL res_code);
    static void printlnProtocol(COMM_PROTOCOL res_code);
    static void printBin(byte aByte);
    static void printlnBin(byte aByte);

    static COMM_PROTOCOL byte_to_enum(byte protocol_id);

    static void get_time();
    void get_next_reminder_B(unsigned long epoch);
    static COMM_PROTOCOL sendLong(const unsigned long res_long, const byte function_id);
    bool get_next_reminder_received() const {return get_next_reminder_status;}

};



#endif //COMMUNICATION_PROTOCOLS_H
