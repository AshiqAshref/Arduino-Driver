//
// Created by user on 19/08/2024.
//

#include "Communication_protocols.h"
#include <RTClib.h>
#include <CRC.h>

extern SoftwareSerial espPort;
extern RTC_DS1307 rtc;


unsigned long time_since_last_NTP_update=0;
void Communication_protocols::handle_communications() {
    if(espPort.available()){
        const byte response_header = espPort.read();
        clear_receive_buffer();
        handle_header(response_header);
    }
    if(millis()-time_since_last_NTP_update>NTP_refresh_rate_) {
        send_request_SYN(get_time());
        NTP_success(false);
    }
}

void Communication_protocols::handle_header(const byte response_header)  {
    if(getFunction_id(response_header)==funct_id_getTime_) {
        if(getProtocol(response_header)==SYN_ACK) {
            Serial.println("Got SYN_ACK");
            NTP_success(NTP_response_handler());
        }else if(getProtocol(response_header)==FIN) {
            Serial.println("protocolFIN");
            NTP_success(false);
        }else {
            clear_receive_buffer();
        }
    }else {
        clear_receive_buffer();
    }
}



//throws TIMEOUT, SUCCESS, RETRY, ACK
bool Communication_protocols::NTP_response_handler()  {
    constexpr byte max_retries =40;
    send_response_ACK(funct_id_getTime_);
    if(!wait_for_response()) {
        send_status_TIMEOUT(funct_id_getTime_);
        return false;
    };

    byte retry_count=0;
    unsigned long time_out_start = millis();
    while(retry_count<max_retries) {
        if(millis()-time_out_start>=time_out_) {
            send_status_TIMEOUT(funct_id_getTime_);
            return false;
        }
        if(espPort.available()) {
            if(getTimeFromBuffer()) {
                send_status_SUCCESS(funct_id_getTime_);
                return true;
            }
            send_request_RETRY(funct_id_getTime_);
            time_out_start= millis();
            retry_count++;
        }
    }
    send_status_TIMEOUT(funct_id_getTime_);
    return false;
}


bool Communication_protocols::getTimeFromBuffer() {
    byte tim[4]{};
    for(int i=0;i<4;i++) {
        tim[i]=espPort.read();
        printBin(tim[i]);
        Serial.print(" ");
    }
    Serial.println();

    const byte crc_val = espPort.read();
    if(calcCRC8(tim, 4)==crc_val) {
        invert_stat_led();
        rtc.adjust(DateTime(bytesToLong(tim)));
        return true;
    }
    return false;
}


COMM_PROTOCOL Communication_protocols::get_response(const byte function_id) const {
    if(!wait_for_response())
        return TIMEOUT;
    while(espPort.available()) {
        const byte response_header = espPort.read();
        if(getFunction_id(response_header)==function_id) {
            if(getProtocol(response_header)==ACK) {
                return ACK;
            }if(getProtocol(response_header)==RETRY) {
                return RETRY;
            }if(getProtocol(response_header)==FIN) {
                return FIN;
            }if(getProtocol(response_header)==SUCCESS) {
                return SUCCESS;
            }
        }
    }
    clear_receive_buffer();
    return UNKW_ERR;
}



void Communication_protocols::NTP_success(const bool success) {
    time_since_last_NTP_update=millis();
    if(success) NTP_refresh_rate_ = retry_interval_onSucc_;
    else NTP_refresh_rate_ = retry_interval_onFail_;
}

void Communication_protocols::invert_stat_led() {
    constexpr byte STAT_LED_PIN=52;
    if(status_led_state_)
        digitalWrite(STAT_LED_PIN,HIGH);
    else
        digitalWrite(STAT_LED_PIN,LOW);
    status_led_state_=!status_led_state_;
}

bool Communication_protocols::wait_for_response() const {
    const unsigned long time_out_start = millis();
    while(!espPort.available())
        if(millis()-time_out_start>=time_out_)
            return false;
    return true;
}
void Communication_protocols::printBin(const byte aByte) {
    Serial.print('(');
    for (int8_t aBit = 7; aBit >= 0; aBit--) {
        Serial.print(bitRead(aByte, aBit) ? '1' : '0');
        if(aBit==4) Serial.print(' ');
    }
    Serial.print(')');
}
void Communication_protocols::printlnBin(const byte aByte) {
    printBin(aByte);
    Serial.println();
}
byte Communication_protocols::getProtocol(const byte b) {
    return b & PROTOCOL_FILTER;
}
byte Communication_protocols::getFunction_id(const byte response_header) const {
    return response_header & function_id_filter_;
}


