#include "ReminderBRunner.h"
#include <Command_reminderB_send_log.h>
#include <IO_PINS.h>
#include <Lcd_Menu.h>
#include <Mech_Arm.h>

extern Command_reminderB_send_log command_reminderB_send_log;
extern Mech_Arm mech_arm;
extern Box boxes[];

void ReminderBRunner::handleReminder() {
    if(active) {
        bool allSuccess=true;
        for(byte i=0;i<this->temp_med_size;i++) {
            if(boxes[this->meds_[i]->box_index_].box_status()!=BOX_STATUS_EMPTY){
                // if(boxes[this->meds_[i]->box_index_].isOpen()) {
                if(true){//TEST_ONLY
                    Serial.print(" BOX OPN: ");
                    Serial.println(boxes[this->meds_[i]->box_index_].box_no());
                    if(boxes[this->meds_[i]->box_index_].box_status()!=BOX_STATUS_COMPLETED) {
                        boxes[this->meds_[i]->box_index_].set_box_status(BOX_STATUS_COMPLETED);
                        box_complete_protocol(this->meds_[i]);
                    }
                }
                if(boxes[this->meds_[i]->box_index_].box_status()!=BOX_STATUS_COMPLETED){
                    allSuccess=false;
                }
            }
        }
        // allSuccess=true; //TEST_ONLY
        if(allSuccess) {
            // for(byte i=0;i<this->temp_med_size;i++) { //TEST_ONLY
                // boxes[this->meds_[i]->box_index_].set_box_status(BOX_STATUS_COMPLETED); //TEST_ONLY
            // }//TEST_ONLY
            Serial.println("ALL SUCESS");
            closing_protocol();
        }
    }
}


void ReminderBRunner::box_complete_protocol(TempMedicine *med){
    boxes[med->box_index_].reduce_amount(med->dosage_);
    med->set_time_completed();
    Lcd_Menu::lcdClear(0);
    const String value =
        "Box:"+
        static_cast<String>(boxes[med->box_index_].box_no())+
        " take:" + med->dosage_;
    Lcd_Menu::printCenter(value, 0);
}


// void ReminderBRunner::add_time_to_med(TempMedicine *med) {
//     const String time_str = AV_Functions::get_current_formated_Time();
//     for(int i=0;i<5;i++)
//         med->time_completed_[i] = time_str.charAt(i);
//     med->time_completed_[5]='\0';
// }


void ReminderBRunner::unlock_boxes() const {
    byte box_positions[temp_med_size] ;
    for(byte i=0;i<this->temp_med_size;i++) {
        if(boxes[this->meds_[i]->box_index_].box_status()!=BOX_STATUS_EMPTY)
            box_positions[i]= this->meds_[i]->box_index_;
    }
    mech_arm.unlockBox(box_positions, temp_med_size);
}


void ReminderBRunner::closing_protocol() {
    Serial.println("starting CLOSING PROTOCOL");
    set_active(false);
    for(int i=0;i<this->temp_med_size;i++) {
        if(boxes[this->meds_[i]->box_index_].box_status()!=BOX_STATUS_COMPLETED) {
            boxes[this->meds_[i]->box_index_].set_box_status(BOX_STATUS_MISSED);
            this->meds_[i]->clear_time_completed();
        }
    }
    const JsonDocument expired_reminder_json=  reminderB_to_json_custom();
    command_reminderB_send_log.send_request(expired_reminder_json);
    Serial.println("---------------REMINDER LOG--------------- ");
    serializeJson(expired_reminder_json,Serial);
    Serial.println();
    clear_temp_med();
    reminder_closed=true;
}

byte ReminderBRunner::getBoxIndex(const byte box_no) {
    for (int i = 0; i < 16; i++) {
        if(boxes[i].box_no()==box_no) return i;
    }
    return 0;
}



JsonDocument ReminderBRunner::reminderB_to_json_custom() const {
    JsonDocument doc;
    doc["t"] = this->time_string_;
    doc["rv"] = this->revision_no_;
    auto med_array = doc["m"].to<JsonArray>();
    if(this->temp_med_size>0) {
        for(int i=0;i<this->temp_med_size;i++) {
            JsonDocument med_doc;
            med_doc["b"]= boxes[this->meds_[i]->box_index_].box_no();
            med_doc["d"]= this->meds_[i]->dosage_;
            med_doc["n"]= boxes[this->meds_[i]->box_index_].medicine_name();
            med_doc["a"]= boxes[this->meds_[i]->box_index_].medicine_amount();
            med_doc["ct"]= this->meds_[i]->time_completed();
            // med_doc["s"]= (this->meds_[i]->success_?1:0);
            // ReSharper disable once CppExpressionWithoutSideEffects
            med_array.add(med_doc);
        }
    }else {
        JsonDocument empty; // ReSharper disable once CppExpressionWithoutSideEffects
        med_array.add(empty);
    }
    Serial.println("SENDING_LOG: ");
    serializeJson(doc, Serial);
    Serial.println();
    return doc;
}

