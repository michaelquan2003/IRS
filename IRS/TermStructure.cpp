#include "TermStructure.h"

namespace IR {
double TermStructure::computeForwardRate(Time ttm) {
  auto num_step = static_cast<std::size_t>(ttm / step_);
  auto num_step_plus_1 = num_step + 1;
  auto ttm_plus_1 = num_step_plus_1 * step_;
  auto disc_ttm = GetDiscountFactor(ttm);
  auto disc_ttm_plus_1 = GetDiscountFactor(ttm_plus_1);
  auto forward_rate = (disc_ttm / disc_ttm_plus_1 - 1) * n_;
  return forward_rate;
}

double TermStructure::computeParRate(Time ttm) {
  auto num_step = static_cast<std::size_t>(ttm / step_);
  if (num_step == 1) {
    return GetZero(ttm);
  }
  auto ttms = Tools::linspace<double>(step_, ttm, num_step);
  auto notional_discounted = 1 - 1 / (std::pow(1 + GetZero(ttms[num_step - 1]) / n_, ttms[num_step - 1] * n_));
  auto coupon_discounted = 0.;
  for (const auto& ttm : ttms) {
    coupon_discounted += (1 / std::pow(1 + GetZero(ttm) / n_, ttm * n_));
  }
  auto par_rate = (notional_discounted / coupon_discounted) * n_;
  return par_rate;
}

double TermStructure::GetPar(Time t) {
  auto num_step = static_cast<std::size_t>(std::floor(t / step_));
  if (Tools::is_equal(num_step, static_cast<std::size_t>(0))) {
    return GetZero(t);
  }

  auto ttm_left = step_ * num_step;
  if (Tools::is_equal(ttm_left, t)) {
    auto par_rate = computeParRate(ttm_left);
    return par_rate;
  }
  
  auto ttm_right = ttm_left + step_;
  auto par_rate_left = computeParRate(ttm_left);
  auto par_rate_right = computeParRate(ttm_right);
  return par_rate_left + (par_rate_right - par_rate_left) / (ttm_right - ttm_left) * (t - ttm_left);

}

double TermStructure::GetForward(Time t) {
  auto num_step = static_cast<std::size_t>(std::floor(t / step_));
  if (Tools::is_equal(num_step, static_cast<std::size_t>(0))) {
    return GetZero(t);
  }

  auto ttm_left = step_ * num_step;
  if (Tools::is_equal(ttm_left, t)) {
    auto forward_rate = computeForwardRate(ttm_left);
    return forward_rate;
  }
  
  auto ttm_right = ttm_left + step_;
  auto forward_rate_left = computeForwardRate(ttm_left);
  auto forward_rate_right = computeForwardRate(ttm_right);
  return forward_rate_left + (forward_rate_right - forward_rate_left) / (ttm_right - ttm_left) * (t - ttm_left);
}

double TermStructure::GetDiscountFactor(Time t) {
  auto zero_rate = GetZero(t);
  return 1 / std::pow(1 + (zero_rate / n_), t * n_);
}
}