#include "ReminderB.h"

ReminderB::ReminderB(DateTime const * upc_p, String const * boxes_p, boolean const * success_p) {
    upc = *upc_p;
    boxes = *boxes_p;
    success = *success_p;
}
ReminderB::ReminderB() {
    upc = DateTime();
    boxes = "0";
    success = false;
}

DateTime & ReminderB::get_upc() {return upc;}
String & ReminderB::get_boxes() {return boxes;}
boolean & ReminderB::get_success() {return success;}

void ReminderB::set_upc(DateTime const * upc_p) {upc = *upc_p;}
void ReminderB::set_boxes(String const * boxes_p) {boxes = *boxes_p;}

String ReminderB::printArray(){return boxes;}

String ReminderB::toString() const{
    String time = "";
    if (upc.hour() <= 9) {
        time += '0' + String(upc.hour());
    } else {
        time += String(upc.hour());
    }
    time += ':';
    if (upc.minute() <= 9) {
        time += '0' + String(upc.minute());
    } else {
        time += String(upc.minute());
    }
    time += "";

    String a = "{"
                    "\"time\" : "  + time +
                    ", \"boxes\":"   + boxes +
                    ", \"success\":" + success +
                "}";
    return a;
}