#pragma once
#include <iostream>
#include <vector>
#include "LinearInterpolation.h"
#include "Defines.h"

namespace IR {
class TermStructure {
public:
  explicit TermStructure(
    const std::vector<Time>& ttms,
    const std::vector<Rate>& rates,
    const Frequency& freq = Frequency::Quarterly,
    const Interpolation::InterpolationMethod& method = Interpolation::InterpolationMethod::Linear
  ) {
    interpolater_ = Interpolation::Interpolator1D::MakeInterpolator(ttms, rates, method);

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
  }

  virtual ~TermStructure() = default;

  virtual ZeroRate GetZero(Time t) = 0;

  virtual ForwardRate GetForward(Time t) = 0;

  virtual DiscountFactor GetDiscountFactor(Time t) = 0;

  virtual ParRate GetPar(Time t);

protected:
  std::shared_ptr<Interpolation::Interpolator1D> interpolater_;
  double step_;
  double n_;

protected:
  ParRate computeParRate(SizeT num_step);
};

class TermStructureZero : public TermStructure {
public:
  explicit TermStructureZero() = default;
  explicit TermStructureZero(
    const std::vector<Time>& ttms,
    const std::vector<ZeroRate>& zero_rates,
    const Frequency& freq = Frequency::Quarterly,
    const Interpolation::InterpolationMethod& method = Interpolation::InterpolationMethod::Linear
  ) : TermStructure(ttms, zero_rates, freq, method) {};
  virtual ~TermStructureZero() = default;

  virtual ZeroRate GetZero(Time t) override {
    return (*interpolater_)(t);
  }

  virtual ForwardRate GetForward(Time t) override;

  virtual DiscountFactor GetDiscountFactor(Time t) override;

private:
  ForwardRate computeForwardRate(SizeT num_step);
};

class TermStructureZeroSimple final : public TermStructureZero {
public:
  explicit TermStructureZeroSimple() = default;
  explicit TermStructureZeroSimple(
    const std::vector<Time>& ttms,
    const std::vector<ZeroRate>& zero_rates,
    const Frequency& freq = Frequency::Quarterly,
    const Interpolation::InterpolationMethod& method = Interpolation::InterpolationMethod::Linear
  ) : TermStructureZero(ttms, zero_rates, freq, method) {};

  virtual ~TermStructureZeroSimple() = default;

  virtual ForwardRate GetForward(Time t) override;

  virtual DiscountFactor GetDiscountFactor(Time t) override;
};

class TermStructureForward : public TermStructure {
public:
  explicit TermStructureForward(
    const std::vector<Time>& ttms,
    const std::vector<ForwardRate>& fwd_rates,
    const Frequency & freq = Frequency::Quarterly,
    const Interpolation::InterpolationMethod & method = Interpolation::InterpolationMethod::Linear
  ) : TermStructure(ttms, fwd_rates, freq, method) {};

  virtual ~TermStructureForward() = default;
};

class TermStructureForwardSimple final : public TermStructureForward {
public:
  explicit TermStructureForwardSimple() = default;
  explicit TermStructureForwardSimple(
    const std::vector<Time>& ttms,
    const std::vector<ForwardRate>& fwd_rates,
    const Frequency& freq = Frequency::Quarterly,
    const Interpolation::InterpolationMethod& method = Interpolation::InterpolationMethod::Linear
  ) : TermStructureForward(ttms, fwd_rates, freq, method) {};

  virtual ~TermStructureForwardSimple() = default;

  virtual ZeroRate GetZero(Time t) override;

  virtual ForwardRate GetForward(Time t) override {
    return (*interpolater_)(t);
  }

  virtual DiscountFactor GetDiscountFactor(Time t) override;

private:
  DiscountFactor computeDiscountFactor(SizeT num_step);
};
}
