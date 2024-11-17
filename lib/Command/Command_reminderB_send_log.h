#ifndef COMMAND_REMINDERB_SEND_LOG_H
#define COMMAND_REMINDERB_SEND_LOG_H
#include <Command.h>

class Command_reminderB_send_log final : public Command{
public:
Command_reminderB_send_log(void(*send_request)(),
    bool (*response_handler_)(),
    bool(*request_handler)(),
    const unsigned long retry_interval_on_fail
    ) : Command(
            REMINDERB_SND_LOG,
            send_request,
            response_handler_,
            request_handler,
            retry_interval_on_fail
            )
        {}
    Command_enum command() override {return this->command_;}
};

#endif //COMMAND_REMINDERB_SEND_LOG_H
