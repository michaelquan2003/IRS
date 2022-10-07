#pragma once
#include <iostream>
#include <vector>

#include "Defines.h"
#include "LinearInterpolation.h"

namespace IR {
class TermStructure {
 public:
  explicit TermStructure(const std::vector<Time>& ttms,
                         const std::vector<Rate>& rates,
                         const Frequency& freq = Frequency::Quarterly,
                         const Interpolation::InterpolationMethod& method =
                             Interpolation::InterpolationMethod::Linear) : method_(method) {
    interpolater_ =
        Interpolation::Interpolator1D::MakeInterpolator(ttms, rates, method);
    step_ = Tools::GetStepPerYear(freq);
    n_ = Tools::GetNumPerYear(freq);
  }

  virtual ~TermStructure() = default;

  TermStructure(const TermStructure& other) {
    n_ = other.n_;
    step_ = other.step_;
    method_ = other.method_;
    interpolater_ = Interpolation::Interpolator1D::MakeInterpolator(other.interpolater_->GetXs(), other.interpolater_->GetYs(), other.method_);
  }

  virtual ZeroRate GetZero(Time t) = 0;

  virtual ForwardRate GetForward(Time t) = 0;

  virtual ForwardRate GetForward(Time t1, Time t2);

  virtual DiscountFactor GetDiscountFactor(Time t) = 0;

  virtual std::shared_ptr<TermStructure> Bump(BumpAmount amount) const = 0;

  virtual ParRate GetPar(Time t);

  void AddRate(const std::pair<Time, Rate>& param) {
    interpolater_->AddParam(param);
  }

  void BumpInPlace(BumpAmount amount);

 protected:
  std::shared_ptr<Interpolation::Interpolator1D> interpolater_;
  double step_;
  double n_;
  Interpolation::InterpolationMethod method_;

 protected:
  ParRate computeParRate(SizeT num_step);
};

class TermStructureZero : public TermStructure {
 public:
  explicit TermStructureZero() = default;
  explicit TermStructureZero(const std::vector<Time>& ttms,
                             const std::vector<ZeroRate>& zero_rates,
                             const Frequency& freq = Frequency::Quarterly,
                             const Interpolation::InterpolationMethod& method =
                                 Interpolation::InterpolationMethod::Linear)
      : TermStructure(ttms, zero_rates, freq, method){};
  virtual ~TermStructureZero() = default;

  virtual ZeroRate GetZero(Time t) override { return (*interpolater_)(t); }

  virtual std::shared_ptr<TermStructure> Bump(BumpAmount amount) const override;

  virtual ForwardRate GetForward(Time t) override;

  virtual DiscountFactor GetDiscountFactor(Time t) override;

 private:
  ForwardRate computeForwardRate(SizeT num_step);
};

class TermStructureZeroSimple final : public TermStructureZero {
 public:
  explicit TermStructureZeroSimple() = default;
  explicit TermStructureZeroSimple(
      const std::vector<Time>& ttms, const std::vector<ZeroRate>& zero_rates,
      const Frequency& freq = Frequency::Quarterly,
      const Interpolation::InterpolationMethod& method =
          Interpolation::InterpolationMethod::Linear)
      : TermStructureZero(ttms, zero_rates, freq, method){};

  static ZeroRate GetZeroFromDisc(DiscountFactor disc, Time t);

  virtual ~TermStructureZeroSimple() = default;

  virtual std::shared_ptr<TermStructure> Bump(BumpAmount amount) const override;

  virtual ForwardRate GetForward(Time t) override;

  virtual DiscountFactor GetDiscountFactor(Time t) override;

  ZeroRate GetZeroFromForward(ForwardRate forward, Time t1, Time t2);
};

class TermStructureForward : public TermStructure {
 public:
  explicit TermStructureForward(
      const std::vector<Time>& ttms, const std::vector<ForwardRate>& fwd_rates,
      const Frequency& freq = Frequency::Quarterly,
      const Interpolation::InterpolationMethod& method =
          Interpolation::InterpolationMethod::Linear)
      : TermStructure(ttms, fwd_rates, freq, method){};

  virtual ~TermStructureForward() = default;
};

class TermStructureForwardSimple final : public TermStructureForward {
 public:
  explicit TermStructureForwardSimple() = default;
  explicit TermStructureForwardSimple(
      const std::vector<Time>& ttms, const std::vector<ForwardRate>& fwd_rates,
      const Frequency& freq = Frequency::Quarterly,
      const Interpolation::InterpolationMethod& method =
          Interpolation::InterpolationMethod::Linear)
      : TermStructureForward(ttms, fwd_rates, freq, method){};

  virtual ~TermStructureForwardSimple() = default;

  virtual std::shared_ptr<TermStructure> Bump(BumpAmount amount) const override;

  virtual ForwardRate GetForward(Time t) override {
    return (*interpolater_)(t);
  }

  virtual ZeroRate GetZero(Time t) override;

  virtual DiscountFactor GetDiscountFactor(Time t) override;

 private:
  DiscountFactor computeDiscountFactor(SizeT num_step);
};
}  // namespace IR
