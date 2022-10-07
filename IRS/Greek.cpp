#include "Greek.h"

namespace IR {
namespace Greek {

void DV01::setStencilsandWeights() {
  switch (method_) {
    case FiniteDifferenceMethod::Central: {
      stencils_ = {-1, 0, 1};
      weights_ = {-0.5, 0, 0.5};
    } break;
    case FiniteDifferenceMethod::Backward: {
      stencils_ = {-1, 0};
      weights_ = {-1, 1};
    } break;
    case FiniteDifferenceMethod::Forward: {
      stencils_ = {0, 1};
      weights_ = {-1, 1};
    } break;
    default:
      throw std::invalid_argument(
          "Only finite difference method forward, backward, central are "
          "supported!");
  }
  prices_.resize(stencils_.size());
}

void Convexity::setStencilsandWeights() {
  switch (method_) {
    case FiniteDifferenceMethod::Central: {
      stencils_ = {-1, 0, 1};
      weights_ = {1, -2, 1};
    } break;
    case FiniteDifferenceMethod::Backward: {
      stencils_ = {-2, -1, 0};
      weights_ = {1, -2, 1};
    } break;
    case FiniteDifferenceMethod::Forward: {
      stencils_ = {0, 1, 2};
      weights_ = {1, -2, 1};
    } break;
    default:
      throw std::invalid_argument(
          "Only finite difference method forward, backward, central are "
          "supported!");
  }
  prices_.resize(stencils_.size());
}

Value Greek::Compute(const std::shared_ptr<Pricer::IRSwapPricer>& pricer,
                     const std::shared_ptr<Instrument::IRSwap>& instr,
                     const std::shared_ptr<Pricer::ProjectionCurve>& proj_curve,
                     const std::shared_ptr<Pricer::DiscountCurve>& disc_curve) {
  for (SizeT i = 0; i < prices_.size(); i++) {
    auto bump_amount = stencils_[i] * h_;
    auto bumped_proj_curve = proj_curve->Bump(bump_amount);
    auto bumped_disc_curve = disc_curve->Bump(bump_amount);
    prices_[i] = pricer->NPV(instr, bumped_proj_curve, bumped_disc_curve);
  }
  double greek = 0.;
  for (SizeT i = 0; i < prices_.size(); i++) {
    greek += weights_[i] * prices_[i];
  }
  greek /= std::pow(h_, order_);
  return greek;
}

}  // namespace Greek

}  // namespace IR