#ifndef REMINDERBRUNNER_H
#define REMINDERBRUNNER_H
#include <ModeB/ReminderB.h>
#include <sensor_unit.h>
#include <ArduinoJson.h>

struct TempMedicine {
    byte box_array_position_;
    byte dosage_;
    bool success_;
};

class ReminderBRunner {
    const uint32_t expire_duration = 600000;
    bool active=false;
    bool reminder_closed=true;

    byte temp_med_size=0;
    TempMedicine **meds_ =new TempMedicine*[temp_med_size];
    uint16_t time_id_=0;
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

    void copy_reminder_to_current(const ReminderB &reminderb) {
        const DateTime t = reminderb.time();
        const String time_str = AV_Functions::beautifyTime(t.hour())+':'+AV_Functions::beautifyTime(t.minute());
        this->time_id_ = reminderb.time_id();
        this->time_string_ = time_str;
        this->revision_no_ = reminderb.revision_no();
        for(int i=0;i<reminderb.medicine_size();i++) {
            const auto temp_medicine = new TempMedicine;
            temp_medicine->box_array_position_ = reminderb.get_medicine(i)->box()->box_no()-1;
            temp_medicine->dosage_= reminderb.get_medicine(i)->dosage();
            temp_medicine->success_= (reminderb.get_medicine(i)->isSuccess()?1:0);
            add_temp_med_to_array(temp_medicine);
        }
    }

    void closing_protocol() ;

public:
    ReminderBRunner()=default;
    void handleReminder();
    void set_current_reminder(const ReminderB &reminder_b) {
        if(!reminder_closed){closing_protocol();}
        copy_reminder_to_current(reminder_b);
        reminder_closed=false;

        Serial.println(F("STARTING REMINDER"));
        reminder_b.toStringPrint();

        for(byte i=0;i<reminder_b.medicine_size();i++) {
            reminder_b.get_medicine(i)->box()->set_status(BOX_STATUS_CURRENT);
        }
        set_active(true);
        unlock_boxes();
    }


    void unlock_boxes() const;
    void set_active(const bool status) {this->active=status;}
    bool is_active() const {return active;}

    static JsonDocument reminderB_to_json( ReminderB &reminderb) {
        JsonDocument doc;
        const DateTime t = reminderb.time();
        const String time_str = AV_Functions::beautifyTime(t.hour())+':'+AV_Functions::beautifyTime(t.minute());
        doc["ti"] = reminderb.time_id();
        doc["t"] = time_str;
        doc["rv"] = reminderb.revision_no();
        auto med_array = doc["m"].to<JsonArray>();
        if(reminderb.medicine_size()>0) {
            for(int i=0;i<reminderb.medicine_size();i++) {
                JsonDocument med_doc;
                med_doc["b"]= reminderb.get_medicine(i)->box()->box_no();
                med_doc["d"]= reminderb.get_medicine(i)->dosage();
                med_doc["s"]= (reminderb.get_medicine(i)->isSuccess()?1:0);
                // ReSharper disable once CppExpressionWithoutSideEffects
                med_array.add(med_doc);
            }
        }else {
            JsonDocument empty; // ReSharper disable once CppExpressionWithoutSideEffects
            med_array.add(empty);
        }
        return doc;
    }




    JsonDocument reminderB_to_json_custom() const {
        JsonDocument doc;
        doc["ti"] = this->time_id_;
        doc["t"] = this->time_string_;
        doc["rv"] = this->revision_no_;
        auto med_array = doc["m"].to<JsonArray>();
        if(this->temp_med_size>0) {
            for(int i=0;i<this->temp_med_size;i++) {
                JsonDocument med_doc;
                med_doc["b"]= this->meds_[i]->box_array_position_+1;
                med_doc["d"]= this->meds_[i]->dosage_;
                med_doc["s"]= (this->meds_[i]->success_?1:0);
                // ReSharper disable once CppExpressionWithoutSideEffects
                med_array.add(med_doc);
            }
        }else {
            JsonDocument empty; // ReSharper disable once CppExpressionWithoutSideEffects
            med_array.add(empty);
        }
        return doc;
    }

};



#endif //REMINDERBRUNNER_H
