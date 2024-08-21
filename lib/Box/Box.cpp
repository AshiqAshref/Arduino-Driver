#include "Box.h"

extern Blink_Array blink_array;
void Box::isOpen(const boolean value) {
    if(value)
        Status_Directive::set_mode(this->box_no_, REMOVED);
    else if(this->isOpen_)
        set_status(status_);
    this->isOpen_=value;
}