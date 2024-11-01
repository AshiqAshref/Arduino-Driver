//
// Created by user on 01/11/2024.
//

#ifndef COMMAND_DAYLIGHT_SAV_H
#define COMMAND_DAYLIGHT_SAV_H
#include <Command.h>


class Command_daylight_sav final : public Command{
    bool daylight_sav =false;
    bool (*response_handler_bool)(bool);

public:
    Command_daylight_sav(void(*send_request)(),
        bool (*response_handler_bool)(bool),
        bool(*request_handler)(),
        const unsigned long retry_interval_on_fail
        ) : Command(
                DAYLIGHT_SAV,
                send_request,
                []{return true;},
                request_handler,
                retry_interval_on_fail),response_handler_bool(response_handler_bool){}

    Command_enum command() override {return this->command_;}

    void send_request(const bool daylight_saving){
        this->daylight_sav=daylight_saving;
        this->set_status(IN_PROGRESS);
        this->send_request_();
    }
    void response_handler() override {
        if(this->response_handler_bool(this->daylight_sav)) {
            this->set_status(COMPLETED);
        }else
            this->set_status(FAILED);
    }

};



#endif //COMMAND_DAYLIGHT_SAV_H
