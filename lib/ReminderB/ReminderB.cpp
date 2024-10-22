#include "ReminderB.h"
#include <Medicine.h>

ReminderB::ReminderB(DateTime *time) {
    delete this->time;
    this->time = time;
    isValid_ = true;
}

ReminderB::ReminderB(DateTime *time, Medicine** medicines) {
    delete time;
    this->time = time;
    delete []this->medicines;
    this->medicines = medicines;
    isValid_ = true;
}

void ReminderB::add_medicine(Medicine *medicine) {
    auto temp_medicine= new Medicine*[this->medicines_size+1] ;
    for(byte i=0; i<this->medicines_size;i++) {
        temp_medicine[i] = this->medicines[i];
    }
    temp_medicine[this->medicines_size]=medicine;
    delete []this->medicines;
    this->medicines = temp_medicine;
    temp_medicine = nullptr;
    this->medicines_size++;
}

void ReminderB::remove_medicine(const byte index) {
    auto temp_medicine = new Medicine*[this->medicines_size-1];
    byte temp_medicine_next = 0;
    for(byte i=0; i<this->medicines_size;i++) {
        if(i==index)
            delete this->medicines[i];
        else
            temp_medicine[temp_medicine_next++] = this->medicines[i];
    }
    delete []this->medicines;
    this->medicines = temp_medicine;
    temp_medicine = nullptr;
    this->medicines_size--;
}

bool ReminderB::check_for_alarm(const DateTime &curr_time) {
    if(isValid_)
        if(time->hour()==curr_time.hour() && time->minute()==curr_time.minute()) {
            isValid_=false;
            return true;
        }
    return false;
}

unsigned int ReminderB::get_time_id() const {return time_id;}
DateTime *ReminderB::get_time() const {return this->time;}
Medicine *ReminderB::get_medicine(const byte index) const {return this->medicines[index];}
byte ReminderB::get_medicine_size() const {return this->medicines_size;}

void ReminderB::clear() {
    time_id = 0;
    for(int i=0;i<medicines_size;i++)
        delete medicines[i];
    medicines_size=0;
    isValid_ = false;
}


void ReminderB::set_time_id(const unsigned int time_id) {this->time_id = time_id;}
void ReminderB::set_time(DateTime  *time) {
    delete this->time;
    this->time = time;
    isValid_ = true;
}


String ReminderB::toString() const{
    String time_str = "";
    if (time->hour() <= 9)
        time_str += '0' + String(time->hour());
    else
        time_str += String(time->hour());
    time_str += ':';
    if (time->minute() <= 9)
        time_str += '0' + String(time->minute());
    else
        time_str += String(time->minute());

    return  "{"
                    R"("time_id" : )" + static_cast<String>(time_id) +
                    R"("time" : )" + time_str +
                "}";
}