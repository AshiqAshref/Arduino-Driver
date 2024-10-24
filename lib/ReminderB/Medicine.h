#ifndef MEDICINE_H
#define MEDICINE_H
#include <Arduino.h>
#include <Box.h>

class Medicine {
    Box *box;
    byte dosage=0;
    bool success=false;
public:
    Medicine(Box *box, const byte dosage)
        : box(box),
          dosage(dosage) {
    }
    Medicine(Box *box, const byte dosage, const bool success)
        : box(box),
          dosage(dosage),
          success(success){
    }
    ~Medicine()=default;

    Box *get_box() const {return this->box;}
    byte get_dosage() const { return this->dosage;}
    bool get_success() const {return this->success;}

    void set_box(Box *box) {this->box = box;}
    void set_dosage(const byte dosage) {this->dosage = dosage;}
    void successfull() {this->success=true;}
    void unsuccessfull() {this->success=false;}

    String toString() const;
};
#endif //MEDICINE_H
