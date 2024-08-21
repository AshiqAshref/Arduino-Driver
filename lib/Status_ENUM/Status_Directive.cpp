#include "Status_Directive.h"
#include <Blink_Array.h>
#include <Led_indicator.h>
#include <COLOR.h>

extern Led_Indicator led_indicator;
extern Blink_Array blink_array;

void Status_Directive::set_mode(const byte boxNo, const Status status) {
    switch (status) {
        case DEFAULT_:
            mode_default(boxNo);
            break;
        case CURRENT:
            mode_current(boxNo);
            break;
        case REMOVED:
            mode_removed(boxNo);
            break;
        case UPCOMMING:
            mode_upcomming(boxNo);
            break;
        case MISSED:
            mode_missed(boxNo);
            break;
        case EMPTY:
            mode_empty(boxNo);
            break;
        case NOT_IN_USE:
            mode_not_in_use(boxNo);
            break;
        case RUNNING_LOW:
            mode_running_low(boxNo);
            break;
        default:
            break;
    }
}


void Status_Directive::mode_default(const byte boxNo) {
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo, COLOR::BLUE);
}

void Status_Directive::mode_current(const byte boxNo) {
    blink_array.add(boxNo,COLOR::GREEN);
}

void Status_Directive::mode_removed(const byte boxNo) {
    blink_array.add(boxNo,COLOR::RED);
}

void Status_Directive::mode_upcomming(const byte boxNo) {
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo,COLOR::GREEN);
}

void Status_Directive::mode_missed(const byte boxNo) {
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo,COLOR::RED);
}

void Status_Directive::mode_empty(const byte boxNo) {
    constexpr COLOR color[2] = {COLOR::RED,COLOR::GREEN};
    blink_array.add(boxNo, color);
}

void Status_Directive::mode_not_in_use(const byte boxNo) {
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo,COLOR::CLEAR);
}

void Status_Directive::mode_running_low(const byte boxNo) {
    constexpr COLOR color[]= {COLOR::RED,COLOR::GREEN};
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo, color);
}








