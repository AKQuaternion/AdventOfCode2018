//
//  main.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/3/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;

#include <range/v3/all.hpp>
namespace r=ranges;
namespace rv=r::view;

//*********
//https://blog.tartanllama.xyz/passing-overload-sets/
#define FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define LIFT(X) [](auto &&... args) \
noexcept(noexcept(X(FWD(args)...)))  \
-> decltype(X(FWD(args)...)) \
{  \
return X(FWD(args)...); \
}
//*********

int main() {
   std::ifstream fin("/Users/hartman/Documents/Xcode projects/AdventOfCode2018/day1");
   cout << "Reading input." << endl;
   
   auto numbers = r::getlines(fin) | rv::transform(LIFT(std::stoi));
   cout << r::accumulate(numbers,0) << endl;
   return 0;
}
