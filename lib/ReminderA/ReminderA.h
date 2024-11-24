#ifndef REMINDERA_H
#define REMINDERA_H
#include <Arduino.h>
#include <RTClib.h>
#include <ModeB/Box.h>


class ReminderA{
    unsigned int id=0;
    Box *box = nullptr;
    DateTime *time=new DateTime(2020,12,12);
    boolean success=false;
public:
    ReminderA(DateTime *time,  Box *box, unsigned int id);
    ReminderA()=default;
    ~ReminderA()=default;

    DateTime * get_time() const;

    Box *get_box() const;
    unsigned int get_id() const;
    boolean get_success() const;

    void set_id(unsigned int id);
    void set_box(Box *box);
    void set_upc(DateTime *time);
    void set_success(boolean const * success_p);
    String toString()const;

};

#endif //REMINDERA_H
