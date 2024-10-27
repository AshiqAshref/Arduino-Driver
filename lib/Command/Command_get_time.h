//
// Created by user on 24/10/2024.
//

#ifndef COMMAND_GET_TIME_H
#define COMMAND_GET_TIME_H
#include <Command.h>


class Command_get_time final: public Command {
    const unsigned long retry_interval_onSucc_;
    const unsigned long retry_interval_onFail_;

public:
    Command_get_time(void(*send_request)(), bool(*response_handler)(), bool(*request_handler)(),
                     const unsigned long retry_interval, const unsigned long retry_interval_onSuccess)
        : Command(GET_TIME, send_request, response_handler, request_handler, retry_interval),
    retry_interval_onSucc_(retry_interval_onSuccess),
    retry_interval_onFail_(retry_interval) {
    }

    void set_status(const CommandStatus status) override {
        this->status_=status;
        last_millis=millis();
        if(status_==COMPLETED) {
            retry_interval = retry_interval_onSucc_;
        }else
            retry_interval=retry_interval_onFail_;
    }

    Command_enum command() override {return this->command_;}


};



#endif //COMMAND_GET_TIME_H
