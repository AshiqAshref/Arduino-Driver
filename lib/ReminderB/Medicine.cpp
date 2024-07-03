#include "Medicine.h"

Medicine::Medicine(const Box &box, const byte dosage) {
    this->box = box;
    this->dosage = dosage;
}

Box Medicine::get_box() const {return this->box;}
byte Medicine::get_dosage() const { return this->dosage;}
boolean Medicine::get_success() const {return this->success;}

void Medicine::set_box(const Box &box) {this->box = box;}
void Medicine::set_dosage(const byte dosage) {this->dosage = dosage;}
void Medicine::successfull() {this->success=true;}
void Medicine::unsuccessfull() {this->success=false;}

String Medicine::toString() const {
    return "{"
                R"("box_no" : )" + box.toString() +
                R"("dosage" : )" + static_cast<String>(dosage) +
              "}";
}