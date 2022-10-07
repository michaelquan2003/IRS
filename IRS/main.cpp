// IRS.cpp : This file contains the 'main' function. Program execution begins
// and ends there.
//
#include <boost/log/trivial.hpp>
#include <iostream>

#include "Bootstrapper.h"
#include "Instrument.h"
#include "Pricer.h"
#include "TermStructure.h"

namespace {
std::vector<std::shared_ptr<IR::Instrument::IRSwap>> MakeSwaps() {
  std::vector<std::shared_ptr<IR::Instrument::IRSwap>> output;
  auto notional = 100.;
  auto num_swap = 50;
  auto last_year = 50.;
  auto ttms = Tools::linspace(1., last_year, num_swap);
  for (auto& ttm : ttms) {
    auto swap_rate = 0.04 + 0.02 * ttm / 30;
    auto swap =
        std::make_shared<IR::Instrument::IRSwap>(notional, swap_rate, ttm);
    output.push_back(swap);
  }
  return output;
}
}  // namespace

int main() {
  using namespace IR;
  using namespace std;
  auto swaps = MakeSwaps();
  double zero_rate_3m = 0.02;
  Pricer::IRSwapPricer pricer;
  Bootstrap::BootstrapperSameCurve bootstrapper(swaps, zero_rate_3m);
  auto term_structure = bootstrapper.Bootstrap();
  std::vector<double> npvs;
  for (auto& swap : swaps) {
    auto npv = pricer.NPV(swap, term_structure, term_structure);
    npvs.push_back(npv);
  }

  try {
    auto ttms_out = Tools::linspace(0.5, 3., 11);
    for (auto& ttm : ttms_out) {
      std::cout << "TTM:" << ttm << std::endl;
      std::cout << "ZERO: " << term_structure->GetZero(ttm) << std::endl;
      std::cout << "PAR: " << term_structure->GetPar(ttm) << std::endl;
      std::cout << "Forward: " << term_structure->GetForward(ttm) << std::endl;
      std::cout << "Discount factor: " << term_structure->GetDiscountFactor(ttm)
                << std::endl;
      std::cout << std::endl;
    }
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add
//   Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project
//   and select the .sln file
