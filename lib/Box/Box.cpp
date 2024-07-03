
#include "Box.h"

Box::Box(const byte box_no, const String &medicine_name, const unsigned short medicine_amount) {
    this->box_no = box_no;
    this->medicine_name = medicine_name;
    this->medicine_amount = medicine_amount;
}

Box::Box(const String& medicine_name, const byte box_no) {
    this->medicine_name = medicine_name;
    this->box_no = box_no;
}

byte Box::get_box_no() const {return box_no;}
unsigned int Box::get_med_id() const {return this->med_id;}
String Box::get_name() {return medicine_name;}
unsigned short Box::get_medicne_amount() const {return medicine_amount;}
unsigned long Box::get_position() const {return this->position;}

void Box::set_med_id(const unsigned int med_id) {this->med_id = med_id;}
void Box::set_name(String const &name_p) {medicine_name = name_p;}
void Box::set_box_no(byte const box_no) {this->box_no = box_no;}
void Box::set_medicne_amount(unsigned short const medicne_amount) {this->medicine_amount = medicne_amount;}
void Box::set_position(const unsigned long position) {this->position = position;}

String Box::toString() const{
    return "{"
            R"("name" : ")" + medicine_name + "\""
            R"("box_no" : ")"+ static_cast<String>(box_no) +
            R"("medicine_amount" : ")"+ static_cast<String>(medicine_amount) +
                "}";
}