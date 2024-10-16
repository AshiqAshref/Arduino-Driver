//
// Created by user on 19/08/2024.
//

#include "Communication_protocols.h"
#include <RTClib.h>
#include <ReminderB.h>
#include <ArduinoJson.h>
#include <CrcWriter.h>
#include <FastCRC.h>
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

void Communication_protocols::get_time() {
    send_request_SYN(funct_id_getTime_);
}

void Communication_protocols::get_next_reminder_B(const unsigned long epoch) {
    get_next_time_ = epoch;
    send_request_SYN(funct_id_get_reminderB_);
}



void Communication_protocols::handle_header(const byte response_header)  {
    if(getFunction_id(response_header)==funct_id_getTime_) {
        if(getProtocol(response_header)==SYN_ACK) {
            Serial.println("Got SYN_ACK");
            NTP_success(NTP_response_handler(funct_id_getTime_));
        }else if(getProtocol(response_header)==FIN) {
            Serial.println("protocolFIN");
            NTP_success(false);
        }else {
            clear_receive_buffer();
        }
    }else  if(getFunction_id(response_header)==funct_id_get_reminderB_) {
        if(getProtocol(response_header)==SYN_ACK) {
            Serial.println("Got SYN_ACK");
            get_reminder_b_response_handler(funct_id_get_reminderB_);
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
bool Communication_protocols::get_reminder_b_response_handler(const byte function_id) const {
    send_response_ACK(function_id);
    if(sendLong(get_next_time_, function_id)!= SUCCESS) return false;

    const JsonDocument doc = receive_jsonDocument(function_id);
    if(doc.size()==0) {
        send_status_UNKW_ERROR(function_id);
        return false;
    };
    return add_reminderb_to_class(doc);
}


byte extractHour(const String &formated_time) {
    return formated_time.substring(0, 2).toInt();
}
byte extractMinute(const String &formated_time) {
    return formated_time.substring(3, 5).toInt();
}


COMM_PROTOCOL Communication_protocols::sendJsonDocument(const JsonDocument &doc, const byte function_id, const byte max_retries) {
    COMM_PROTOCOL response_code = send_response_SYN_ACK(function_id);
    if(response_code!=ACK) return response_code;

    byte current_retries = 0;
    while(current_retries<max_retries) {
        if(current_retries>0) send_request_RETRY(function_id);

        CrcWriter writer;
        serializeMsgPack(doc, writer);
        response_code = sendLong(writer.hash(), funct_id_get_reminderB_);
        if(response_code!=SUCCESS) return response_code;

        response_code = send_response_SYN_ACK(function_id,false);
        if(response_code!=ACK) return response_code;
        serializeMsgPack(doc,Serial1);

        response_code=get_response(funct_id_get_reminderB_);
        if(response_code != RETRY) return response_code;
        current_retries++;
    }
    send_status_TIMEOUT(function_id);
    return TIMEOUT;
}

JsonDocument Communication_protocols::receive_jsonDocument(const byte function_id, const byte max_retries) {
    JsonDocument doc;
    COMM_PROTOCOL response_code = get_response(function_id);
    if(response_code!=SYN_ACK) return doc;
    send_response_ACK(function_id);

    byte current_retries = 0;
    while(current_retries<max_retries) {
        if(current_retries>0) {
            send_request_RETRY(function_id);
            response_code = get_response(function_id,false);
            if (response_code!=RETRY) return doc;
        }
        const uint32_t crc = getLongFromBuffer(function_id);
        if(!crc) {
            close_session(function_id);
            return doc;
        }

        if(get_response(function_id)!=SYN_ACK) {
            close_session(function_id);
            return doc;
        }
        send_response_ACK(function_id);

        if(!wait_for_response(function_id)) return doc;
        deserializeMsgPack(doc, Serial1);
        CrcWriter writer;
        serializeMsgPack(doc, writer);
        if(writer.hash() == crc) {
            send_status_SUCCESS(function_id);
            return doc;
        }
        doc.clear();
        current_retries++;
    }
    send_status_TIMEOUT(function_id);
    return doc;
}

COMM_PROTOCOL Communication_protocols::sendLong(const unsigned long res_long, const byte function_id) {
    const COMM_PROTOCOL rescode = send_response_SYN_ACK(function_id);
    if(rescode!=ACK) return rescode;

    constexpr byte max_retries = 20;
    byte current_retries = 0;
    while(current_retries<max_retries) {
        if(current_retries>0) send_request_RETRY(function_id);

        const byte *res = longToByte(res_long);
        for(int i=0;i<4;i++) Serial1.write(res[i]);
        FastCRC8 CRC8;
        Serial1.write(CRC8.smbus(res ,4));
        delete res;

        const COMM_PROTOCOL response_code =get_response(function_id);
        if(response_code != RETRY) return response_code;
        current_retries++;
    }
    send_status_TIMEOUT(function_id);
    return TIMEOUT;
}

unsigned long Communication_protocols::getLongFromBuffer(const byte function_id) {
    COMM_PROTOCOL rescode = get_response(function_id);
    if(rescode!=SYN_ACK) return 0;
    send_response_ACK(function_id);

    constexpr byte max_retries = 20;
    byte current_retries = 0;
    while(current_retries<max_retries) {
        if(current_retries>0) {
            send_request_RETRY(function_id);
            rescode = get_response(function_id,false);
            if (rescode!=RETRY) return 0;
        }
        byte long_[4]{};
        for(int i=0;i<4;i++) { // NOLINT(*-loop-convert)
            if(!wait_for_response(function_id)) return 0;
            long_[i]=Serial1.read();
        }
        if(!wait_for_response(function_id))return 0;
        const byte crc_val = Serial1.read();
        FastCRC8 CRC8;
        if(CRC8.smbus(long_, 4)==crc_val) {
            send_status_SUCCESS(function_id);
            return bytesToLong(long_);
        }
        current_retries++;
    }
    send_status_TIMEOUT(function_id);
    return 0;
}

extern ReminderB *upcommingReminderB;
extern Box boxes[];
bool Communication_protocols::add_reminderb_to_class(JsonDocument doc) {
    auto reminder_b = new ReminderB();
    reminder_b->set_time_id(doc["ti"]);
    const String time_str = doc["t"].as<String>();
    reminder_b->set_time(new DateTime(0,0,0,extractHour(time_str), extractMinute(time_str)));
    for(int i = 0;i<doc["m"].size();i++) {
        byte box_no = doc["m"][i]["b"];
        boxes[--box_no].set_box(doc["b"],doc["b"].as<String>(),doc["d"]);
        const auto medicine = new Medicine(&boxes[box_no],doc["d"]);
        reminder_b->add_medicine(medicine);
    }
    delete upcommingReminderB;
    upcommingReminderB = reminder_b;
    reminder_b=nullptr;
    return true;
}

void Communication_protocols::send_tcp_ack(const byte function_id, const byte recived_squence) {
    send_header(function_id , SYN_ACK);
    Serial1.write(recived_squence);
}





//throws TIMEOUT, SUCCESS, RETRY, ACK
bool Communication_protocols::NTP_response_handler(const byte function_id)  {
    constexpr byte max_retries =40;
    send_response_ACK(function_id);
    if(!wait_for_response()) {
        send_status_TIMEOUT(function_id);
        return false;
    };

    byte retry_count=0;
    while(retry_count<max_retries) {
        if(wait_for_response()) {
            send_status_TIMEOUT(function_id);
            return false;
        }

        const unsigned long value = getLongFromBuffer(function_id);
        if(value) {
            send_status_SUCCESS(function_id);
            setTime(value);
            return true;
        }
        return false;
    }
    send_status_TIMEOUT(function_id);
    return false;
}







void Communication_protocols::setTime(const unsigned long ux_time) {
    invert_stat_led();
    Serial.print("Time : ");
    Serial.println(ux_time);
    rtc.adjust(DateTime(ux_time));
}


COMM_PROTOCOL Communication_protocols::get_response(const byte function_id, const bool clear_buffer)  {
    if(!wait_for_response())
        return TIMEOUT;
    while(Serial1.available()) {
        const byte response_header = Serial1.read();
        if(getFunction_id(response_header)==function_id) {
            if(getProtocol(response_header)==ACK) {
                return ACK;
            }if(getProtocol(response_header)==SYN_ACK) {
                return SYN_ACK;
            }if(getProtocol(response_header)==RETRY) {
                return RETRY;
            }if(getProtocol(response_header)==FIN) {
                return FIN;
            }if(getProtocol(response_header)==SUCCESS) {
                return SUCCESS;
            }
        }
        if(clear_buffer) clear_receive_buffer();
    }
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

bool Communication_protocols::wait_for_response() {
    const unsigned long time_out_start = millis();
    while(!Serial1.available())
        if(millis()-time_out_start>=time_out_)
            return false;
    return true;
}
bool Communication_protocols::wait_for_response(const byte function_id) {
    const unsigned long time_out_start = millis();
    while(!Serial1.available())
        if(millis()-time_out_start>=time_out_) {
            send_status_TIMEOUT(function_id);
            return false;
        }
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
byte Communication_protocols::getFunction_id(const byte response_header) {
    return response_header & function_id_filter_;
}


COMM_PROTOCOL Communication_protocols::send_response_SYN_ACK(const byte function_id, const bool clear_buffer) {
    send_header(function_id , SYN_ACK);
    return get_response(function_id, clear_buffer);
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
void Communication_protocols::send_status_UNKW_ERROR(const byte function_id) {
    send_header(function_id , UNKW_ERR);
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
