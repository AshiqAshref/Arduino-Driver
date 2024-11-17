//
// Created by user on 24/10/2024.
//

#ifndef COMMAND_GET_REMINDERB_H
#define COMMAND_GET_REMINDERB_H
#include <Command.h>

class Command_get_reminderB final: public Command {
    unsigned long next_reminder_time_key_=0;
    bool (*response_handler_long_)(unsigned long);
public:
    Command_get_reminderB(void(*send_request)(), bool(*response_handler_long)(unsigned long),
                              bool(*request_handler)(), const unsigned long retry_interval_on_fail)
            : Command(GET_REMINDER_B, send_request, []{return true;}, request_handler, retry_interval_on_fail),response_handler_long_(response_handler_long) {
    }

    unsigned long next_reminder_time_key() const {return this->next_reminder_time_key_;}
    void set_next_reminder_time_key(const unsigned long next_reminder_time_key_) {this->next_reminder_time_key_=next_reminder_time_key_;}
    void send_request(const unsigned long next_reminder_time_key){
        this->next_reminder_time_key_ = next_reminder_time_key;
        Command::send_request();
    }
    void response_handler() override {
        if(this->response_handler_long_(this->next_reminder_time_key_)) {
            this->set_status(COMPLETED);
        }else
            this->set_status(FAILED);
    }

    Command_enum command() override {return this->command_;}


};

#endif //COMMAND_GET_REMINDERB_H
