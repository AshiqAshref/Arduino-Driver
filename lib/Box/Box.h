#ifndef BOX_H
#define BOX_H
#include <Arduino.h>

class Box {
private:
    int box_no;
    String name;
    int no_of_pills;
public:
    Box(int const *box_no_p, String const *name_p, int const *no_of_pills_p);
    ~Box() = default;

    int & get_box_no();
    String & get_name();
    int & get_no_of_pills();

    void set_box_no(int const *box_no_p);
    void set_name(String const *name_p);
    void set_no_of_pills(int const *no_of_pills_p);

    void toString() const;
};
#endif //BOX_H
