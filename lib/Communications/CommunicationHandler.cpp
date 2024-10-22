//
// Created by user on 22/10/2024.
//

#include "CommunicationHandler.h"

#include <ReminderB.h>
unsigned long time_since_last_get_reminder_b_fail=0;
unsigned long time_since_last_NTP_update=0;
unsigned long NTP_refresh_rate_ = 0;
unsigned long get_reminder_time_key = 0;
bool get_next_reminder_status=false;

extern ReminderB upcommingReminderB;
extern RTC_DS3231 rtc;
extern Box boxes[];
IPAddress ip;
struct  {
private:
    bool AP_active=true;
public:
    IPAddress IP;
    const unsigned long refresh_value=6000;
    unsigned long time_since_last_display_refresh = 0;
    void update_screen() {
        this->time_since_last_display_refresh=millis();
        Lcd_Menu::lcdClear();
        Lcd_Menu::printCenter("AP_active",0);
        Lcd_Menu::printCenter(this->IP.toString(),1);
    };
    void on_ap_deactivate_display() const{
        Lcd_Menu::lcdClear();
        Lcd_Menu::printCenter("AP_ClOSED",0);
        delay(3000);
    };
    bool is_active() const {return this->AP_active;}
    void set_active(const bool AP_active_) {
        this->AP_active=AP_active_;
        if(AP_active_)
            this->on_ap_deactivate_display();
    }
}ap_status;


void CommunicationHandler::handle_communications() {
    if(Serial1.available()){
        const byte response_header = Serial1.read();
        clear_receive_buffer();
        handle_header(response_header);
    }
    if(millis()-time_since_last_NTP_update>NTP_refresh_rate_) {
        get_time();
        NTP_success(false);
    }
    if(ap_status.is_active())
        if(millis()-ap_status.time_since_last_display_refresh>ap_status.refresh_value)
            ap_status.update_screen();


    if(!get_next_reminder_status){
        if(millis()-time_since_last_get_reminder_b_fail>get_reminder_refresh_rate_) {
            get_next_reminder_B(get_reminder_time_key);
        }
    }
}

void CommunicationHandler::handle_header(const byte response_header)  {
    if(getCommand(response_header)==GET_TIME) {
        if(getProtocol(response_header)==SYN_ACK) {
            Serial.println("Got SYN_ACK");
            NTP_success(NTP_response_handler(GET_TIME));
        }else if(getProtocol(response_header)==FIN) {
            Serial.println("protocolFIN");
            NTP_success(false);
        }else {
            clear_receive_buffer();
        }

    }else  if(getCommand(response_header)==GET_REMINDER_B) {
        if(getProtocol(response_header)==SYN_ACK) {
            Serial.println("Got SYN_ACK_");
            reminder_b_success(get_reminder_b_response_handler(GET_REMINDER_B));
        }else if(getProtocol(response_header)==FIN) {
            Serial.println("protocolFIN");
        }else {
            clear_receive_buffer();
        }
    }else  if(getCommand(response_header)==ACTIVATE_AP) {
        if(getProtocol(response_header)==SYN_ACK) {
            Serial.println("Got SYN_ACK_");
            activate_AP_response_handler(ACTIVATE_AP);
        }else if(getProtocol(response_header)==FIN) {
            Serial.println("protocolFIN");
        }else {
            clear_receive_buffer();
        }
    }else  if(getCommand(response_header)==DEACTIVATE_AP) {
        if(getProtocol(response_header)==SYN_ACK) {
            Serial.println("Got SYN_ACK_");
            activate_AP_response_handler(ACTIVATE_AP);
        }else if(getProtocol(response_header==SYN)) {
            deactivate_AP_request_handler(DEACTIVATE_AP);

        }else if(getProtocol(response_header)==FIN) {
            Serial.println("protocolFIN");
        }else {
            clear_receive_buffer();
        }
    }else {
        clear_receive_buffer();
    }
}

// throws TIMEOUT, SUCCESS, RETRY, ACK
// ReSharper disable once CppDFAConstantParameter
bool CommunicationHandler::get_reminder_b_response_handler(const Commands command)  {
    send_response_ACK(command);
    if(sendLong(get_reminder_time_key, command)!= SUCCESS) return false;

    const JsonDocument doc = receive_jsonDocument(command);
    if(doc.size()==0) {
        send_status_UNKW_ERROR(command);
        return false;
    }
    add_reminderb_to_class(doc);
    return true;
}

void CommunicationHandler::send_command_deactivate_ap() {
    send_request_SYN(DEACTIVATE_AP);
}

bool CommunicationHandler::deactivate_AP_response_handler(const Commands command) {
    if(!send_response_SYN_ACK(command)==ACK) return false;
    send_status_SUCCESS(command);
    ap_status.set_active(false);
    return true;
}
bool CommunicationHandler::deactivate_AP_request_handler(const Commands command)  {
    send_response_ACK(command);
    const COMM_PROTOCOL response_code = get_response(command);
    if(response_code!=SUCCESS) {
        ap_status.set_active(true);
        return false;
    }
    ap_status.set_active(false);
    return true;
}


bool CommunicationHandler::activate_AP_response_handler(const Commands command)  {
    send_response_ACK(command);

    const IPAddress doc = receive_IP(command);
    if(doc.toString().length()==0) {
        send_status_UNKW_ERROR(command);
        return false;
    }
    ap_status.IP=doc;
    ap_status.set_active(true);
    return true;
}

// throws TIMEOUT, SUCCESS, RETRY, ACK
// ReSharper disable once CppDFAConstantParameter
bool CommunicationHandler::NTP_response_handler(const Commands command)  {
    send_response_ACK(command);
    const unsigned long value = receive_long(command);
    if(value) {
        send_status_SUCCESS(command);
        setTime(value);
        return true;
    }
    return false;
}


void CommunicationHandler::reminder_b_success(const bool success) {
    time_since_last_get_reminder_b_fail=millis();
    get_next_reminder_status = success;
}

void CommunicationHandler::NTP_success(const bool success) {
    time_since_last_NTP_update=millis();
    if(success) NTP_refresh_rate_ = retry_interval_onSucc_;
    else NTP_refresh_rate_ = retry_interval_onFail_;
}
void CommunicationHandler::setTime(const unsigned long ux_time) {
    invert_stat_led();
    Serial.print("Time : ");
    Serial.println(ux_time);
    rtc.adjust(DateTime(ux_time));
}
void CommunicationHandler::add_reminderb_to_class(JsonDocument doc) {
    upcommingReminderB.clear();
    const String time_str = doc["t"].as<String>();
    upcommingReminderB.set_time_id(doc["ti"]);
    upcommingReminderB.set_time(new DateTime(0,0,0,extractHour(time_str), extractMinute(time_str)));
    for(size_t i = 0;i<doc["m"].size();i++) {
        byte box_no = doc["m"][i]["b"];--box_no;
        const auto medicine = new Medicine(&boxes[box_no],doc["d"], doc["s"]);
        upcommingReminderB.add_medicine(medicine);
    }
}




void CommunicationHandler::get_next_reminder_B(const unsigned long epoch) {
    get_reminder_time_key = epoch;
    send_request_SYN(GET_REMINDER_B);
}

void CommunicationHandler::get_time() {
    send_request_SYN(GET_TIME);
}
