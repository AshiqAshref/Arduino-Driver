//
// Created by user on 8/08/2024.
//

#ifndef STATUS_DIRECTIVE_H
#define STATUS_DIRECTIVE_H
#include <Arduino.h>
enum BoxStatus {
    BOX_STATUS_DEFAULT,
    BOX_STATUS_CURRENT,
    BOX_STATUS_REMOVED,
    BOX_STATUS_COMPLETED,
    BOX_STATUS_MISSED,
    BOX_STATUS_EMPTY,
    BOX_STATUS_NOT_IN_USE,
    BOX_STATUS_RUNNING_LOW,
    BOX_STATUS_UNLOCKING
};

class Status_Directive {
public:
    Status_Directive()=default;

    static void set_mode(byte boxNo, BoxStatus status);

    static void mode_default(byte boxNo);
    static void mode_current(byte boxNo);
    static void mode_removed(byte boxNo);
    static void mode_unlocking(byte boxNo);
    static void mode_upcomming(byte boxNo);
    static void mode_missed(byte boxNo);
    static void mode_empty(byte boxNo);
    static void mode_not_in_use(byte boxNo);
    static void mode_running_low(byte boxNo);
};



#endif //STATUS_DIRECTIVE_H
