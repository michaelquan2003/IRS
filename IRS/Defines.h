#pragma once
#include <memory>
#include <vector>

using SizeT = std::size_t;

namespace IR {
class TermStructure;

using Time = double;
using ZeroRate = double;
using ForwardRate = double;
using DiscountFactor = double;
using ParRate = double;
using Rate = double;
using BumpAmount = double;
using Value = double;

enum class Frequency {
  Yearly,
  SemiAnnually,
  Quarterly,
  Monthly
};

namespace Greek {
using Stencils = std::vector<double>;
using Weights = std::vector<double>;
using Prices = std::vector<double>;
using Step = double;
using Order = SizeT;

enum class FiniteDifferenceMethod {
  Forward, Backward, Central
};
}

namespace Instrument {
using SwapRate = double;
using Notional = double;
using FixedLegFrequency = Frequency;
using FloatLegFrequency = Frequency;
using FixedLegSchedule = std::vector<double>;
using FloatLegSchedule = std::vector<double>;
}

namespace Pricer {
using ProjectionCurve = TermStructure;
using DiscountCurve = TermStructure;
}
}

namespace Tools {
constexpr double EPSILON = 1E-6;
}

namespace Interpolation {
using X = double;
using Y = double;

enum class InterpolationMethod {
  Linear, LogLinear, CubicSpline, AkimaSpline
};



}
