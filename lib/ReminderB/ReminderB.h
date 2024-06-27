#ifndef REMINDERB_H
#define REMINDERB_H
#include <Arduino.h>
#include <RTClib.h>

class ReminderB{
private:
    DateTime upc;
    String boxes;
    boolean success;
public:
    ReminderB(DateTime const * upc_p, String const * boxes_p, boolean const * success_p);
    ReminderB();
    ~ReminderB()=default;

    DateTime & get_upc();
    String & get_boxes();
    boolean & get_success();

    void set_upc(DateTime const * upc_p);
    void set_boxes(String const * boxes_p);
    void set_success(boolean const * success_p);

    String printArray();
    String toString() const;

};

#endif //REMINDERB_H
