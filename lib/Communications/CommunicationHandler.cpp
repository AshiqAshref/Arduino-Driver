#include "CommunicationHandler.h"

#include <Command_activate_AP.h>
#include <Command_daylight_sav.h>
#include <Command_deactivate_ap.h>
#include <Command_get_network_inf.h>
#include <Command_get_reminderB.h>
#include <Command_get_time.h>
#include <Network_info.h>
#include <ReminderB.h>

unsigned long get_reminder_time_key = 0;
bool get_next_reminder_status=false;

extern ReminderB upcommingReminderB;
extern RTC_DS3231 rtc;
extern Box boxes[];
extern Network_info network_info;
IPAddress ip;

extern Command_get_time command_get_time;
extern Command_get_reminderB command_get_reminder_b;
extern Command_activate_AP command_activate_AP;
extern Command_deactivate_ap command_deactivate_ap;
extern Command_get_network_inf command_get_network_inf;
extern Command_daylight_sav command_daylight_sav;

constexpr byte commands_size=6;
Command *commands[commands_size]= {&command_get_time, &command_get_reminder_b, &command_activate_AP, &command_deactivate_ap, &command_get_network_inf, &command_daylight_sav}; // NOLINT(*-slicing, *-interfaces-global-init)


void CommunicationHandler::handle_header(const byte response_header) {
    for(int i=0;i<commands_size;i++) {
        if(getCommand(response_header)==commands[i]->command()) {
            if(getProtocol(response_header)==SYN_ACK) {
                commands[i]->response_handler();
            }else if(getProtocol(response_header)==SYN) {
                commands[i]->request_handler();
            }else if(getProtocol(response_header)==TIMEOUT) {
                commands[i]->set_status(FAILED);
            }else if(getProtocol(response_header)==UNKW_ERR) {
                commands[i]->set_status(FAILED);
            }else if(getProtocol(response_header)==SUCCESS) {
                commands[i]->set_status(COMPLETED);
            }else if(getProtocol(response_header)==FIN) {
                commands[i]->set_status(FAILED);
            }else {
                Serial.println("IDK WTF");
                send_status_UNKW_ERROR(commands[i]->command());
                break;
            }
        }
    }
    clear_receive_buffer();
}
void CommunicationHandler::handle_communications() {
    if(Serial1.available()){
        const byte response_header = Serial1.read();
        Serial.println();
        Serial.print("RCV_MAIN: ");
        printHeader(response_header);
        clear_receive_buffer();
        handle_header(response_header);
    }

    for(int i=0;i<commands_size;i++) {
        if(commands[i]->status()==IN_PROGRESS || commands[i]->status()==FAILED) {
            if(millis()-commands[i]->last_millis>commands[i]->retry_interval) {
                commands[i]->send_request();
            }
        }
    }
    if(millis()-command_get_time.last_millis>command_get_time.retry_interval) {
        command_get_time.send_request();
    }
}

bool CommunicationHandler::get_network_inf_request_handler() {
    Serial.println("NET_INF - REQ_H");
    constexpr Command_enum command = GET_NETWORK_INF;
    if(send_response_SYN_ACK(command)!=ACK) {close_session(command); return false;}
    send_status_SUCCESS(command);
    network_info.set_needs_update();
    return true;
}
bool CommunicationHandler::get_network_inf_response_handler() {
    if(!get_network_inf_response_handler_local()) {
        network_info.set_needs_update();
        return false;
    }
    return true;
}
bool CommunicationHandler::get_network_inf_response_handler_local() {
    Serial.println("NET_INF RES_H");
    constexpr Command_enum command = GET_NETWORK_INF;
    send_response_ACK(command);
    if(!wait_for_response(command)) return false;
    const byte net_stat = Serial1.read();
    if(!wait_for_response(command)) return false;
    if(Serial1.read()-10==net_stat) {
        Serial.println("crc pass");
        send_status_SUCCESS(command);
    }else {
        Serial.println("crc fail");
        close_session(command);
        return false;
    }
    Serial.print("net_stat: ");
    Serial.println(net_stat);
    if(net_stat==24) {
        const IPAddress ip = receive_IP(command);
        if(ip.toString() == "0.0.0.0") {
            close_session(command);
            return false;
        }
        network_info.set_ap_active(ip);
        send_status_SUCCESS(command);
        return true;
    }if(net_stat==60) {
        const IPAddress ip = receive_IP(command);
        if(ip.toString() == "0.0.0.0") {
            close_session(command);
            return false;
        }
        network_info.set_wifi_active(ip);
        send_status_SUCCESS(command);
        return true;
    }if(net_stat==36) {
        network_info.set_none_active();
        send_status_SUCCESS(command);
        return true;
    }
    network_info.set_none_active();
    close_session(command);
    return false;
}


