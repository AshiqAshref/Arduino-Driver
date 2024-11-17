#ifndef MEDICINE_H
#define MEDICINE_H
#include <Arduino.h>
#include <Box.h>

class Medicine {
    Box *box_;
    byte dosage_=0;
    bool success_=false;
public:
    Medicine(Box *box, const byte dosage)
        : box_(box),
          dosage_(dosage) {
    }
    Medicine(Box *box, const byte dosage, const bool success)
        : box_(box),
          dosage_(dosage),
          success_(success){
    }
    ~Medicine()=default;

    Box *box() const {return this->box_;}
    byte dosage() const { return this->dosage_;}
    bool isSuccess() const {return this->success_;}

    void set_box(Box *box) {this->box_ = box;}
    void set_dosage(const byte dosage) {this->dosage_ = dosage;}
    void set_success(const bool status) {
        this->box_->set_status(BOX_STATUS_DEFAULT);
        this->success_=status;
    }

    String toString() const;
};
#endif //MEDICINE_H
