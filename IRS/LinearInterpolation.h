#pragma once
#include <vector>

#include "Utilities.h"

namespace Interpolation {
class Interpolator1D {
 public:
  Interpolator1D(const std::vector<X>& xs, const std::vector<Y>& ys,
                 bool is_sorted = false)
      : xs_(xs), ys_(ys) {
    if (!is_validated()) {
      throw std::runtime_error("Xs size is not equal to ys size!");
    }
    if (!is_sorted) {
      Tools::Sort::sortInPlace(ys_, xs_);
    }
  };
  virtual ~Interpolator1D(){};
  virtual double operator()(X x) = 0;
  static std::shared_ptr<Interpolator1D> MakeInterpolator(
      const std::vector<X>& xs, const std::vector<Y>& ys,
      const InterpolationMethod& method = InterpolationMethod::Linear);

  std::vector<std::pair<X, Y>> GetParams() const;
  void SetParams(const std::vector<std::pair<X, Y>>&);
  void AddParam(const std::pair<X, Y>&);
  std::vector<X> GetXs() const { return xs_; }

  std::vector<Y> GetYs() const { return ys_; }

 protected:
  bool is_validated() const {
    if (xs_.size() == ys_.size()) {
      return true;
    }
    return false;
  }

 protected:
  std::vector<X> xs_;
  std::vector<Y> ys_;
};

class LinearInterpolation1D : public Interpolator1D {
 public:
  LinearInterpolation1D(const std::vector<X>& xs, const std::vector<Y>& ys,
                        bool is_sorted = false)
      : Interpolator1D(xs, ys, is_sorted){};
  virtual ~LinearInterpolation1D(){};
  virtual double operator()(X x) override {
    if (x <= xs_.front()) {
      return ys_.front();
    }

    if (x >= xs_.back()) {
      return ys_.back();
    }

    auto right_index = std::distance(
        xs_.cbegin(), std::lower_bound(xs_.cbegin(), xs_.cend(), x));
    auto left_index = right_index - 1;
    auto deriv = (ys_[right_index] - ys_[left_index]) /
                 (xs_[right_index] - xs_[left_index]);
    auto value = ys_[left_index] + deriv * (x - xs_[left_index]);
    return value;
  }
};
}  // namespace Interpolation
