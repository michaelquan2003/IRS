#pragma once
#include <vector>
#include "Utilities.h"

namespace Interpolation {
class Interpolator1D {
public:

  Interpolator1D(const std::vector<double>& xs, const std::vector<double>& ys, bool is_sorted=false) : xs_(xs), ys_(ys) 
  {
    if (!is_sorted) {
      Sort::sortInPlace(ys_, xs_);
    }
  };
  virtual ~Interpolator1D() {};
  virtual double operator()(double ttm) = 0;

protected:
  std::vector<double> xs_;
  std::vector<double> ys_;

};

class LinearInterpolation1D : public Interpolator1D {
public:
  LinearInterpolation1D(const std::vector<double>& xs, const std::vector<double>& ys, bool is_sorted = false) : Interpolator1D(xs, ys, is_sorted) {};
  virtual ~LinearInterpolation1D() {};
  virtual double operator()(double x) override {
    if (x <= xs_.front()) {
      return ys_.front();
    }
    
    if (x >= xs_.back()) {
      return ys_.back();
    }
    
    auto left_index = std::distance(xs_.cbegin(), std::lower_bound(xs_.cbegin(), xs_.cend(), x));
    auto right_index = left_index + 1;
    auto deriv = (ys_[right_index] - ys_[left_index]) / (xs_[right_index] - xs_[left_index]);
    auto value = ys_[left_index] + deriv * (x - xs_[left_index]);
    return value;
  }

};
}



