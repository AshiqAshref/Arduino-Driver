#ifndef COMMUNICATION_PROTOCOLS_H
#define COMMUNICATION_PROTOCOLS_H
#include <Arduino.h>
#include <SoftwareSerial.h>

enum COMM_PROTOCOL:byte {
    SYN =             0b10000000,
    SYN_ACK =         0b01000000,
    ACK =             0b11000000,
    FIN =             0b00100000,
    RETRY =           0b01100000,
    TIMEOUT=          0b10100000,
    UNKW_ERR=         0b01010000,
    SUCCESS=          0b01110000,
    PROTOCOL_FILTER = 0b11110000
};

class Communication_protocols {
    const unsigned long time_out_ = 10000;
    const unsigned long retry_interval_onSucc_= 3600000;
    // const unsigned long retry_interval_onFail_= 30000;
    const unsigned long retry_interval_onFail_= 30000;
    unsigned long NTP_refresh_rate_ = retry_interval_onFail_;
    const byte funct_id_getTime_ =   0b00001000;
    const byte function_id_filter_ = 0b00001111;

    static byte getProtocol(byte b);
    byte getFunction_id(byte response_header) const;
    COMM_PROTOCOL get_response(byte function_id) const;
    COMM_PROTOCOL send_response_SYN_ACK(byte function_id) const;
    static void send_response_ACK(byte function_id);
    static void send_request_RETRY(byte function_id);
    static void send_request_SYN(byte function_id);
    static void send_status_SUCCESS(byte function_id);
    static void send_status_TIMEOUT(byte function_id);
    static void send_header(byte function_id,  byte protocol_id);
    static void close_session(byte function_id);

    static void clear_receive_buffer();
    bool getTimeFromBuffer();
    void handleResponse();
    void handleRequest();
    void handle_header(byte response_header) ;
    bool NTP_response_handler() ;
    bool wait_for_response() const;
    void NTP_success(bool success);

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
    byte get_time() const {return funct_id_getTime_;}


};



#endif //COMMUNICATION_PROTOCOLS_H