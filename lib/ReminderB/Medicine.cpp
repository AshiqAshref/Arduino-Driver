#include "Medicine.h"

String Medicine::toString() const {
    return "{"
                R"("box_no" : )" + box->toString() +
                R"("dosage" : )" + static_cast<String>(dosage) +
              "}";
}