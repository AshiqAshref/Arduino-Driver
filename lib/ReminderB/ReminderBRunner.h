#ifndef REMINDERBRUNNER_H
#define REMINDERBRUNNER_H
#include <ReminderB.h>
#include <sensor_unit.h>
#include <ArduinoJson.h>


class ReminderBRunner {
    ReminderB last_reminder_b_ = ReminderB();
    const uint32_t expire_duration = 600000;
    bool active=false;

public:
    ReminderBRunner()=default;
    ReminderB get_reminderb() {
        return this->last_reminder_b_;
    }

    void handleReminder();
    void closing_protocol() ;

    void set_current_reminder(const ReminderB &reminder_b) {
        closing_protocol();

        this->last_reminder_b_=reminder_b;
        for(int i=0;i<last_reminder_b_.medicine_size();i++) {
            last_reminder_b_.get_medicine(i)->box()->set_status(BOX_STATUS_CURRENT);
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

};



#endif //REMINDERBRUNNER_H
