#pragma once
#include <memory>

using SizeT = std::size_t;

namespace Tools {
constexpr double EPSILON = 1E-6;
}

namespace IR {
using Time = double;
using ZeroRate = double;
using ForwardRate = double;
using DiscountFactor = double;
using ParRate = double;
using Rate = double;

enum class Frequency {
  Yearly,
  SemiAnnually,
  Quarterly
};
}

namespace Interpolation {
using X = double;
using Y = double;
enum class InterpolationMethod {
  Linear
};



}
