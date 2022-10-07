#include "LinearInterpolation.h"

namespace Interpolation {
std::shared_ptr<Interpolator1D> Interpolator1D::MakeInterpolator(
    const std::vector<X>& xs, const std::vector<Y>& ys,
    const InterpolationMethod& method) {
  switch (method) {
    case InterpolationMethod::Linear:
      return std::make_shared<LinearInterpolation1D>(xs, ys);
      break;
    default:
      throw std::invalid_argument("Does not recognize interpolation method!");
  }
  return nullptr;
}

std::vector<std::pair<X, Y>> Interpolator1D::GetParams() const {
  std::vector<std::pair<X, Y>> output;
  output.reserve(xs_.size());
  for (SizeT i = 0; i < xs_.size(); i++) {
    output.push_back(std::make_pair(xs_[i], ys_[i]));
  }
  return output;
}

void Interpolator1D::SetParams(const std::vector<std::pair<X, Y>>& params) {
  if (params.size() == 0) {
    throw std::runtime_error(
        "Input params size cannot be zero when setting params");
  }
  xs_.resize(params.size());
  ys_.resize(params.size());
  for (SizeT i = 0; i < params.size(); i++) {
    auto param = params[i];
    xs_[i] = param.first;
    ys_[i] = param.second;
  }
  Tools::Sort::sortInPlace(ys_, xs_);
}

void Interpolator1D::AddParam(const std::pair<X, Y>& param) {
  xs_.push_back(param.first);
  ys_.push_back(param.second);
  Tools::Sort::sortInPlace(ys_, xs_);
}
}  // namespace Interpolation