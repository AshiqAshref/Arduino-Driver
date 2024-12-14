#ifndef MEDICINE_H
#define MEDICINE_H
#include <Arduino.h>
#include <ModeB/Box.h>

class Medicine {
    Box *box_;
    byte dosage_=0;
    bool success_=false;
public:
    Medicine(Box *box, const byte dosage)
        : box_(box),
          dosage_(dosage) {
    }
    ~Medicine()=default;

    Box *box() const {return this->box_;}
    byte dosage() const { return this->dosage_;}
    bool isSuccess() const {return this->success_;}

    // void set_box(Box box) {this->box_ = &box;}
    void set_dosage(const byte dosage) {this->dosage_ = dosage;}
    void set_success(const bool status) {
        this->box_->set_box_status(BOX_STATUS_DEFAULT);
        this->success_=status;
    }

    void toStringPrint() const{
        Serial.print("{");
        Serial.print(  R"("box" : )" );box_->toStringPrint();
        Serial.print(R"(", dosage" : )"); Serial.print(this->dosage_);
        Serial.print(R"(", succes" : )");Serial.print(isSuccess()?"true":"false");
        Serial.print("} ");
    }


    String toString() const {
        return "{"
                    R"("bno" : )" + box_->toString() +
                    R"(", d" : )" + static_cast<String>(dosage_) +
                    R"(", s" : )" + (isSuccess()?"t":"f") +
                  "} ";
    }
};
#endif //MEDICINE_H