COMM_PROTOCOL Communication_protocols::send_response_SYN_ACK(const byte function_id) const {
    send_header(function_id , SYN_ACK);
    return get_response(function_id);
}
void Communication_protocols::send_response_ACK(const byte function_id) {
    send_header(function_id , ACK);
}
void Communication_protocols::send_request_SYN(const byte function_id) {
    send_header(function_id , SYN);
}
void Communication_protocols::send_request_RETRY(const byte function_id) {
    send_header(function_id , RETRY);
}
void Communication_protocols::send_status_SUCCESS(const byte function_id) {
    send_header(function_id , SUCCESS);
}
void Communication_protocols::send_status_TIMEOUT(const byte function_id) {
    send_header(function_id , TIMEOUT);
}
void Communication_protocols::close_session(const byte function_id) {
    send_header(function_id , FIN);
}


COMM_PROTOCOL Communication_protocols::byte_to_enum(const byte protocol_id) {
    if(protocol_id==SYN)
        return static_cast<COMM_PROTOCOL>(protocol_id);
    if(protocol_id==SYN_ACK)
        return static_cast<COMM_PROTOCOL>(protocol_id);
    if(protocol_id==ACK)
        return static_cast<COMM_PROTOCOL>(protocol_id);
    if(protocol_id==FIN)
        return static_cast<COMM_PROTOCOL>(protocol_id);
    if(protocol_id==RETRY)
        return static_cast<COMM_PROTOCOL>(protocol_id);
    if(protocol_id==TIMEOUT)
        return static_cast<COMM_PROTOCOL>(protocol_id);
    if(protocol_id==UNKW_ERR)
        return static_cast<COMM_PROTOCOL>(protocol_id);
    if(protocol_id==SUCCESS)
        return static_cast<COMM_PROTOCOL>(protocol_id);
    if(protocol_id==PROTOCOL_FILTER)
        return static_cast<COMM_PROTOCOL>(protocol_id);
    return UNKW_ERR;
}


void Communication_protocols::send_header(const byte function_id, const byte protocol_id) {
    clear_receive_buffer();
    Serial.print("sending: ");
    printlnProtocol(byte_to_enum(protocol_id));
    espPort.write((function_id | protocol_id));
}
void Communication_protocols::clear_receive_buffer() {
    while(espPort.available()>0)espPort.read();
}


byte *Communication_protocols::longToByte(const unsigned long long_) {
    const auto a_arr = new byte[4]  {
        static_cast<byte>(long_ >> 24),
        static_cast<byte>(long_ >> 16),
        static_cast<byte>(long_ >> 8),
        static_cast<byte>(long_)
        };
    return a_arr;
}
unsigned long Communication_protocols::bytesToLong(const byte *byte_) {
    const unsigned int a =
        (static_cast<long>(byte_[0])<<24) |
        (static_cast<long>(byte_[1])<<16) |
        (static_cast<long>(byte_[2])<<8)  |
        (static_cast<long>(byte_[3]));
    return a;
}

void Communication_protocols::printlnProtocol(const COMM_PROTOCOL res_code) {
    printProtocol(res_code);
    Serial.println();
}

void Communication_protocols::printProtocol(const COMM_PROTOCOL res_code) {
    if(res_code==SYN)
        Serial.println("SYN");
    else if(res_code==SYN_ACK)
        Serial.println("SYN_ACK");
    else if(res_code==ACK)
        Serial.println("ACK");
    else if(res_code==FIN)
        Serial.println("FIN");
    else if(res_code==RETRY)
        Serial.println("RETRY");
    else if(res_code==TIMEOUT)
        Serial.println("TIMEOUT");
    else if(res_code==UNKW_ERR)
        Serial.println("UNKW_ERR");
    else if(res_code==SUCCESS)
        Serial.println("SUCCESS");
    else if(res_code==PROTOCOL_FILTER)
        Serial.println("PROTOCOL_FILTER");
    else
        Serial.println("INVALID PROTOCOL FORMAT");
}
