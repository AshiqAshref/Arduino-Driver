//
// Created by user on 24/07/2024.
//

#ifndef BLINK_H
#define BLINK_H
#include <Arduino.h>
#include <AV_Functions.h>
#include <COLOR.h>


class Blink {
    unsigned long blinkPrevioustime_=0;
    bool blinkState_=false;
    unsigned int delay_=500;
    byte boxNo_;
    COLOR color_a;
    COLOR color_b=COLOR::CLEAR;
    bool color_single;
    void blink_ch() const;
    void blink_st() const;

public:
    Blink(const byte box_no, const COLOR *color)
        : boxNo_(box_no)
    {
        color_a =color[0];
        color_b =color[1];
        color_single=false;
    }
    Blink(const int delay, const byte box_no, const COLOR *color)
        : delay_(delay),
        boxNo_(box_no)
    {
        color_a =color[0];
        color_b =color[1];
        color_single=false;
    }
    Blink(const byte box_no, const COLOR color)
        : boxNo_(box_no),
          color_a(color) {
        color_single=true;
    }
    Blink(const int delay, const byte box_no, const COLOR color)
        : delay_(delay),
          boxNo_(box_no),
          color_a(color){
        color_single=true;
    }
    ~Blink()=default;

    void blink();

    byte &box_no() {return boxNo_;}
    void set_box_no(const byte box_no) {boxNo_ = box_no;}


    String color() const {
        if(color_single) return String(AV_Functions::enum_to_char(color_a));
        return String(AV_Functions::enum_to_char(color_a))+String(AV_Functions::enum_to_char(color_b));
    }

    void set_color(const COLOR color) {
        this->color_a = color;
        this->color_single = true;
    }
    void set_color(const COLOR *color) {
        this->color_a = color[0];
        this->color_b = color[1];
        this->color_single = false;
    }

    unsigned int &get_delay() {return delay_;}
    void set_delay(const int delay) {this->delay_ = delay;}

    String toString() const  {
        return R"({)"
            R"(boxNo: )" + static_cast<String>(boxNo_) +
            R"( color: )" + color()+
                "}";
    }
};



#endif //BLINK_H
