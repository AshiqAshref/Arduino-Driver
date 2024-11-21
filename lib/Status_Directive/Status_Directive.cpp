#include "Status_Directive.h"
#include <Blink_Array.h>
#include <Led_indicator.h>

extern Led_Indicator led_indicator;
extern Blink_Array blink_array;

void Status_Directive::set_mode(const byte boxNo, const BoxStatus status) {
    switch (status) {
        case BOX_STATUS_DEFAULT:
            mode_default(boxNo);
            break;
        case BOX_STATUS_CURRENT:
            mode_current(boxNo);
            break;
        case BOX_STATUS_REMOVED:
            mode_removed(boxNo);
            break;
        case BOX_STATUS_UPCOMMING:
            mode_upcomming(boxNo);
            break;
        case BOX_STATUS_MISSED:
            mode_missed(boxNo);
            break;
        case BOX_STATUS_EMPTY:
            mode_empty(boxNo);
            break;
        case BOX_STATUS_NOT_IN_USE:
            mode_not_in_use(boxNo);
            break;
        case BOX_STATUS_RUNNING_LOW:
            mode_running_low(boxNo);
            break;
        case BOX_STATUS_UNLOCKING:
            mode_running_low(boxNo);
            break;
        default:
            break;
    }
}


void Status_Directive::mode_default(const byte boxNo) {
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo, COLOR_BLUE);
}

void Status_Directive::mode_current(const byte boxNo) {
    blink_array.add(boxNo,COLOR_GREEN);
}

void Status_Directive::mode_removed(const byte boxNo) {
    blink_array.add(boxNo,COLOR_RED);
}

void Status_Directive::mode_unlocking(const byte boxNo) {
    blink_array.add(boxNo,COLOR_BLUE);
}

void Status_Directive::mode_upcomming(const byte boxNo) {
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo,COLOR_GREEN);
}

void Status_Directive::mode_missed(const byte boxNo) {
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo,COLOR_RED);
}

void Status_Directive::mode_empty(const byte boxNo) {
    constexpr COLOR color[2] = {COLOR_RED,COLOR_GREEN};
    blink_array.add(boxNo, color);
}

void Status_Directive::mode_not_in_use(const byte boxNo) {
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo,COLOR_CLEAR);
}

void Status_Directive::mode_running_low(const byte boxNo) {
    constexpr COLOR color[]= {COLOR_RED,COLOR_GREEN};
    blink_array.remove_by_boxNo(boxNo);
    led_indicator.setColor(boxNo, color);
}








