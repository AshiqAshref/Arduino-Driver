#ifndef REMINDERB_H
#define REMINDERB_H


#include <Arduino.h>
#include <AV_Functions.h>
#include <ModeB/Medicine.h>
#include <RTClib.h>

enum TimeMode:bool{
    TIME_MODE_12 = true,
    TIME_MODE_24 = false,
};

class ReminderB{
    unsigned int time_id_ = 0;
    DateTime time_= DateTime();
    String display_time_= "NA";
    TimeMode display_time_format=TIME_MODE_12;
    byte medicines_size_ = 0;
    Medicine **medicines = new Medicine*[medicines_size_];
    unsigned long revision_no_ =0;
    bool isValid_ = false;

public:
    explicit ReminderB(const DateTime &time) {
        set_time(time);
        setValidity(true);
    }
    ReminderB(const DateTime &time, Medicine** medicines){
        set_time(time);
        delete []this->medicines;
        this->medicines = medicines;
        setValidity(true);
    }
    ReminderB()=default;
    ~ReminderB()=default;

    byte medicine_size() const {return this->medicines_size_;}
    void add_medicine(Medicine *medicine);
    void remove_medicine(byte index);

    bool check_for_alarm(const DateTime &curr_time);

    Medicine* get_medicine(const byte index) const {return this->medicines[index];}
    void clear_reminder();


    unsigned int time_id() const {return time_id_;}
    void set_time_id(const unsigned int time_id){
        this->time_id_ = time_id;
    }

    DateTime time() const{return this->time_;};
    void set_time(const DateTime &time){
        this->time_ = time;
        setValidity(true);
        generate_display_time();
    }

    String display_time() const{ return this->display_time_;}
    void set_display_time_format(const TimeMode format){this->display_time_format=format;}

    void set_revision_no(const unsigned long revision_no){this->revision_no_= revision_no;}
    unsigned long revision_no() const{return this->revision_no_;}



    bool isValid() const {return isValid_;}
    void setValidity(const bool isValid_) {this->isValid_ = isValid_;}


    void generate_display_time() {
        if(this->display_time_format==TIME_MODE_24)
            this->display_time_=AV_Functions::beautifyTime(this->time_.hour())+':'
                +AV_Functions::beautifyTime(this->time_.minute());
        else
            this->display_time_=AV_Functions::beautifyTime(this->time_.twelveHour())+':'
                +AV_Functions::beautifyTime(this->time_.minute())
                    + (this->time_.isPM()? " P": " A");
    }



    String get_medicines_as_String() const {
        String result = "[";
        for(int i=0;i<medicine_size();i++) {
            result+=get_medicine(i)->toString();
            if(i+1!=medicine_size())
                result+=", ";
        }
        result+="] ";
        return result;
    }
    void get_medicines_as_String_print() const {
        Serial.print("[");
        for(int i=0;i<medicine_size();i++) {
            get_medicine(i)->toStringPrint();
            if(i+1!=medicine_size())
                Serial.print(", ");
        }
        Serial.print("] ");

    }



    void toStringPrint() const {
        Serial.print(" {");
        Serial.print(R"("time_id" : )" ); Serial.print(this->time_id_);
        Serial.print(R"(", time" : )" + display_time());
        Serial.print(R"(", medicines" : )");
        get_medicines_as_String_print();
        Serial.print("} ");
    }

    String toString() const{
        return  " {"
                        R"("tid" : )" + static_cast<String>(time_id()) +
                        R"(", t" : )" + display_time() +
                        R"(", m" : )" + get_medicines_as_String() +
                    "} ";
    }


};

#endif //REMINDERB_H
