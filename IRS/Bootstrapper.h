#pragma once
#include "Instrument.h"
#include "Pricer.h"
#include "TermStructure.h"

namespace IR {
namespace Bootstrap {

//Abstract Bootstrapper
class Bootstrapper {
 public:
  explicit Bootstrapper() = default;
  explicit Bootstrapper(
      const std::vector<std::shared_ptr<Instrument::IRSwap>>& instrs,
      ZeroRate zero_rate_3m)
      : zero_rate_3m_(zero_rate_3m) {
    prepDataFromInput(instrs);
  }

  virtual ~Bootstrapper() = default;
  virtual std::shared_ptr<TermStructure> Bootstrap() = 0;

 protected:
  std::map<Time, std::shared_ptr<Instrument::IRSwap>> instrs_;
  ZeroRate zero_rate_3m_ = 0;
  std::shared_ptr<Interpolation::Interpolator1D> swap_rates_interpolator_;

 protected:
  void prepDataFromInput(
      const std::vector<std::shared_ptr<Instrument::IRSwap>>& instrs);
};

//Bootstrapper assuming projection curve and discount curve to be the same
class BootstrapperSameCurve : public Bootstrapper {
 public:
  explicit BootstrapperSameCurve() : Bootstrapper(){};
  explicit BootstrapperSameCurve(
      const std::vector<std::shared_ptr<Instrument::IRSwap>>& instrs,
      ZeroRate zero_rate_3m)
      : Bootstrapper(instrs, zero_rate_3m) {}

  virtual ~BootstrapperSameCurve() = default;
  virtual std::shared_ptr<TermStructure> Bootstrap() override;
};

// Bootstrapper assuming projection curve and discount curve to be different
class BootstrapperDiffCurve : public Bootstrapper {
 public:
  explicit BootstrapperDiffCurve() : Bootstrapper(){};
  explicit BootstrapperDiffCurve(
      const std::vector<std::shared_ptr<Instrument::IRSwap>>& instrs,
      ZeroRate zero_rate_3m,
      const std::shared_ptr<TermStructure>& discount_curve)
      : Bootstrapper(instrs, zero_rate_3m), discount_curve_(discount_curve) {}

  virtual ~BootstrapperDiffCurve() = default;
  virtual std::shared_ptr<TermStructure> Bootstrap() override;

protected:
  const std::shared_ptr<TermStructure> discount_curve_;
};
}  // namespace Bootstrap
}  // namespace IR
