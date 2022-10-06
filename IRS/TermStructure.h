#pragma once
#include <iostream>
#include <vector>
#include "LinearInterpolation.h"

namespace IR {
using Time = double;
using ZeroRate = double;
using ForwardRate = double;
using DiscountFactor = double;
using ParRate = double;

enum class Frequency {
    Yearly,
    SemiAnnually,
    Quarterly
};

class Termstructure {
public:
    virtual ~Termstructure() = default;

    virtual ParRate GetPar(Time t) = 0;

    virtual ZeroRate GetZero(Time t) = 0;

    virtual ForwardRate GetForward(Time t) = 0;

    virtual DiscountFactor GetDiscountFactor(Time t) = 0;
};

class TermStructure_Zero : public Termstructure{
public:
  explicit TermStructure_Zero() = default;
  explicit TermStructure_Zero(const std::vector<Time>& ttms, const std::vector<ZeroRate>& zero_rates,
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
  virtual ~TermStructure_Zero() = default;

  virtual ParRate GetPar(Time t) override;

  virtual ZeroRate GetZero(Time t) override{
    return (*interpolater_)(t);
  }

  virtual ForwardRate GetForward(Time t) override;

  virtual DiscountFactor GetDiscountFactor(Time t) override;

protected:
  std::shared_ptr<Interpolation::Interpolator1D> interpolater_;
  double step_;
  double n_;

protected:
  ParRate computeParRate(Time t);
  ForwardRate computeForwardRate(Time t);
};
}
