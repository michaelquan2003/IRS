#include "TermStructure.h"

namespace IR {
ForwardRate TermStructure::GetForward(Time t1, Time t2) {
  auto disc_t1 = GetDiscountFactor(t1);
  auto disc_t2 = GetDiscountFactor(t2);
  auto forward_rate = (disc_t1 / disc_t2 - 1) / (t2 - t1);
  return forward_rate;
}

ParRate TermStructure::computeParRate(SizeT num_step) {
  auto ttm = num_step * step_;
  if (num_step <= 1) {
    return GetZero(ttm);
  }
  auto ttms = Tools::linspace<double>(step_, ttm, num_step);
  auto notional_discounted = 1 - GetDiscountFactor(ttms[num_step - 1]);
  auto coupon_discounted = 0.;
  for (const auto& ttm : ttms) {
    coupon_discounted += GetDiscountFactor(ttm);
  }
  auto par_rate = (notional_discounted / coupon_discounted) * n_;
  return par_rate;
}

ParRate TermStructure::GetPar(Time t) {
  auto num_step = static_cast<SizeT>(std::floor(t / step_));
  if (Tools::is_equal(num_step, static_cast<SizeT>(0))) {
    return GetZero(t);
  }

  auto ttm_left = step_ * num_step;
  if (Tools::is_equal(ttm_left, t)) {
    auto par_rate = computeParRate(num_step);
    return par_rate;
  }

  auto ttm_right = ttm_left + step_;
  auto num_step_plus_1 = num_step + 1;
  auto par_rate_left = computeParRate(num_step);
  auto par_rate_right = computeParRate(num_step_plus_1);
  return par_rate_left + (par_rate_right - par_rate_left) /
                             (ttm_right - ttm_left) * (t - ttm_left);
}

void TermStructure::BumpInPlace(BumpAmount amount) {
  auto params = interpolater_->GetParams();
  for (auto& param : params) {
    param.second += amount;
  }
  interpolater_->SetParams(params);
}

std::shared_ptr<TermStructure> TermStructureZero::Bump(
    BumpAmount amount) const {
  auto copy = std::make_shared<TermStructureZero>(*this);
  copy->BumpInPlace(amount);
  return copy;
}

ForwardRate TermStructureZero::computeForwardRate(SizeT num_step) {
  auto num_step_plus_1 = num_step + 1;
  auto ttm = num_step * step_;
  auto ttm_plus_1 = num_step_plus_1 * step_;
  auto disc_ttm = GetDiscountFactor(ttm);
  auto disc_ttm_plus_1 = GetDiscountFactor(ttm_plus_1);
  auto forward_rate = (disc_ttm / disc_ttm_plus_1 - 1) * n_;
  return forward_rate;
}

ForwardRate TermStructureZero::GetForward(Time t) {
  auto num_step = static_cast<SizeT>(std::floor(t / step_));
  if (Tools::is_equal(num_step, static_cast<SizeT>(0))) {
    return GetZero(t);
  }

  auto ttm_left = step_ * num_step;
  if (Tools::is_equal(ttm_left, t)) {
    auto forward_rate = computeForwardRate(num_step);
    return forward_rate;
  }

  auto ttm_right = ttm_left + step_;
  auto num_step_plus_1 = num_step + 1;
  auto forward_rate_left = computeForwardRate(num_step);
  auto forward_rate_right = computeForwardRate(num_step_plus_1);
  return forward_rate_left + (forward_rate_right - forward_rate_left) /
                                 (ttm_right - ttm_left) * (t - ttm_left);
}

DiscountFactor TermStructureZero::GetDiscountFactor(Time t) {
  auto zero_rate = GetZero(t);
  return 1 / std::pow(1 + (zero_rate / n_), t * n_);
}

std::shared_ptr<TermStructure> TermStructureZeroSimple::Bump(
    BumpAmount amount) const {
  auto copy = std::make_shared<TermStructureZeroSimple>(*this);
  copy->BumpInPlace(amount);
  return copy;
}

DiscountFactor TermStructureZeroSimple::GetDiscountFactor(Time t) {
  auto zero_rate = GetZero(t);
  auto disc = 1 / (1 + zero_rate * t);
  return disc;
}

ZeroRate TermStructureZeroSimple::GetZeroFromDisc(DiscountFactor disc, Time t) {
  auto zero_rate = (1 / t) * (1 / disc - 1);
  return zero_rate;
}

ForwardRate TermStructureZeroSimple::GetForward(Time t) {
  auto t_plus_1 = t + step_;
  auto disc_t = GetDiscountFactor(t);
  auto disc_t_plus_1 = GetDiscountFactor(t_plus_1);
  auto forward_rate = (disc_t / disc_t_plus_1 - 1) * n_;
  return forward_rate;
}

std::shared_ptr<TermStructure> TermStructureForwardSimple::Bump(
    BumpAmount amount) const {
  auto copy = std::make_shared<TermStructureForwardSimple>(*this);
  copy->BumpInPlace(amount);
  return copy;
}

DiscountFactor TermStructureForwardSimple::computeDiscountFactor(
    SizeT num_step) {
  auto disc = 1.;
  for (SizeT i = 0; i < num_step; i++) {
    auto ttm = i * step_;
    disc *= 1 / (1 + GetForward(ttm) * step_);
  }
  return disc;
}

DiscountFactor TermStructureForwardSimple::GetDiscountFactor(Time t) {
  auto num_step = static_cast<SizeT>(std::floor(t / step_));
  auto ttm_left = step_ * num_step;
  if (Tools::is_equal(ttm_left, t)) {
    auto disc = computeDiscountFactor(num_step);
    return disc;
  }
  auto ttm_right = ttm_left + step_;
  auto num_step_plus_1 = num_step + 1;
  auto zero_rate_left = GetZero(ttm_left);
  auto zero_rate_right = GetZero(ttm_right);
  auto zero_rate = zero_rate_left +
                   (zero_rate_right - zero_rate_left) / step_ * (t - ttm_left);
  auto disc = 1 / (1 + zero_rate * t);
  return disc;
}

ZeroRate TermStructureForwardSimple::GetZero(Time t) {
  auto disc = GetDiscountFactor(t);
  auto zero_rate = (1 - disc) / (t * disc);
  return zero_rate;
}
}  // namespace IR