#include "Instrument.h"

namespace IR {
namespace Instrument {
void IRSwap::setSchedule() {
  auto fixed_num_per_year = Tools::GetNumPerYear(fixed_freq_);
  auto float_num_per_year = Tools::GetNumPerYear(float_freq_);
  auto fixed_step_per_year = Tools::GetStepPerYear(fixed_freq_);
  auto float_step_per_year = Tools::GetStepPerYear(float_freq_);

  auto fixed_num_step = static_cast<SizeT>(std::floor(ttm_ * fixed_num_per_year)) + 1;
  auto float_num_step = static_cast<SizeT>(std::floor(ttm_ * float_num_per_year)) + 1;
  fixed_schedule_ = Tools::linspace(fixed_step_per_year, ttm_, fixed_num_step);
  float_schedule_ = Tools::linspace(float_step_per_year, ttm_, float_num_step);
}
}
}