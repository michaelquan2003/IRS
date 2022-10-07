#include "Bootstrapper.h"

namespace IR {
namespace Bootstrap {
void Bootstrapper::prepDataFromInput(
    const std::vector<std::shared_ptr<Instrument::IRSwap>>& instrs) {
  std::vector<double> swap_rates;
  std::vector<double> ttms;
  swap_rates.reserve(instrs.size());
  ttms.reserve(instrs.size());
  for (const auto& instr : instrs) {
    ttms.push_back(instr->GetTTM());
    swap_rates.push_back(instr->GetSwapRate());
    if (instrs_.find(instr->GetTTM()) == instrs_.cend()) {
      instrs_.insert({instr->GetTTM(), instr});
    }
  }

  swap_rates_interpolator_ =
      Interpolation::Interpolator1D::MakeInterpolator(ttms, swap_rates);
}

std::shared_ptr<TermStructure> BootstrapperSameCurve::Bootstrap() {
  auto notional = instrs_.begin()->second->GetNotional();
  auto float_freq = instrs_.begin()->second->GetFloatFrequency();
  auto fixed_freq = instrs_.begin()->second->GetFixedFrequency();
  auto float_step = Tools::GetStepPerYear(float_freq);
  auto fixed_step = Tools::GetStepPerYear(fixed_freq);
  auto ttm_start = 0.25;
  auto ttm = ttm_start + float_step;
  std::vector<double> ttms{ttm_start};
  std::vector<double> zero_rates{zero_rate_3m_};

  auto term_structure =
      std::make_shared<TermStructureZeroSimple>(ttms, zero_rates, float_freq);
  Pricer::IRSwapPricer pricer;
  auto last_ttm = std::prev(instrs_.end())->first;
  while (ttm <= last_ttm) {
    auto swap_rate = (*swap_rates_interpolator_)(ttm);
    auto ttm_minus_1 = ttm - float_step;
    auto swap_minus_1 =
        std::make_shared<Instrument::IRSwap>(notional, swap_rate, ttm_minus_1);
    auto swap = std::make_shared<Instrument::IRSwap>(notional, swap_rate, ttm);
    auto fixed_schedule = swap->GetFixedSchedule();
    auto price_minus_1 =
        pricer.NPV(swap_minus_1, term_structure, term_structure);
    auto disc_minus_1 = term_structure->GetDiscountFactor(ttm_minus_1);
    auto denominator = Tools::is_equal(fixed_schedule.back(), ttm)
                           ? 1 + swap_rate * fixed_step
                           : 1.;
    auto disc = (disc_minus_1 + price_minus_1 / notional) / denominator;
    auto zero_rate = term_structure->GetZeroFromDisc(disc, ttm);
    term_structure->AddRate(std::make_pair(ttm, zero_rate));
    ttm += float_step;
  }
  return term_structure;
}

std::shared_ptr<TermStructure> BootstrapperDiffCurve::Bootstrap() {
  std::vector<double> ttms{0.25};
  std::vector<double> zero_rates{zero_rate_3m_};
  auto float_freq = instrs_[0]->GetFloatFrequency();
  IR::TermStructureZeroSimple term_structure(ttms, zero_rates, float_freq);
  return nullptr;
}

}  // namespace Bootstrap
}  // namespace IR