#include "ReminderB.h"
#include <Medicine.h>



void ReminderB::add_medicine(Medicine *medicine) {
    auto temp_medicine= new Medicine*[this->medicines_size_+1] ;
    for(byte i=0; i<this->medicines_size_;i++) {
        temp_medicine[i] = this->medicines[i];
    }
    temp_medicine[this->medicines_size_]=medicine;
    delete []this->medicines;
    this->medicines = temp_medicine;
    temp_medicine = nullptr;
    this->medicines_size_++;
}

void ReminderB::remove_medicine(const byte index) {
    auto temp_medicine = new Medicine*[this->medicines_size_-1];
    byte temp_medicine_next = 0;
    for(byte i=0; i<this->medicines_size_;i++) {
        if(i==index)
            delete this->medicines[i];
        else
            temp_medicine[temp_medicine_next++] = this->medicines[i];
    }
    delete []this->medicines;
    this->medicines = temp_medicine;
    temp_medicine = nullptr;
    this->medicines_size_--;
}

bool ReminderB::check_for_alarm(const DateTime &curr_time) {
    if(isValid_)
        if(time_.hour()==curr_time.hour() && time_.minute()==curr_time.minute()) {
            isValid_=false;
            return true;
        }
    return false;
}


void ReminderB::clear() {
    time_id_ = 0;
    for(int i=0;i<medicines_size_;i++)
        delete medicines[i];
    medicines_size_=0;
    isValid_ = false;
}




// String ReminderB::toString() const{
//     String time_str = "";
//     if (time.hour() <= 9)
//         time_str += '0' + String(time.hour());
//     else
//         time_str += String(time.hour());
//     time_str += ':';
//     if (time.minute() <= 9)
//         time_str += '0' + String(time.minute());
//     else
//         time_str += String(time.minute());
//
//     return  "{"
//                     R"("time_id" : )" + static_cast<String>(time_id) +
//                     R"("time" : )" + time_str +
//                 "}";
// }