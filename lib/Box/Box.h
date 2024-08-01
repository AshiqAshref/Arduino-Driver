//
// Created by user on 3/07/2024.
//

#ifndef BOX_H
#define BOX_H
#include <Arduino.h>
#include <Led_Coordinate.h>
#include <Pos_Coordinate.h>

class Box {
    const byte box_no;
    Pos_Coordinate position= Pos_Coordinate();
    Led_Coordinate led_address=Led_Coordinate();

    unsigned int med_id = 0;
    String medicine_name="";
    unsigned short medicine_amount=0;
public:
    Box();
    explicit Box(const byte box_no, const Pos_Coordinate &pos_coordinate)
        : box_no(box_no), position(pos_coordinate) {}


    void set_box(
        const unsigned int med_id,
        const String& medicine_name,
        const unsigned short medicine_amount
    ) {
        this->med_id = med_id;
        this->medicine_name = medicine_name;
        this->medicine_amount = medicine_amount;
    }


    byte get_box_no() const {return box_no;}
    unsigned int get_med_id() const {return this->med_id;}
    String get_name() {return medicine_name;}
    unsigned short get_medicne_amount() const {return medicine_amount;}

    void set_med_id(const unsigned int med_id) {this->med_id = med_id;}
    void set_name(String const& medicine_name) {this->medicine_name = medicine_name;}
    void set_medicne_amount(unsigned short const medicne_amount) {this->medicine_amount = medicne_amount;}
    String toString() const{
        return "{"
                R"("name" : ")" + medicine_name + "\""
                R"("box_no" : ")"+ static_cast<String>(box_no) +
                R"("medicine_amount" : ")"+ static_cast<String>(medicine_amount) +
                    "}";
    }
};

#endif //BOX_H
