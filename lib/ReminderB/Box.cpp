#include "Box.h"

Box::Box(int const *box_no_p, String const *name_p, int const *no_of_pills_p) {
    box_no = *box_no_p;
    name = *name_p;
    no_of_pills = *no_of_pills_p;
}
Box::Box(const String& name, const byte box_no) {
    this->name = name;
    this->box_no = box_no;
}


byte Box::get_dosage() const { return this->dosage;}


Box::Box(Box& t) {
    this->box_no = t.box_no;
    this->name = t.name;
    this->no_of_pills = t.no_of_pills;
    this->dosage = t.dosage;
}

void Box::deleteThis() const {
    delete this;
}

Box::~Box() {
}

int  Box::get_box_no() const {return box_no;}
String  Box::get_name() {return name;}
int  Box::get_no_of_pills() const {return no_of_pills;}

void Box::set_box_no(int const *box_no_p) {box_no = *box_no_p;}
void Box::set_name(String const *name_p) {name = *name_p;}
void Box::set_no_of_pills(int const *no_of_pills_p) {no_of_pills = *no_of_pills_p;}

void Box::toString() const{
    Serial.println(" Box_No: "+ String(box_no) + " Name: " + name + " Pills: "+ no_of_pills);
}