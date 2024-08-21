//
// Created by user on 8/08/2024.
//

#ifndef STATUS_DIRECTIVE_H
#define STATUS_DIRECTIVE_H
#include <Arduino.h>
#include <Status.h>


class Status_Directive {
public:
    Status_Directive()=default;

    static void set_mode(byte boxNo, Status status);

    static void mode_default(byte boxNo);
    static void mode_current(byte boxNo);
    static void mode_removed(byte boxNo);
    static void mode_upcomming(byte boxNo);
    static void mode_missed(byte boxNo);
    static void mode_empty(byte boxNo);
    static void mode_not_in_use(byte boxNo);
    static void mode_running_low(byte boxNo);
};



#endif //STATUS_DIRECTIVE_H
