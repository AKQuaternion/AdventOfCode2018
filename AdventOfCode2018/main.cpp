//
//  main.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/3/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <algorithm>
#include <iterator>
#include <vector>
using std::vector;
#include <range/v3/all.hpp>
namespace r=ranges;
namespace rv=r::view;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;

int main() {
   ifstream fin("/Users/hartman/Documents/Xcode projects/AdventOfCode2018/day1");
   cout << "Reading input." << endl;
   std::istream_iterator<string> cinBegin{fin};
   std::istream_iterator<string> cinEnd;
   vector<string> v;
   std::copy(cinBegin,cinEnd,std::back_inserter(v));
   auto numbers = rv::all(v) | rv::transform([](auto x){return std::stoi(x);});
   cout << r::accumulate(numbers,0) << endl;
   return 0;
}
