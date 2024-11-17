#ifndef COMMAND_REMINDERB_CHANGE_H
#define COMMAND_REMINDERB_CHANGE_H
#include <Command.h>


class Command_reminderB_change final: public Command {
    unsigned long current_revision_no_=0;
    const unsigned long retry_interval_onSucc_;
    const unsigned long retry_interval_onFail_;
public:
    Command_reminderB_change(void(*send_request)(), bool(*response_handler)(),
                              bool(*request_handler)(),
                              const unsigned long retry_interval_on_fail, const unsigned long retry_interval_on_succ)
            : Command(REMINDERB_CH, send_request, response_handler, request_handler, retry_interval_on_fail),
    retry_interval_onSucc_(retry_interval_on_succ), retry_interval_onFail_(retry_interval_on_fail) {
        set_status(COMPLETED_REFRESH);

    }

    unsigned long current_revision_no() const {return this->current_revision_no_;}
    void set_next_reminder_time_key(const unsigned long current_revision_no_) {this->current_revision_no_=current_revision_no_;}



    void set_status(const CommandStatus status) override {
        if(status==COMPLETED) {
            this->status_ = COMPLETED_REFRESH;
            retry_interval = retry_interval_onSucc_;
        }else {
            this->status_=status;
            retry_interval=retry_interval_onFail_;
        }
        last_millis=millis();
    }

    Command_enum command() override {return this->command_;}

};


#endif //COMMAND_REMINDERB_CHANGE_H
