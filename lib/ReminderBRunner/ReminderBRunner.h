#ifndef REMINDERBRUNNER_H
#define REMINDERBRUNNER_H
#include <ModeB/ReminderB.h>
#include <sensor_unit.h>
#include <ArduinoJson.h>

struct TempMedicine {
    byte box_index_;
    byte dosage_;
private:
    // char time_completed_[6]{};
    uint32_t time_completed_=0;
public:
    uint32_t time_completed(){
        return this->time_completed_;
    }
    void set_time_completed() {
        this->time_completed_=AV_Functions::get_current_unix_time();
    }
    void clear_time_completed() {
        this->time_completed_=0;
    }

};

class ReminderBRunner {
    const uint32_t expire_duration = 600000;
    bool active=false;
    bool reminder_closed=true;

    byte temp_med_size=0;
    TempMedicine **meds_ =new TempMedicine*[temp_med_size];
    String time_string_ = "";
    uint32_t revision_no_ = 0;


    void add_temp_med_to_array(TempMedicine *med) {
        auto temp_med_local= new TempMedicine*[this->temp_med_size+1] ;
        for(byte i=0; i<this->temp_med_size;i++) {
            temp_med_local[i] = this->meds_[i];
        }
        temp_med_local[this->temp_med_size]=med;
        delete []this->meds_;
        this->meds_ = temp_med_local;
        temp_med_local = nullptr;
        this->temp_med_size++;
    }

    void remove_med(const byte index) {
        auto temp_med_local = new TempMedicine*[this->temp_med_size-1];
        byte temp_med_local_next = 0;
        for(byte i=0; i<this->temp_med_size;i++) {
            if(i==index)
                delete this->meds_[i];
            else
                temp_med_local[temp_med_local_next++] = this->meds_[i];
        }
        delete []this->meds_;
        this->meds_ = temp_med_local;
        temp_med_local = nullptr;
        this->temp_med_size--;
    }

    void clear_temp_med() {
        for(int i=0;i<temp_med_size;i++)
            delete meds_[i];
        temp_med_size=0;
    }

    byte getBoxIndex(byte box_no) ;

    void copy_reminder_to_current(const ReminderB &reminderb) {
        const DateTime t = reminderb.time();
        const String time_str = AV_Functions::beautifyTime(t.hour())+':'+AV_Functions::beautifyTime(t.minute());
        this->time_string_ = time_str;
        this->revision_no_ = reminderb.revision_no();
        for(int i=0;i<reminderb.medicine_size();i++) {
            const auto temp_medicine = new TempMedicine;
            temp_medicine->box_index_ = getBoxIndex(reminderb.get_medicine(i)->box()->box_no());
            temp_medicine->dosage_= reminderb.get_medicine(i)->dosage();
            add_temp_med_to_array(temp_medicine);
        }
    }
    void closing_protocol() ;
    // void add_time_to_med(TempMedicine *med) ;


public:
    ReminderBRunner()=default;
    void handleReminder();
    void box_complete_protocol(TempMedicine *med);

    void set_current_reminder(const ReminderB &reminder_b) {
        if(!reminder_closed){closing_protocol();}
        copy_reminder_to_current(reminder_b);
        reminder_closed=false;

        Serial.println(F("STARTING REMINDER"));
        reminder_b.toStringPrint();

        for(byte i=0;i<reminder_b.medicine_size();i++) {
            reminder_b.get_medicine(i)->box()->set_box_status(BOX_STATUS_CURRENT);
        }

        set_active(true);
        //unlock_boxes();//TEST ONLY
    }

    void unlock_boxes() const;
    void set_active(const bool status) {this->active=status;}
    bool is_active() const {return active;}
    JsonDocument reminderB_to_json_custom() const;
};

#endif //REMINDERBRUNNER_H
