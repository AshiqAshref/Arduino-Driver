//
// Created by user on 3/07/2024.
//

#ifndef BOX_H
#define BOX_H
#include <Arduino.h>

class Box {
    byte box_no=0;
    unsigned int med_id = 0;
    String medicine_name="";
    unsigned short medicine_amount=0;
    unsigned long position=0;
public:
    Box(byte box_no, const String &medicine_name, unsigned short medicine_amount);
    Box(const String& medicine_name, byte box_no);
    Box()=default;
    ~Box()=default;

    byte get_box_no() const;
    unsigned int get_med_id() const;
    String get_name();
    unsigned short get_medicne_amount() const;
    unsigned long get_position()const;

    void set_box_no(byte box_no);
    void set_med_id(unsigned int med_id);
    void set_name(String const &name_p);
    void set_medicne_amount(unsigned short medicne_amount);
    void set_position(unsigned long position);


    String toString() const;
};

#endif //BOX_H
