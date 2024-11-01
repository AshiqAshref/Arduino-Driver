//
// Created by user on 24/10/2024.
//

#ifndef COMMAND_ACTIVATE_AP_H
#define COMMAND_ACTIVATE_AP_H
#include <Command.h>



class Command_activate_AP final : public Command {
public:
    Command_activate_AP(void(*send_request)(), bool(*response_handler)(),
                        bool(*request_handler)(), const unsigned long retry_interval_on_fail)
        : Command(ACTIVATE_AP, send_request, response_handler, request_handler, retry_interval_on_fail) {
    }
    Command_enum command() override {return this->command_;}
};



#endif //COMMAND_ACTIVATE_AP_H
