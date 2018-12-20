//
//  main.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/3/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <set>
#include <tuple>
#include <stack>

//using std::get;
#include <utility>
#include <vector>
using std::cout;
using std::endl;
//using std::string;

//#include <range/v3/action/transform.hpp>
//#include <range/v3/action/remove_if.hpp>
//#include <range/v3/algorithm/any_of.hpp>
//#include <range/v3/algorithm/find_if.hpp>
//#include <range/v3/algorithm/max.hpp>
//#include <range/v3/algorithm/min.hpp>
//#include <range/v3/algorithm/max_element.hpp>
//#include <range/v3/algorithm/min_element.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
//#include <range/v3/algorithm/reverse.hpp>
//#include <range/v3/algorithm/sort.hpp>
//#include <range/v3/getlines.hpp>
//#include <range/v3/istream_range.hpp>
//#include <range/v3/numeric/accumulate.hpp>
//#include <range/v3/range_concepts.hpp>
//#include <range/v3/range_traits.hpp>
//#include <range/v3/to_container.hpp>
//#include <range/v3/utility/concepts.hpp>
//#include <range/v3/utility/iterator_concepts.hpp>
//#include <range/v3/utility/iterator_traits.hpp>
//#include <range/v3/view/all.hpp>
//#include <range/v3/view/cartesian_product.hpp>
//#include <range/v3/view/concat.hpp>
//#include <range/v3/view/cycle.hpp>
//#include <range/v3/view/drop.hpp>
//#include <range/v3/view/filter.hpp>
//#include <range/v3/view/for_each.hpp>
//#include <range/v3/view/iota.hpp>
//#include <range/v3/view/join.hpp>
//#include <range/v3/view/partial_sum.hpp>
//#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
//#include <range/v3/view/take.hpp>
//#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>
//#include <range/v3/view/zip_with.hpp>
//#include <range/v3/view_interface.hpp>

#include "oldDays.hpp"

//namespace r=ranges;
//namespace rv=r::view;

namespace day20 {
   using std::vector;
   using std::pair;
   using std::map;
   using coord=pair<int,int>;
   
   map<pair<int,int>,vector<pair<int,int>>> doors;
   
   void connect(coord &a, const coord &b) {
      doors[a].push_back(b);
      doors[b].push_back(a);
      a = b;
   }
   
   void read(std::string_view chars, pair<int,int> s) {
      int i=0;
      auto orig=s;
      auto & [x,y] = s;
      while(true) {
         switch(chars[i++]) {
            case 'N':
               connect(s,{x,y-1});
               break;
            case 'E':
               connect(s,{x+1,y});
               break;
            case 'S':
               connect(s,{x,y+1});
               break;
            case 'W':
               connect(s,{x-1,y});
               break;
            case '(':
               read(chars.substr(i),s);
               break;
            case ')':
               return;
            case '|':
               s = orig;
               break;
            case '$':
               return;
         }
      }
   }

   void day20stars() {
      std::ifstream fin(DIRECTORY+"day20.txt");
      std::string chars;
      fin >> chars;
      read(chars.substr(1),{0,0});
      
      std::set<coord> visited;
      std::queue<pair<coord,int>> q;
      q.push({{0,0},0});
      auto longest = 0;
      auto numGT1000 = 0;
      while (!q.empty()) {
         auto [n,length] = q.front();
         q.pop();
         if (visited.count(n))
            continue;
         if (length>=1000) ++numGT1000;
         visited.insert(n);
         longest = std::max(length,longest);
         for(auto n2:doors[n])
            q.push({n2,length+1});
      }
      cout << "Day 20 star 1: " << longest << endl;
      cout << "Day 20 star 2: " << numGT1000 << endl;
   }
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
   //   day12stars();
   //   day13stars();
   //   day14stars();
   //   day15stars();
   //   day16stars();
   //   day17stars();
   //   day18stars();
   //   day19stars();
   day20::day20stars();
//   day20b();
   return 0;
}
