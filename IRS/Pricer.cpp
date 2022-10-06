#include "Pricer.h"

namespace IR {
namespace Pricer {
Value IRSwapPricer::NPV(
  const std::shared_ptr<Instrument::IRSwap>& instr,
  const std::shared_ptr<ProjectionCurve>& proj_curve,
  const std::shared_ptr<DiscountCurve>& disc_curve) {
  auto notional = instr->GetNotional();
  auto swap_rate = instr->GetSwapRate();
  auto fixed_schedule = instr->GetFixedSchedule();
  auto float_schedule = instr->GetFloatSchedule();
  auto is_pay_fixed = instr->GetIsPayFixed();
  double fixed_sum = 0, annuity_factor = 0;
  for (SizeT i = 1; i < fixed_schedule.size(); i++) {
    auto dt = fixed_schedule[i] - fixed_schedule[i-1];
    auto disc = disc_curve->GetDiscountFactor(fixed_schedule[i]);
    annuity_factor += disc * dt;
  }
  fixed_sum = notional * swap_rate * annuity_factor;
  auto float_sum = notional * (proj_curve->GetDiscountFactor(float_schedule.front()) - proj_curve->GetDiscountFactor(float_schedule.back()));
  auto multiplier = is_pay_fixed ? 1 : -1;
  return multiplier * (float_sum - fixed_sum);
}
}
}