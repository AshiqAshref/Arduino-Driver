#include "ReminderBRunner.h"
#include <Command_reminderB_send_log.h>
#include <Mech_Arm.h>

extern Command_reminderB_send_log command_reminderB_send_log;
extern Mech_Arm mech_arm;

void ReminderBRunner::handleReminder() {
    if(active) {
        bool allSuccess=true;
        for(int i=0;i<last_reminder_b_.medicine_size();i++) {
            if(last_reminder_b_.get_medicine(i)->box()->isOpen()) {
                last_reminder_b_.get_medicine(i)->set_success(true);
            }
            if(!last_reminder_b_.get_medicine(i)->isSuccess()){
                allSuccess=false;
            }
        }
        if(allSuccess) {
            closing_protocol();
        }
    }
}


void ReminderBRunner::unlock_boxes() const {
    mech_arm.bringEmHome();
    for(int i=0;i<last_reminder_b_.medicine_size();i++) {
        mech_arm.unlockBox(last_reminder_b_.get_medicine(i)->box());
    }
}


void ReminderBRunner::closing_protocol() {
    set_active(false);
    for(int i=0;i<last_reminder_b_.medicine_size();i++) {
        if(!last_reminder_b_.get_medicine(i)->isSuccess()) {
            last_reminder_b_.get_medicine(i)->box()->set_status(BOX_STATUS_MISSED);
        }
    }
    const JsonDocument expired_reminder_json=  reminderB_to_json(last_reminder_b_);
    command_reminderB_send_log.send_request(expired_reminder_json);
}

