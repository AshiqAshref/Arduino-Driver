#ifndef REMINDERB_H
#define REMINDERB_H
#include <Arduino.h>
#include <Box.h>
#include <RTClib.h>


class ReminderB{
    DateTime upc{};
    size_t boxes_size = 0;
    Box * boxes_;
    Box **boxes_test = new Box*[];
    size_t boxes_size_test = 0;

    boolean success=false;
public:
    template<size_t size>
    ReminderB(DateTime const * upc_p, Box (&boxes)[size], boolean const * success_p);
    ReminderB(DateTime const * upc_p, boolean const * success_p);
    ReminderB();
    ~ReminderB()=default;

    size_t get_boxes_size_test();
    void add_to_boxes_test(Box *box);
    void remove_a_box_test(size_t index);
    Box* get_a_box_test(size_t index);

    DateTime & get_upc();
    // String & get_boxes();
    boolean & get_success();
    Box &get_a_box(size_t index) const;

    size_t get_boxes_size() const;
    Box * get_boxes() const;

    void set_upc(DateTime const * upc_p);
    void set_boxes(String const * boxes_p);
    void set_success(boolean const * success_p);
    void add_to_boxes(const Box *box);
    void remove_a_box(size_t index);

    String printArray();
    String toString() const;

};

#endif //REMINDERB_H
