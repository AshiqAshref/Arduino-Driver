#include "Medicine.h"

String Medicine::toString() const {
    return "{"
                R"("box_no" : )" + box_->toString() +
                R"("dosage" : )" + static_cast<String>(dosage_) +
              "}";
}