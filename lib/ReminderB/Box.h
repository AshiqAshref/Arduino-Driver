#ifndef BOX_H
#define BOX_H
#include <Arduino.h>

class Box {
    // int box_no=0;
    // String name{};
    // int no_of_pills=0;
    // byte dosage=0;
public:
    int box_no=0;
    String name{};
    int no_of_pills=0;
    byte dosage=0;
    Box(int const *box_no_p, String const *name_p, int const *no_of_pills_p);
	Box(const String& name, byte box_no);
	Box()=default;
    Box(Box& t);
    ~Box();


    int get_box_no() const;
    String get_name();
    int get_no_of_pills() const;
    byte get_dosage() const;

    void set_box_no(int const *box_no_p);
    void set_name(String const *name_p);
    void set_no_of_pills(int const *no_of_pills_p);

    void deleteThis() const;

    void toString() const;
};
#endif //BOX_H
