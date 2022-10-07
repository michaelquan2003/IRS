// main.cpp : This file contains the 'main' function. Program execution begins
// and ends there.
//
#include <iostream>

#include "Bootstrapper.h"
#include "Greek.h"
#include "Instrument.h"
#include "Pricer.h"
#include "TermStructure.h"

namespace {
// Convenient method for creating swaps
std::vector<std::shared_ptr<IR::Instrument::IRSwap>> MakeSwaps(
    bool is_libor_swap = true) {
  std::vector<std::shared_ptr<IR::Instrument::IRSwap>> output;
  auto notional = 100.;
  auto num_swap = 10;
  auto last_year = 10.;
  auto ttms = Tools::linspace(1., last_year, num_swap);
  for (auto& ttm : ttms) {
    auto swap_rate = 0.04 + 0.02 * ttm / 30;
    if (!is_libor_swap) {
      swap_rate -= 0.01;
    }
    auto swap =
        std::make_shared<IR::Instrument::IRSwap>(notional, swap_rate, ttm);
    output.push_back(swap);
  }
  return output;
}

// Convenient method for creating discount curve
std::shared_ptr<IR::TermStructure> MakeDiscCurve(
    bool is_libor_curve = true) {
  auto num_steps = 200;
  auto last_year = 50.;
  auto ttms = Tools::linspace(0.25, last_year, num_steps);
  std::vector<IR::ZeroRate> zero_rates;
  zero_rates.reserve(ttms.size());
  for (const auto& ttm : ttms) {
    auto zero_rate = 0.04 + 0.02 * ttm / 30;
    if (!is_libor_curve) {
      zero_rate -= 0.01;
    }
    zero_rates.push_back(zero_rate);
  }
  return std::make_shared<IR::TermStructureZeroSimple>(ttms, zero_rates);
}
}  // namespace

namespace TestCase {
// Test Case 1: Test term structures can reprice all libor swaps to par.
// Projection and Discount curve are both LIBOR curve
void TestCase1() {
  using namespace IR;
  using namespace std;
  auto libor_swaps = MakeSwaps(true);
  double zero_rate_3m = 0.02;
  Pricer::IRSwapPricer pricer;

  Bootstrap::BootstrapperSameCurve bootstrapper(libor_swaps, zero_rate_3m);
  auto term_structure = bootstrapper.Bootstrap();
  std::vector<double> npvs;
  cout << "Test Case 1" << endl;
  for (auto& libor_swap : libor_swaps) {
    auto npv = pricer.NPV(libor_swap, term_structure, term_structure);
    // NPV below should all be zero
    cout << "TTM: " << libor_swap->GetTTM() << ", NPV: " << npv << endl;
    npvs.push_back(npv);
  }
  cout << endl;
}

// Test Case 2: Test term structures can reprice all libor swaps to par.
// Projection curve using LIBOR CURVE and Discount curve using OIS Curve
void TestCase2() {
  using namespace IR;
  using namespace std;
  auto libor_swaps = MakeSwaps(true);
  double zero_rate_3m = 0.02;
  Pricer::IRSwapPricer pricer;

  auto ois_term_structure = MakeDiscCurve(false);

  Bootstrap::BootstrapperDiffCurve bootstrapper_diff_curve(
      libor_swaps, zero_rate_3m, ois_term_structure);
  auto libor_term_structure = bootstrapper_diff_curve.Bootstrap();
  std::vector<double> npvs;
  cout << "Test Case 2" << endl;
  for (auto& libor_swap : libor_swaps) {
    auto npv = pricer.NPV(libor_swap, libor_term_structure, ois_term_structure);
    // NPV below should all be zero
    cout << "TTM: " << libor_swap->GetTTM() << ", NPV: " << npv << endl;
    npvs.push_back(npv);
  }
  cout << endl;
}

// Test Case 3: Test DV01 and Convexity
void TestCase3() {
  using namespace std;
  using namespace IR;
  std::cout << "Test Case 3" << std::endl;
  double h = 0.0001;  // 1 basis point step size
  Greek::DV01 dv01(h);
  Greek::Convexity convexity(h);
  auto ttms = Tools::linspace(0.5, 3., 6);
  std::vector<double> zero_rates{0.02,     0.024024, 0.027669,
                                 0.030974, 0.033975, 0.036701};
  auto term_structure = std::make_shared<IR::TermStructureZeroSimple>(
      ttms, zero_rates, IR::Frequency::SemiAnnually);
  auto pricer = std::make_shared<Pricer::IRSwapPricer>();
  auto swap = std::make_shared<Instrument::IRSwap>(100, 0.05, 1, Frequency::Quarterly, Frequency::SemiAnnually, false);
  auto dv01_value = dv01.Compute(pricer, swap, term_structure, term_structure);
  auto convexity_value = convexity.Compute(pricer, swap, term_structure, term_structure);
  cout << "DV01: " << dv01_value << endl;
  cout << "Convexity: " << convexity_value << endl;
  cout << endl;
}

// Test Case 4: Test Term Structure is working properly
void TestCase4() {
  std::cout << "Test Case 4" << std::endl;
  using namespace IR;
  auto ttms = Tools::linspace(0.5, 3., 6);
  std::vector<double> zero_rates{0.02,     0.024024, 0.027669,
                                 0.030974, 0.033975, 0.036701};
  IR::TermStructureZeroSimple term_structure(ttms, zero_rates,
                                             IR::Frequency::SemiAnnually);
  auto ttms_out = Tools::linspace(0.5, 2.75, 11);
  for (auto& ttm : ttms_out) {
    std::cout << "TTM:" << ttm << std::endl;
    std::cout << "ZERO: " << term_structure.GetZero(ttm) << std::endl;
    std::cout << "PAR: " << term_structure.GetPar(ttm) << std::endl;
    std::cout << "Forward: " << term_structure.GetForward(ttm) << std::endl;
    std::cout << "Discount factor: " << term_structure.GetDiscountFactor(ttm)
              << std::endl;
    std::cout << std::endl;
  }
}
}  // namespace TestCase

int main() {
  try {
    TestCase::TestCase1();
    TestCase::TestCase2();
    TestCase::TestCase3();
    TestCase::TestCase4();
  } catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
}
