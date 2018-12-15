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
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/algorithm/copy.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
//#include <range/v3/algorithm/max_element.hpp>
//#include <range/v3/algorithm/min_element.hpp>
//#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/view/all.hpp>
//#include <range/v3/view/for_each.hpp>
//#include <range/v3/view/any_view.hpp>
//#include <range/v3/view/cartesian_product.hpp>
//#include <range/v3/view/iota.hpp>
//#include <range/v3/view/concat.hpp>
//#include <range/v3/view/drop.hpp>
//#include <range/v3/view/join.hpp>
//#include <range/v3/view/filter.hpp>
//#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
//#include <range/v3/view/split.hpp>
//#include <range/v3/view/take.hpp>
//#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>

#include "oldDays.hpp"
#include <sstream>

namespace r=ranges;
namespace rv=r::view;

void day12stars() {
   std::ifstream fin(DIRECTORY+"day12");
   string _;
   string state;
   fin >> _ >> _ >> state;
   
   fin.ignore();
   fin.ignore();

   auto notes = r::getlines(fin) | r::to_vector;
//   r::sort(notes);
   auto updateNumByPot = [](int &n, char p) {
      n = (2*n+(p=='#'?1:0))%32;
   };
   
   auto fivePotsToNum = [updateNumByPot](std::string_view s) {
      int n=0;
      for(auto c:s)
         updateNumByPot(n,c);
      return n;
   };
   
   std::vector<char> rules(32);
   for(const auto &s : notes) {
      std::istringstream sin(s);
      std::string rule;
      char result;
      sin >> rule >> _ >> result;
      rules[fivePotsToNum(rule)]=result;
   }
   
   const int generations=20;
   std::string pots(2*generations+state.size()+2*generations+2,'.');
   r::copy(state,pots.begin()+2*generations);

   for(int g=0;g<generations;++g) {
      auto num=0;
      updateNumByPot(num,pots[0]);
      updateNumByPot(num,pots[1]);
      for(int p=2;p<pots.size();++p) {
         updateNumByPot(num,pots[p]);
         pots[p-2] = rules[num];
      }
      cout << pots << "\n";
   }
   
   auto sum=0;
   for(int p=0;p<pots.size();++p) {
      if (pots[p]=='#')
         sum += p-2*generations;
   }
   cout << "Day 12 star 1: " << sum << "\n";
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
//   day11stars();
   day12stars();
   return 0;
}
