#ifndef REMINDERA_H
#define REMINDERA_H
#include <Arduino.h>
#include <RTClib.h>

class ReminderA{
private:
    DateTime upc=DateTime(0,0,0);;
    unsigned int box_no;
    unsigned int id;
    boolean success;
public:
    ReminderA(DateTime const * upc_p, unsigned int const  * box_no_p,
        unsigned int const * id_p, boolean const * success_p);
    ReminderA();
    ~ReminderA()=default;

    DateTime & get_upc();
    unsigned int & get_box_no();
    unsigned int & get_id();
    boolean & get_success();

    void set_upc(DateTime const * upc_p);
    void set_box_no(byte const * box_no_p);
    void set_id(unsigned int const * id_p);
    void set_success(boolean const * success_p);
    String toString()const;

};

#endif //REMINDERA_H
