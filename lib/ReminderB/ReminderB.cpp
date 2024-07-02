#include "ReminderB.h"

#include <Box.h>

ReminderB::ReminderB(DateTime const * upc_p, boolean const * success_p) {
    upc = *upc_p;
    success = *success_p;
    boxes_ = new Box[0];
}
ReminderB::ReminderB() {
    upc = DateTime();
    success = false;
    boxes_ = new Box[0];
}


size_t ReminderB::get_boxes_size_test() {return this->boxes_size_test;}

void ReminderB::add_to_boxes_test(Box *box) {
    Box **temp_box= new Box*[boxes_size_test+1] ;
    for(size_t i=0; i<boxes_size_test;i++) {
        temp_box[i] = boxes_test[i];
    }
    temp_box[boxes_size_test]=box;
    *boxes_test = *temp_box;
    temp_box = nullptr;
    boxes_size_test++;
}

void ReminderB::remove_a_box_test(size_t index) {
    Box **temp_box =new Box*[boxes_size_test-1];
    delete []temp_box;

}
Box* ReminderB::get_a_box_test(size_t index) {

}


template<size_t size>
ReminderB::ReminderB(DateTime const * upc_p, Box (&boxes)[size], boolean const * success_p) {
    upc = *upc_p;
    boxes_ = boxes;
    success = *success_p;
}
Box *a[] = {new Box()};

void ReminderB::add_to_boxes(const Box *box) {

    // Box *temp_array = new Box[boxes_size+1];
    // // ReSharper disable once CppDFAConstantConditions
    // if(!temp_array){        Serial.println("\n\n\tOUT OF SPACEE IN CLASS\n\n"); return;}
    // for(size_t i=0; i<boxes_size; i++)
    //     temp_array[i] = boxes_[i];
    // delete []this->boxes_;
    // this->boxes_ = temp_array;
    // temp_array = nullptr;
    //
    // this->boxes_[boxes_size]=*box;
    // this->boxes_size++;
}

void ReminderB::remove_a_box( size_t index) {
    Box* temp_array = new Box[this->boxes_size-1];
    if(!temp_array){        Serial.println("\n\n\tOUT OF SPACEE IN CLASS REM\n\n"); return;}
    size_t temp_array_next = 0;
    Serial.print("Size..: ");
    Serial.print(this->boxes_size);
    for(size_t i=0; i<this->boxes_size; i++)
        if(i!=index) {
            temp_array[temp_array_next] = this->boxes_[i];
            temp_array_next= temp_array_next+1;
        }else {
            Serial.print(" Deleting..: ");
            Serial.println(i);
            Serial.println();
        }
    delete []this->boxes_;
    this->boxes_ = temp_array;
    temp_array=nullptr;
    this->boxes_size = temp_array_next;
}


DateTime & ReminderB::get_upc() {return upc;}
// String & ReminderB::get_boxes() {return boxes;}
Box * ReminderB::get_boxes() const {return boxes_;}
Box &ReminderB::get_a_box(const size_t index) const {return boxes_[index];}
boolean & ReminderB::get_success() {return success;}
size_t ReminderB::get_boxes_size() const { return boxes_size;}

void ReminderB::set_upc(DateTime const * upc_p) {upc = *upc_p;}
void ReminderB::set_success(boolean const *success_p) {success = *success_p;}

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
                    // ", \"boxes\":"   + boxes +
                    ", \"success\":" + success +
                "}";
    return a;
}