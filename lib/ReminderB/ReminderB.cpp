#include "ReminderB.h"

#include <Box.h>

ReminderB::ReminderB(DateTime const * upc_p, String const * boxes_p, boolean const * success_p) {
    upc = *upc_p;
    boxes = *boxes_p;
    success = *success_p;
    boxes_ = new Box[0];
}
ReminderB::ReminderB() {
    upc = DateTime();
    boxes = "0";
    success = false;
    boxes_ = new Box[0];
}


template<size_t size>
ReminderB::ReminderB(DateTime const * upc_p, Box (&boxes)[size], boolean const * success_p) {
    upc = *upc_p;
    boxes_ = boxes;
    success = *success_p;
}

void ReminderB::add_to_boxes(const Box *box) {
    Box *tempBox = new Box[boxes_size+1];
    // ReSharper disable once CppDFAConstantConditions
    if(!tempBox){ Serial.println("\n\n\tOUT OF SPACEE IN CLASS\n\n"); return;}

    for(size_t i=0;i<boxes_size;i++) {
        tempBox[i] = boxes_[i];
    }

    delete []boxes_;
    boxes_ = tempBox;
    tempBox =nullptr;
    // this->boxes_ = new Box[boxes_size+1];
    // for(size_t i=0; i<boxes_size; i++) {
    //     boxes_[i] = * new Box(tempBox[i]);
    //     delete &tempBox[i];
    // }


    this->boxes_[boxes_size]=*box;
    boxes_size++;

    // delete[]tempBox;


    // memmove(tempBox,boxes_,10);
    // for(size_t i= 0; i< this->boxes_size; i++)
    //     tempBox[i]= this->boxes_[i];
    // tempBox[this->boxes_size] = *box;
    // this->boxes_size++;
    // memcpy(this->boxes_, tempBox, boxes_size);
    // this->boxes_= tempBox;
}
void ReminderB::remove_a_box(const size_t index) {
    auto new_array = new Box[this->boxes_size-1];
    for(size_t i =0;i<(this->boxes_size-1);i++) {
        if(i==index) delete &new_array[i];
        else new_array[i] = this->boxes_[i];
    }
    this->boxes_size--;
    delete this->boxes_;
    this->boxes_ = new_array;
}


DateTime & ReminderB::get_upc() {return upc;}
// String & ReminderB::get_boxes() {return boxes;}
Box * ReminderB::get_boxes() const {return boxes_;}
Box ReminderB::get_a_box(const size_t index) const {return boxes_[index];}
boolean & ReminderB::get_success() {return success;}
size_t ReminderB::get_boxes_size() const { return boxes_size;}

void ReminderB::set_upc(DateTime const * upc_p) {upc = *upc_p;}
void ReminderB::set_boxes(String const * boxes_p) {boxes = *boxes_p;}
void ReminderB::set_success(boolean const *success_p) {success = *success_p;}

String ReminderB::printArray(){return boxes;}
String ReminderB::toString() const{
    String time = "";
    if (upc.hour() <= 9) {
        time += '0' + String(upc.hour());
    } else {
        time += String(upc.hour());
    }
    time += ':';
    if (upc.minute() <= 9) {
        time += '0' + String(upc.minute());
    } else {
        time += String(upc.minute());
    }
    time += "";

    String a = "{"
                    "\"time\" : "  + time +
                    ", \"boxes\":"   + boxes +
                    ", \"success\":" + success +
                "}";
    return a;
}