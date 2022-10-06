#pragma once
#include "Utilities.h"
#include "Defines.h"

namespace IR {
namespace Instrument {
class IRSwap {
public:
  IRSwap() = default;
  
  explicit IRSwap(
    Notional notional,
    SwapRate swap_rate,
    Time ttm,
    const FloatLegFrequency& float_freq = FloatLegFrequency::Quarterly,
    const FixedLegFrequency& fixed_freq = FixedLegFrequency::SemiAnnually,
    bool is_pay_fixed = true) 
    : float_freq_(float_freq), fixed_freq_(fixed_freq), ttm_(ttm), swap_rate_(swap_rate), 
    is_pay_fixed_(is_pay_fixed), notional_(notional)
    {
      setSchedule();
    }
  
  virtual ~IRSwap() = default;

  Notional GetNotional() const {
    return notional_;
  }

  SwapRate GetSwapRate() const {
    return swap_rate_;
  }

  Time GetTTM() const {
    return ttm_;
  }

  FloatLegFrequency GetFloatFrequency() const {
    return float_freq_;
  }

  FixedLegFrequency GetFixedFrequency() const {
    return fixed_freq_;
  }

  FloatLegSchedule GetFloatSchedule() const {
    return float_schedule_;
  }

  FixedLegSchedule GetFixedSchedule() const {
    return fixed_schedule_;
  }

  bool GetIsPayFixed() const {
    return is_pay_fixed_;
  }

protected:
  const Time ttm_;
  const SwapRate swap_rate_;
  const FloatLegFrequency float_freq_;
  const FixedLegFrequency fixed_freq_;
  const bool is_pay_fixed_;
  const Notional notional_;

  FixedLegSchedule fixed_schedule_;
  FloatLegSchedule float_schedule_;

protected:
  void setSchedule();
  
};
}
}
