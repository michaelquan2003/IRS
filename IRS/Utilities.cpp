#include "Utilities.h"

namespace Tools {
double GetStepPerYear(const IR::Frequency& freq) {
  switch (freq) {
    case IR::Frequency::Yearly: {
      return 1;
    } break;
    case IR::Frequency::SemiAnnually: {
      return 0.5;
    } break;
    case IR::Frequency::Quarterly: {
      return 0.25;
    } break;
    case IR::Frequency::Monthly: {
      return 1. / 12;
    } break;
    default:
      throw std::invalid_argument(
          "Does not recognize frequency other than yearly, semi-annually, and "
          "quarterly");
  }
  return 0;
}

double GetNumPerYear(const IR::Frequency& freq) {
  auto step = GetStepPerYear(freq);
  return 1 / step;
}

}  // namespace Tools