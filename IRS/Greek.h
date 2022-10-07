#pragma once
#include "Instrument.h"
#include "Pricer.h"

namespace IR {
namespace Greek {
class Greek {
 public:
  Greek() = default;
  explicit Greek(
      Step h, Order order,
      const FiniteDifferenceMethod method = FiniteDifferenceMethod::Forward)
      : h_(h), method_(method), order_(order) {
    setStencilsandWeights();
  }
  virtual ~Greek() = default;

  virtual Value Compute(
      const std::shared_ptr<Pricer::IRSwapPricer>& pricer,
      const std::shared_ptr<Instrument::IRSwap>& instr,
      const std::shared_ptr<Pricer::ProjectionCurve>& proj_curve,
      const std::shared_ptr<Pricer::DiscountCurve>& disc_curve);

  Step GetStepSize() const { return h_; }

  void SetStepSize(Step h) { h_ = h; }

  FiniteDifferenceMethod GetMethod() const { return method_; }

  void SetMethod(const FiniteDifferenceMethod& method) {
    method_ = method;
    setStencilsandWeights();
  }

 protected:
  const Order order_;

  Stencils stencils_;
  Weights weights_;
  Step h_;
  FiniteDifferenceMethod method_;
  Prices prices_;

 protected:
  virtual void setStencilsandWeights() = 0;
};

class DV01 : public Greek {
 public:
  explicit DV01(Step h, const FiniteDifferenceMethod method =
                            FiniteDifferenceMethod::Forward)
      : Greek(h, 1, method){};
  virtual ~DV01() = default;

 protected:
  virtual void setStencilsandWeights() override;
};

class Convexity : public Greek {
 public:
  explicit Convexity(Step h, const FiniteDifferenceMethod method =
                                 FiniteDifferenceMethod::Forward)
      : Greek(h, 2, method){};
  virtual ~Convexity() = default;

 protected:
  virtual void setStencilsandWeights() override;
};

}  // namespace Greek
}  // namespace IR