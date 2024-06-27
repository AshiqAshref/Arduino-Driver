#include "Box.h"

Box::Box(int const *box_no_p, String const *name_p, int const *no_of_pills_p) {
    box_no = *box_no_p;
    name = *name_p;
    no_of_pills = *no_of_pills_p;
}

int & Box::get_box_no() {return box_no;}
String & Box::get_name() {return name;}
int & Box::get_no_of_pills() {return no_of_pills;}

void Box::set_box_no(int const *box_no_p) {box_no = *box_no_p;}
void Box::set_name(String const *name_p) {name = *name_p;}
void Box::set_no_of_pills(int const *no_of_pills_p) {no_of_pills = *no_of_pills_p;}

void Box::toString() const{
    Serial.println(" No: "+ String(box_no) + " Name: " + name + "Pills: "+ no_of_pills);
}