bool CommunicationHandler::get_reminder_b_response_handler(const unsigned long get_reminder_time_key)  {
    Serial.println("REMB RES_H");
    constexpr Command_enum command = GET_REMINDER_B;
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

bool CommunicationHandler::deactivate_AP_response_handler() {
    Serial.println("DCT_AP RES_H");
    constexpr Command_enum command= DEACTIVATE_AP;
    send_response_ACK(command);
    const COMM_PROTOCOL response_code = get_response(command);
    if(response_code!=SUCCESS) {
        network_info.set_needs_update();
        close_session(command);
        return false;
    }
    network_info.set_needs_update();
    return true;
}
bool CommunicationHandler::deactivate_AP_request_handler()  {
    Serial.println("DCT_AP REQ_H");
    constexpr Command_enum command = DEACTIVATE_AP;
    if(send_response_SYN_ACK(command)!=ACK) {
        network_info.set_needs_update();
        close_session(command);
        return false;
    }
    send_status_SUCCESS(command);
    network_info.set_needs_update();
    return true;
}
bool CommunicationHandler::activate_AP_response_handler()  {
    Serial.println("ACT_AP RES_H");
    constexpr Command_enum command=ACTIVATE_AP;
    send_response_ACK(command);

    const COMM_PROTOCOL response_code = get_response(command);
    if(response_code!=READY_TO_SEND) {close_session(command); return false;}

    send_response_READY_TO_RECV(command);

    const IPAddress ip_address = receive_IP(command);
    Serial.println(ip_address);
    if(ip_address.toString()=="0.0.0.0") {
        Serial.print("Wrong IP ");
        close_session(command);
        return false;
    }
    network_info.set_ap_active(ip_address);
    return true;
}


bool CommunicationHandler::daylight_sav_response_handler(const bool daylight_sav,const bool command_type) {
    Serial.println("DLT_SV RES_H");
    constexpr Command_enum command=DAYLIGHT_SAV;
    send_response_ACK(command);

    if(command_type) {
        if(send_response_READY_TO_SEND(command)!=READY_TO_RECV) {close_session(command); return false;}
        return daylight_sav_send_dls(daylight_sav);
    }
    send_response_READY_TO_RECV(command);
    if(get_response(command)!=READY_TO_SEND){close_session(command); return false;}
    send_response_READY_TO_RECV(command);
    return daylight_sav_receive_dls();

}
bool CommunicationHandler::daylight_sav_request_handler(const bool daylight_sav) {
    Serial.println("DLT_SV REQ_H");
    constexpr Command_enum command = DAYLIGHT_SAV;
    if(send_response_SYN_ACK(command)!=ACK) {close_session(command); return false;}

    const COMM_PROTOCOL response_code = get_response(command);
    if(response_code==READY_TO_SEND) {
        send_response_READY_TO_RECV(command);
        return daylight_sav_receive_dls();
    }
    if(response_code==READY_TO_RECV) {
        if(send_response_READY_TO_SEND(command)!=READY_TO_RECV) {close_session(command); return false;}
        return daylight_sav_send_dls(daylight_sav);
    }
    return true;
}

bool CommunicationHandler::daylight_sav_receive_dls() {
    Serial.println("DLT_SV REQ_H");
    constexpr Command_enum command = DAYLIGHT_SAV;
    if(get_response(command)!=READY_TO_SEND) {close_session(command); return false;}
    send_response_READY_TO_RECV(command);

    if(!wait_for_response(command)) return false;
    const byte response = Serial.read();
    if(response==153) {
        network_info.set_daylight_saving(true);
    }else if(response==102) {
        network_info.set_daylight_saving(false);
    }else {
        close_session(command);
        return false;
    }
    send_status_SUCCESS(command);
    Serial.print("RCV SUCC DLS VAL: ");
    Serial.println(response);
    return true;
}

bool CommunicationHandler::daylight_sav_send_dls(const bool daylight_sav) {
    Serial.println("DLS-SV - RES_H");
    constexpr Command_enum command = DAYLIGHT_SAV;
    if(send_response_READY_TO_SEND(command)!=READY_TO_RECV) {close_session(command); return false;}
    daylight_sav?
        Serial1.write(153):
        Serial1.write(102);
    if(get_response(command)!=SUCCESS) {close_session(command); return false;}
    Serial.print("SENT SUCC DLS VAL: ");
    Serial.println(daylight_sav);
    return true;
}


bool CommunicationHandler::NTP_response_handler()  {
    Serial.println("NTP RES_H");
    constexpr Command_enum command = GET_TIME;
    send_response_ACK(command);
    const unsigned long value = receive_long(command);
    if(value) {
        setTime(value);
        send_status_SUCCESS(command);
        return true;
    }
    close_session(command);
    return false;
}
void CommunicationHandler::setTime(const unsigned long ux_time) {
    rtc.adjust(DateTime(ux_time));
    Serial.print("Time : ");
    Serial.println(ux_time);
}

void CommunicationHandler::resend_command_get_reminder_B() {
    Serial.println("REMB send SYN");
    send_request_SYN(GET_REMINDER_B);
}
void CommunicationHandler::send_command_get_time() {
    send_request_SYN(GET_TIME);
}
void CommunicationHandler::send_command_daylight_sav() {
    send_request_SYN(DAYLIGHT_SAV);
}
void CommunicationHandler::send_command_get_network_inf() {
    send_request_SYN(GET_NETWORK_INF);
}
void CommunicationHandler::send_command_get_reminder_B(const unsigned long epoch) {
    get_reminder_time_key = epoch;
    command_get_reminder_b.send_request(epoch);
}
void CommunicationHandler::send_command_activate_ap() {
    send_request_SYN(ACTIVATE_AP);
}
void CommunicationHandler::send_command_deactivate_ap() {
    send_request_SYN(DEACTIVATE_AP);
}

