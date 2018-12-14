//
//  main.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/3/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <set>
#include <tuple>
using std::get;
#include <utility>
#include <vector>
using std::cout;
using std::endl;
using std::string;


#include <range/v3/getlines.hpp>
#include <range/v3/to_container.hpp>
//#include <range/v3/algorithm/sort.hpp>
//#include <range/v3/algorithm/count_if.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/istream_range.hpp>
//#include <range/v3/view/for_each.hpp>
//#include <range/v3/view/any_view.hpp>
#include <range/v3/view/cartesian_product.hpp>
//#include <range/v3/view/iota.hpp>
//#include <range/v3/view/concat.hpp>
//#include <range/v3/view/drop.hpp>
#include <range/v3/view/join.hpp>
//#include <range/v3/view/filter.hpp>
//#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
#include <range/v3/view/split.hpp>
//#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

#include "oldDays.hpp"
#include <sstream>

namespace r=ranges;
namespace rv=r::view;

void day11stars() {
//   Find the fuel cell's rack ID, which is its X coordinate plus 10.
//   Begin with a power level of the rack ID times the Y coordinate.
//   Increase the power level by the value of the grid serial number (your puzzle input).
//   Set the power level to itself multiplied by the rack ID.
//   Keep only the hundreds digit of the power level (so 12345 becomes 3; numbers with no hundreds digit become 0).
//   Subtract 5 from the power level.
   const int sn = 3214;
   std::vector<std::vector<int>> powers;
   const int ySize=300;
   const int xSize=300;
   
   for(int y=0;y<ySize;++y) {
      powers.push_back({});
      for(int x=0;x<xSize;++x) {
         auto xc=x+1;
         auto yc=y+1;
         //   Find the fuel cell's rack ID, which is its X coordinate plus 10.
         auto rackId = xc+10;
         //   Begin with a power level of the rack ID times the Y coordinate.
         auto pl = rackId*yc;
         //   Increase the power level by the value of the grid serial number (your puzzle input).
         pl += sn;
         //   Set the power level to itself multiplied by the rack ID.
         pl *= rackId;
         //   Keep only the hundreds digit of the power level (so 12345 becomes 3; numbers with no hundreds digit become 0).
         pl = pl/100%10;
         //   Subtract 5 from the power level.
         pl -= 5;
         powers.back().push_back(pl);
         assert(powers.size()==yc && powers.back().size()==xc);
      }
   }

   auto max=0;
   std::string output;
   for(int filterSize = 1; filterSize<=300; ++filterSize) {
      auto totalPower = [filterSize](const auto &p, int x, int y) {
         auto sum=0;
         for(int ii=0;ii<filterSize;++ii)
            for(int jj=0;jj<filterSize;++jj)
               sum += p[y+ii][x+jj];
         return sum;
      };
      
      std::vector<std::vector<int>> gridPowers(ySize-filterSize+1,std::vector<int>(xSize-filterSize+1));
      
      for(int y=0;y<=ySize-filterSize;++y)
         for(int x=0;x<=xSize-filterSize;++x)
            gridPowers[y][x]=totalPower(powers,x,y);
      
      auto coords = rv::cartesian_product(rv::ints(1,ySize-filterSize+2),rv::ints(1,xSize-filterSize+2));
      auto elements = rv::zip(coords, gridPowers | rv::join) | r::to_vector;
      auto maxPos = r::max_element(elements,[](auto &x, auto &y) {return get<1>(x) < get<1>(y);});
      if (maxPos->second > max) {
         max = maxPos->second;
         output = std::to_string(get<1>(maxPos->first)) + "," +
         std::to_string(get<0>(maxPos->first)) + "," + std::to_string(filterSize);
      }
      if (filterSize == 3)
         cout << "Day 11 star 1: " << output << "\n";
   }
   cout << "Day 11 star 2: " << output <<"\n";
}

int main() {
//   day1star1();
//   day1star2();
//   day2star1();
//   day2star2();
//   day3stars();
//   day4stars();
//   day5stars();
//   day6stars();
//   day7stars();
//   day8stars();
//   day9stars();
//   day10stars();
   day11stars();
   return 0;
}
