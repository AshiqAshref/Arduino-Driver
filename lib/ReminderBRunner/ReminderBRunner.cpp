#include "ReminderBRunner.h"
#include <Command_reminderB_send_log.h>
#include <Mech_Arm.h>

extern Command_reminderB_send_log command_reminderB_send_log;
extern Mech_Arm mech_arm;
extern Box boxes[];

void ReminderBRunner::handleReminder() {
    if(active) {
        bool allSuccess=true;
        for(byte i=0;i<this->temp_med_size;i++) {
            if(boxes[this->meds_[i]->box_array_position_].isOpen()) {
                this->meds_[i]->success_ = true;
                Serial.print(" BOX OPN: ");
                Serial.println(boxes[this->meds_[i]->box_array_position_].box_no());
                boxes[this->meds_[i]->box_array_position_].set_status(BOX_STATUS_UPCOMMING);
            }
            if(!this->meds_[i]->success_){
                allSuccess=false;
            }
        }
        allSuccess=true; //TEST_ONLY
        if(allSuccess) {
            for(byte i=0;i<this->temp_med_size;i++) { //TEST_ONLY
                this->meds_[i]->success_ = true;//TEST_ONLY
            }//TEST_ONLY
            Serial.println("ALL SUCESS");
            closing_protocol();
        }
    }
}


void ReminderBRunner::unlock_boxes() const {
    mech_arm.bringEmHome();
    for(byte i=0;i<this->temp_med_size;i++) {
        mech_arm.unlockBox(&boxes[this->meds_[i]->box_array_position_]);
    }
}


void ReminderBRunner::closing_protocol() {
    Serial.println("starting CLOSING PROTOCOL");
    set_active(false);
    for(int i=0;i<this->temp_med_size;i++) {
        if(!this->meds_[i]->success_) {
            boxes[this->meds_[i]->box_array_position_].set_status(BOX_STATUS_MISSED);
        }
    }
    const JsonDocument expired_reminder_json=  reminderB_to_json_custom();
    command_reminderB_send_log.send_request(expired_reminder_json);
    serializeJson(expired_reminder_json,Serial);
    clear_temp_med();
    reminder_closed=true;
}

