//
//  main.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/3/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::get;

#include <range/v3/action/sort.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/getlines.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/view_interface.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/cycle.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/partial_sum.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>
#include <range/v3/view/zip_with.hpp>

#include "oldDays.hpp"

namespace r=ranges;
namespace rv=r::view;

void day4star1() {
   //[1518-11-01 00:00] Guard #10 begins shift
   std::ifstream fin(DIRECTORY+"day4");
   auto records = r::getlines(fin) | r::to_vector;
   r::sort(records);
}

int main() {
//   day1star1();
//   day1star2();
//   day2star1();
//   day2star2();
//   day3stars();
   return 0;
}
