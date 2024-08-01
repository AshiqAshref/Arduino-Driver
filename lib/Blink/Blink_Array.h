#ifndef BLINK_ARRAY_H
#define BLINK_ARRAY_H
#include <Blink.h>

class Blink_Array {
    byte blink_size_ = 0;
    Blink **blinks = new Blink*[blink_size_];
public:
    Blink_Array() = default;
    ~Blink_Array() = default;

    void add(byte box_no, char color);
    void add(Blink *blink);
    void remove_by_index(byte index);
    void remove_by_boxNo(byte box_no);

    void blinkAll() const;
    void clearAll();
    int find_index_by_box_no(byte box_no) const;

    Blink *get_blink_by_index(byte index) const;
    Blink *get_blink_by_box(byte box_no) const;
    byte blink_size() const {return blink_size_;}
};
#endif //BLINK_ARRAY_H
