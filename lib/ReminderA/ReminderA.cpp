#include <ReminderA.h>

ReminderA::ReminderA(DateTime const * upc_p, unsigned int const * box_no_p,
    unsigned int const * id_p, boolean const * success_p)
{
    upc=*upc_p;
    box_no = *box_no_p;
    id = *id_p;
    success = success_p;
}

ReminderA::ReminderA() {
    upc = DateTime() ;
    box_no = 0;
    id = 0;
    success = false;
}


DateTime & ReminderA::get_upc() {return upc;}
unsigned int & ReminderA::get_box_no() {return box_no;}
unsigned int & ReminderA::get_id() {return id;}
boolean & ReminderA::get_success() {return success;}

void ReminderA::set_upc(DateTime const * upc_p) {upc = *upc_p;}
void ReminderA::set_box_no(byte const * box_no_p) {box_no = *box_no_p;}
void ReminderA::set_id(unsigned int const * id_p) {this->id = *id_p;}
void ReminderA::set_success(boolean const * success_p) {this->success = *success_p;}

String ReminderA::toString()const{
    String time="\"";
    String BoxNo="\"";
    if(upc.hour()<=9){
        time+='0'+String(upc.hour());
    }else{
        time+=String(upc.hour());
    }
    time+=':';
    if(upc.minute()<=9){
        time+='0'+String(upc.minute());
    }else{
        time+=String(upc.minute());
    }
    time+="\"";

    if(box_no<=9){
        BoxNo+='0'+String(box_no);
    }else{
        BoxNo+=String(box_no);
    }
    BoxNo+="\"";

    String a="{"
             R"('id:')"     +String(id)+
             R"(,'date': ")"  +time+"\""+
             R"(,'boxNo':)"   +BoxNo +
             R"(,'success':")"+success+"\""
        "}";
    return a;
}


