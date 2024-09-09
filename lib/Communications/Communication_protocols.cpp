//
// Created by user on 19/08/2024.
//

#include "Communication_protocols.h"
#include <RTClib.h>
#include <CRC.h>
#include <ReminderB.h>
#include <ArduinoJson.h>
extern RTC_DS3231 rtc;


unsigned long time_since_last_NTP_update=0;
void Communication_protocols::handle_communications() {
    if(Serial1.available()){
        const byte response_header = Serial1.read();
        clear_receive_buffer();
        handle_header(response_header);
    }
    if(millis()-time_since_last_NTP_update>NTP_refresh_rate_) {
        get_time();
        NTP_success(false);
    }
}

void Communication_protocols::get_time() const {
    send_request_SYN(funct_id_getTime_);
}

void Communication_protocols::get_next_reminder_B(const unsigned long epoch) {
    get_next_time_ = epoch;
    send_request_SYN(funct_id_get_next_reminderB_);
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
    }else  if(getFunction_id(response_header)==funct_id_get_next_reminderB_) {
        if(getProtocol(response_header)==SYN_ACK) {
            Serial.println("Got SYN_ACK");
            get_reminder_b_response_handler();
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
bool Communication_protocols::get_reminder_b_response_handler()  {
    send_response_ACK(funct_id_get_next_reminderB_);
    sendLong(get_next_time_);
    if(!wait_for_response()) {
        send_status_TIMEOUT(funct_id_getTime_);
        return false;
    }

    unsigned long time_out_start = millis();
    constexpr byte max_retries =40;
    byte retry_count=0;
    while(retry_count<max_retries) {
        if(millis()-time_out_start>=time_out_) {
            send_status_TIMEOUT(funct_id_getTime_);
            return false;
        }
        if(Serial1.available()) {
            byte res = Serial.read();
            if(getFunction_id(res)==funct_id_get_next_reminderB_ && getProtocol(res)==TCP) {
                const byte total_sequence_number = Serial1.read();
                receive_reminderB_from_buffer(total_sequence_number);
            }
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

void Communication_protocols::send_tcp_ack(const byte function_id, byte recived_squence) {
    send_header(function_id , SYN_ACK);
    Serial1.write(recived_squence);
}
extern ReminderB *upcommingReminderB_;
extern Box boxes[];
void Communication_protocols::receive_reminderB_from_buffer(const byte total_sequence_number) {
    send_tcp_ack(funct_id_get_next_reminderB_, 0);
    auto reminder_b = new ReminderB();
    for(int i=1;i<=total_sequence_number;i++) {
        if(!wait_for_response()) return;
        JsonDocument doc;
        deserializeJson(doc, Serial1);
        if(i==1) {
            unsigned long time = doc["t"];
            reminder_b->set_time(new DateTime(time));
        }
        byte box_no = doc["bn"];
        boxes[--box_no].set_box(doc["mi"],doc["mn"],doc["ma"]);
        Medicine *medicine = new Medicine(&boxes[box_no],doc["md"]);
        reminder_b->add_medicine(medicine);

        send_tcp_ack(funct_id_get_next_reminderB_, i);
    }
    upcommingReminderB_ = reminder_b;
}


void Communication_protocols::sendLong(const unsigned long res_long) {
    const byte *res = longToByte(res_long);
    for(int i=0;i<4;i++)
        Serial1.write(res[i]);
    Serial1.write(calcCRC8(res,4));
    delete res;
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
        if(Serial1.available()) {
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
        tim[i]=Serial1.read();
        printBin(tim[i]);
        Serial.print(" ");
    }
    Serial.println();

    const byte crc_val = Serial1.read();
    if(calcCRC8(tim, 4)==crc_val) {
        invert_stat_led();
        Serial.print("Time : ");
        Serial.println(bytesToLong(tim));
        rtc.adjust(DateTime(bytesToLong(tim)));
        return true;
    }
    return false;
}


COMM_PROTOCOL Communication_protocols::get_response(const byte function_id) const {
    if(!wait_for_response())
        return TIMEOUT;
    while(Serial1.available()) {
        const byte response_header = Serial1.read();
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
    while(!Serial1.available())
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
    send_header(function_id ,
        RETRY);
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
    Serial1.write((function_id | protocol_id));
}
void Communication_protocols::clear_receive_buffer() {
    while(Serial1.available()>0)Serial1.read();
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
    return
        (static_cast<long>(byte_[0])<<24) |
        (static_cast<long>(byte_[1])<<16) |
        (static_cast<long>(byte_[2])<<8)  |
        (static_cast<long>(byte_[3]));
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
