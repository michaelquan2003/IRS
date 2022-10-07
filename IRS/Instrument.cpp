#include "Instrument.h"

namespace IR {
namespace Instrument {
void IRSwap::setSchedule() {
  auto fixed_num_per_year = Tools::GetNumPerYear(fixed_freq_);
  auto float_num_per_year = Tools::GetNumPerYear(float_freq_);
  auto fixed_step_per_year = Tools::GetStepPerYear(fixed_freq_);
  auto float_step_per_year = Tools::GetStepPerYear(float_freq_);

  auto fixed_num_step =
      static_cast<SizeT>(std::floor(ttm_ * fixed_num_per_year));
  auto float_num_step =
      static_cast<SizeT>(std::floor(ttm_ * float_num_per_year));
  fixed_schedule_ = Tools::linspace(0., fixed_step_per_year * fixed_num_step,
                                    fixed_num_step + 1);
  float_schedule_ = Tools::linspace(0., float_step_per_year * float_num_step,
                                    float_num_step + 1);
}
}  // namespace Instrument
}  // namespace IR