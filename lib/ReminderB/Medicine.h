#ifndef MEDICINE_H
#define MEDICINE_H
#include <Arduino.h>
#include <Box.h>

class Medicine {
    Box box=Box();
    byte dosage=0;
    boolean success=false;
public:
    Medicine(const Box &box, byte dosage);
	Medicine()=default;
    ~Medicine()=default;

    Box get_box() const;
    byte get_dosage() const;
    boolean  get_success() const;

    void set_box(const Box &box);
    void set_dosage(byte dosage);
    void successfull();
    void unsuccessfull();

    String toString() const;
};
#endif //MEDICINE_H
