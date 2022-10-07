# IRS
This repo is used to bootstrap term structure based on following market inputs
1. Anually Interest Rate Swap indexed on LIBOR 3M, e.g. 1y, 2y, ... 50y.
2. 3M spot rate.

Assumptions made:
1. The Interest Rate Swap is assumed to be simply-compounded.
2. Linear interpolation is used on spot rate, and swap rate when needed.

To run this code:
  1. This code is written using visual studio 2019.
  2. main.cpp is the access point to run.
  
  Note: 
  There are 4 test cases already there which tests the following:
  
    Test Case 1
    Test term structures can reprice all libor swaps to par.
    Projection and Discount curve are both LIBOR curve
    
    Test Case 2
    Test term structures can reprice all libor swaps to par.
    Projection curve using LIBOR CURVE and Discount curve using OIS Curve
    
    Test Case 3
    Test DV01 and Convexity
    
    Test Case 4
    Test Term Structure is working properly, including properly returning zero rate, forward rate, par rate and discount factor
