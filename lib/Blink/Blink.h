//
// Created by user on 24/07/2024.
//

#ifndef BLINK_H
#define BLINK_H
#include <Arduino.h>

class Blink {
    unsigned long blinkPrevioustime_=0;
    bool blinkState_=false;
    unsigned int delay_=500;
    byte boxNo_;
    char color_;
public:
    Blink(const byte box_no, const char color)
        : boxNo_(box_no),
          color_(color) {
    }
    Blink(const int delay, const byte box_no, const char color)
        : delay_(delay),
          boxNo_(box_no),
          color_(color) {
    }
    ~Blink()=default;

    void blink();

    byte & box_no() {return boxNo_;}
    void set_box_no(const byte box_no) {boxNo_ = box_no;}

    char & color() {return color_;}
    void set_color(const char color) {this->color_ = color;}

    unsigned int &get_delay() {return delay_;}
    void set_delay(const int delay) {this->delay_ = delay;}
};



#endif //BLINK_H
