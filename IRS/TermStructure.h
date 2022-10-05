#pragma once
#include <iostream>
#include <vector>
#include "LinearInterpolation.h"

namespace IR {
class TermStructure {
  using Time = double;
  using ZeroRate = double;

public:
  enum class Frequency{
    Yearly,
    SemiAnnually,
    Quarterly
  };

  explicit TermStructure() = default;
  explicit TermStructure(const std::vector<Time>& ttms, const std::vector<ZeroRate>& zero_rates,
                const Frequency& freq = Frequency::Quarterly)
  {
    interpolater_ = std::make_shared<Interpolation::LinearInterpolation1D>(ttms, zero_rates);
    switch (freq) {
    case Frequency::Yearly: {
      step_ = 1;
    }
    break;
    case Frequency::SemiAnnually: {
      step_ = 0.5;
    }
    break;
    case Frequency::Quarterly: {
      step_ = 0.25;
    }
    break;
    default:
      throw std::invalid_argument("Does not recognize frequency other than yearly, semi-annually, and quarterly");
    }
    n_ = 1 / step_;
  };
  virtual ~TermStructure() = default;

  double GetPar(Time t);

  double GetZero(Time t) {
    return (*interpolater_)(t);
  }

  double GetForward(Time t);

  double GetDiscountFactor(Time t);

protected:
  std::shared_ptr<Interpolation::Interpolator1D> interpolater_;
  double step_;
  double n_;

protected:
  double computeParRate(Time t);
  double computeForwardRate(Time t);
};
}
