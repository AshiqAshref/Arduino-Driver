#ifndef COMMUNICATION_PROTOCOLS_H
#define COMMUNICATION_PROTOCOLS_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <IPAddress.h>



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

enum Commands:byte {
    GET_TIME =          0b00001000,
    GET_REMINDER_B =    0b00001100,
    ACTIVATE_AP =       0b00000100,
    DEACTIVATE_AP =     0b00000010,
    COMMAND_FILTER =    0b00001111
};

enum CommandStatus:byte {
    IN_PROGRESS = 0b10101010,
    FAILED = 0b01010101,
    COMPLETED = 0b00111100,
};


constexpr unsigned long time_out_ = 10000;

constexpr  unsigned long retry_interval_onSucc_= 3600000;
constexpr unsigned long retry_interval_onFail_= 30000;



class Communication_protocols {
protected:
    static byte getProtocol(byte header);
    static COMM_PROTOCOL get_response(Commands command, bool clear_buffer = true);

    static byte getCommand(byte header);
    static COMM_PROTOCOL send_response_SYN_ACK(Commands command, bool clear_buffer = true) ;
    static void send_request_SYN(Commands command);
    static void send_response_ACK(Commands command);
    static void send_request_RETRY(Commands command);
    static void send_status_SUCCESS(Commands command);
    static void send_status_UNKW_ERROR(Commands command);
    static void send_status_TIMEOUT(Commands command);
    static void send_header(Commands command,  byte protocol_id);
    static void close_session(Commands command);

    static void initialize();

    static COMM_PROTOCOL sendJsonDocument(const JsonDocument &doc, Commands command, byte max_retries=20);
    static JsonDocument receive_jsonDocument(Commands command, byte max_retries=20);
    static COMM_PROTOCOL sendLong(unsigned long res_long, Commands command);
    static unsigned long receive_long(Commands command);
    static IPAddress receive_IP(Commands command, byte max_retries=20);
    static COMM_PROTOCOL send_IP(const IPAddress &IP, Commands command);

    static void clear_receive_buffer();
    static byte extractHour(const String &formated_time);
    static byte extractMinute(const String &formated_time);
    static bool wait_for_response();
    static bool wait_for_response(Commands command);

public:
    Communication_protocols()=default;
    

    static byte *longToByte(unsigned long long_);
    static unsigned long bytesToLong(const byte *byte_);
    static void printProtocol(COMM_PROTOCOL res_code);
    static void printlnProtocol(COMM_PROTOCOL res_code);
    static void printBin(byte aByte);
    static void printlnBin(byte aByte);

    static COMM_PROTOCOL byte_to_enum(byte protocol_id);
    static void invert_stat_led();
};



#endif //COMMUNICATION_PROTOCOLS_H
