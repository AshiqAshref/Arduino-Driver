#ifndef COMMAND_REMINDERB_SEND_LOG_H
#define COMMAND_REMINDERB_SEND_LOG_H
#include <Command.h>

class Command_reminderB_send_log final : public Command{
    bool(*response_handler_)(const JsonDocument&);
    JsonDocument remb_log_{};
public:
    Command_reminderB_send_log(
            void(*send_request)(),
            bool(*response_handler_)(const JsonDocument&),
            bool(*request_handler)(),
            const uint32_t retry_interval_on_fail
        ):
        Command(
            REMINDERB_SND_LOG,
            send_request,
            []{return true;},
            request_handler,
            retry_interval_on_fail
        ),
        response_handler_(response_handler_)
    {}

    void send_request(const JsonDocument &remb_log) {
        this->remb_log_=remb_log;
        Command::send_request();
    }

    void response_handler() override {
        this->response_handler_(remb_log_)?
            this->set_status(COMPLETED):
            this->set_status(FAILED);
    }

    Command_enum command() override {return this->command_;}
};

#endif //COMMAND_REMINDERB_SEND_LOG_H
