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
        set_status(COMPLETED_REFRESH);
    }

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
#endif //COMMAND_GET_TIME_H
