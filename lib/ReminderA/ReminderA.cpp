#include <Box.h>
#include <ReminderA.h>

ReminderA::ReminderA(DateTime *time, const Box &box, const unsigned int id){
    this->id = id;
    this->box = box;
    this->time = time;
}

unsigned int ReminderA::get_id() const {return this->id;}
Box &ReminderA::get_box() {return this->box;}
DateTime *ReminderA::get_time() const {return this->time;}
boolean ReminderA::get_success() const {return this->success;}

void ReminderA::set_id(const unsigned int id) {this->id = id;}
void ReminderA::set_box(Box const &box) {this->box = box;}
void ReminderA::set_upc(DateTime *time) {this->time = time;}
void ReminderA::set_success(boolean const * success_p) {this->success = *success_p;}

String ReminderA::toString()const{
    String time_str="\"";
    if(time->hour()<=9)
        time_str+='0'+String(time->hour());
    else
        time_str+=String(time->hour());

    time_str+=':';
    if(time->minute()<=9)
        time_str+='0'+String(time->minute());
    else
        time_str+=String(time->minute());
    time_str+="\"";

    String a="{"
             R"( "id";)"     +static_cast<String>(id)+
             R"(,"date": ")"   +time_str+"\""+
             R"(,"success": )" +success +
        "}";
    return a;
}


