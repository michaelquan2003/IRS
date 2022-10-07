#pragma once
#include "Instrument.h"
#include "TermStructure.h"

namespace IR {
namespace Pricer {

//Interest Rate Swap Pricer. We could make abstract base class and do inheritance if we actually have different instruments to price
class IRSwapPricer final {
 public:
  explicit IRSwapPricer() = default;
  ~IRSwapPricer() = default;

  Value NPV(const std::shared_ptr<Instrument::IRSwap>& instr,
            const std::shared_ptr<ProjectionCurve>& proj_curve,
            const std::shared_ptr<DiscountCurve>& disc_curve);
};
}  // namespace Pricer
}  // namespace IR
