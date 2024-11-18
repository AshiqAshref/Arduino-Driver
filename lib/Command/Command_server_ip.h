#ifndef COMMAND_SERVER_IP_H
#define COMMAND_SERVER_IP_H
#define RES_GET true
#define RES_SET false

#include <Command.h>
#include <IPAddress.h>

class Command_server_ip final : public Command{
    bool command_type_ = RES_GET;
    IPAddress server_ip;
    bool (*response_handler_bool)(const IPAddress&, bool);
public:
    Command_server_ip(
            void(*send_request)(),
            bool (*response_handler_bool)(const IPAddress&, bool),
            bool(*request_handler)(),
            const unsigned long retry_interval_on_fail
        ):
        Command(
            SERVER_IP,
            send_request,
            []{return true;},
            request_handler,
            retry_interval_on_fail
        ),
        response_handler_bool(response_handler_bool)
    {}

    Command_enum command() override {return this->command_;}
    bool command_type() const {return this->command_type_;}
    void send_request(const IPAddress& server_ip, const bool command_type=RES_GET){
        this->command_type_=command_type;
        this->server_ip=server_ip;
        Command::send_request();
    }

    void response_handler() override {
        if(this->response_handler_bool(this->server_ip, this->command_type_)) {
            this->set_status(COMPLETED);
        }else
            this->set_status(FAILED);
    }
};



#endif //COMMAND_SERVER_IP_H
