#include "Blink_Array.h"


void Blink_Array::add(const byte box_no, const COLOR color) {
    const int result = find_index_by_box_no(box_no);
    if(result>=0)
        blinks[result]->set_color(color);
    else
        add(new Blink(box_no, color));
}

void Blink_Array::add(const byte box_no, const COLOR *color) {
    const int result = find_index_by_box_no(box_no);
    if(result>=0)
        blinks[result]->set_color(color);
    else
        add(new Blink(box_no, color));
}

void Blink_Array::add(Blink *blink) {
    auto temp_blink = new Blink*[this->blink_size_+1];
    for(byte i=0;i<this->blink_size_;i++)
        temp_blink[i] = this->blinks[i];
    temp_blink[this->blink_size_]= blink;
    delete []this->blinks;
    this->blinks = temp_blink;
    temp_blink = nullptr;
    this->blink_size_++;
}

Blink *Blink_Array::get_blink_by_index(const byte index) const {
    return this->blinks[index];
}

Blink *Blink_Array::get_blink_by_box(const byte box_no) const {
    const int result = find_index_by_box_no(box_no);
    if(result >=0)
        return this->blinks[result];
    return nullptr;
}

void Blink_Array::remove_by_index(const byte index) {
    auto temp_blink = new Blink*[this->blink_size_-1];
    byte temp_blink_next = 0;
    for(byte i=0; i<this->blink_size_;i++) {
        if(i==index)
            delete this->blinks[i];
        else
            temp_blink[temp_blink_next++] = this->blinks[i];
    }
    delete []this->blinks;
    this->blinks = temp_blink;
    temp_blink = nullptr;
    this->blink_size_--;
}


void Blink_Array::remove_by_boxNo(const byte box_no) {
    const int result = find_index_by_box_no(box_no);
    if(result >=0)
        remove_by_index(result);
}

void Blink_Array::blinkAll() const {
    for(int i=0;i<blink_size_;i++)
        blinks[i]->blink();
}

void Blink_Array::clearAll() {
    this->blink_size_=0;
    delete []this->blinks;
}

int Blink_Array::find_index_by_box_no(const byte box_no) const {
    for(int i=0;i<this->blink_size_;i++)
        if(this->blinks[i]->box_no()==box_no)
            return i;
    return -1;
}


