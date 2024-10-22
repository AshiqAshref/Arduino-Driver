#ifndef REMINDERB_H
#define REMINDERB_H
#include <Arduino.h>
#include <Medicine.h>
#include <RTClib.h>

class ReminderB{
    unsigned int time_id = 0;
    DateTime *time= new DateTime();
    byte medicines_size = 0;
    Medicine **medicines = new Medicine*[medicines_size];
    bool isValid_ = false;

public:
    explicit ReminderB(DateTime *time);
    ReminderB(DateTime *time, Medicine** medicines);
    ReminderB()=default;
    ~ReminderB()=default;

    byte get_medicine_size() const;
    void add_medicine(Medicine *medicine);
    void remove_medicine(byte index);

    bool check_for_alarm(const DateTime &curr_time);


    Medicine* get_medicine(byte index) const;
    void clear();

    unsigned int get_time_id() const;
    DateTime * get_time() const;

    void set_time(DateTime *time);
    void set_time_id(unsigned int time_id);
    String toString() const;
    bool isValid() const {return isValid_;}
    void setValidity(const bool isValid_) {this->isValid_ = isValid_;}
};

#endif //REMINDERB_H
