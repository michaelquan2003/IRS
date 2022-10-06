// IRS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <boost/log/trivial.hpp>
#include "TermStructure.h"

#include <iostream>

int main()
{
  auto ttms = Tools::linspace(0., 3., 6);
  std::vector<double>forward_rates {0.02, 0.024024, 0.027669, 0.030974, 0.033975, 0.036701};
  IR::TermStructureForwardSimple term_structure(ttms, forward_rates, IR::Frequency::SemiAnnually);
  try {
    auto ttms_out = Tools::linspace(0.5, 3., 11);
    for (auto& ttm : ttms_out) {
      std::cout << "TTM:" << ttm << std::endl;
      std::cout << "ZERO: " << term_structure.GetZero(ttm) << std::endl;
      std::cout<<  "PAR: " << term_structure.GetPar(ttm) << std::endl;
      std::cout << "Forward: " << term_structure.GetForward(ttm) << std::endl;
      std::cout << "Discount factor: " << term_structure.GetDiscountFactor(ttm) << std::endl;
      std::cout<<std::endl;
    }
  }
  catch (std::exception e) {
    std::cout<<e.what()<<std::endl;
  }

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
