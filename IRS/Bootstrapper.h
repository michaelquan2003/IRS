#pragma once
#include "Instrument.h"
#include "Pricer.h"
#include "TermStructure.h"

namespace IR {
namespace Bootstrap {
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

class BootstrapperDiffCurve : public Bootstrapper {
 public:
  explicit BootstrapperDiffCurve() : Bootstrapper(){};
  explicit BootstrapperDiffCurve(
      const std::vector<std::shared_ptr<Instrument::IRSwap>>& instrs,
      ZeroRate zero_rate_3m)
      : Bootstrapper(instrs, zero_rate_3m) {}

  virtual ~BootstrapperDiffCurve() = default;
  virtual std::shared_ptr<TermStructure> Bootstrap() override;
};
}  // namespace Bootstrap
}  // namespace IR
