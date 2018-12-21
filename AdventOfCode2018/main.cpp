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
#include <unordered_set>
#include <tuple>
#include <stack>
#include <valarray>

//using std::get;
#include <utility>
#include <vector>
using std::cout;
using std::endl;
//using std::string;

#include <range/v3/action/transform.hpp>
#include <range/v3/action/remove_if.hpp>
//#include <range/v3/algorithm/any_of.hpp>
//#include <range/v3/algorithm/find_if.hpp>
//#include <range/v3/algorithm/max.hpp>
//#include <range/v3/algorithm/min.hpp>
//#include <range/v3/algorithm/max_element.hpp>
//#include <range/v3/algorithm/min_element.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
//#include <range/v3/algorithm/reverse.hpp>
//#include <range/v3/algorithm/sort.hpp>
#include <range/v3/getlines.hpp>
#include <range/v3/istream_range.hpp>
//#include <range/v3/numeric/accumulate.hpp>
//#include <range/v3/range_concepts.hpp>
//#include <range/v3/range_traits.hpp>
#include <range/v3/to_container.hpp>
//#include <range/v3/utility/concepts.hpp>
//#include <range/v3/utility/iterator_concepts.hpp>
//#include <range/v3/utility/iterator_traits.hpp>
//#include <range/v3/view/all.hpp>
//#include <range/v3/view/cartesian_product.hpp>
//#include <range/v3/view/concat.hpp>
//#include <range/v3/view/cycle.hpp>
//#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
//#include <range/v3/view/for_each.hpp>
//#include <range/v3/view/iota.hpp>
//#include <range/v3/view/join.hpp>
//#include <range/v3/view/partial_sum.hpp>
#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
#include <range/v3/view/take.hpp>
//#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>
//#include <range/v3/view/zip_with.hpp>
//#include <range/v3/view_interface.hpp>

#include "oldDays.hpp"

namespace r=ranges;
namespace rv=r::view;

void day20stars() {
   using Pos=Point;
   std::ifstream fin(DIRECTORY+"day20.txt");
   std::string chars;
   fin >> chars;
   //   chars = "^(E|SSEENNW)S$";
   int i=1; //skip ^
   Pos curPos{0,0};
   std::stack<Pos> v;
   v.push(curPos);
   std::map<char,Pos> dirs{{'N',{0,-1}},{'E',{1,0}},{'S',{0,1}},{'W',{-1,0}}};
   std::map<Pos,std::set<Pos>> doors;
   
   for(char c=chars[i]; c != '$'; c=chars[++i]) { //read the regex
      if (c=='(')
         v.push(curPos);
      else if (c==')') {
         curPos=v.top();
         v.pop();
      } else if (c=='|')
         curPos = v.top();
      else {
         auto nextPos = curPos+dirs[c];
         if(doors[curPos].count(nextPos)) {
            cout << "Seen before " << curPos << " -> " << nextPos << "\n";
         }
         doors[curPos].insert(nextPos);
         //         doors[nextPos].push_back(curPos);
         curPos = nextPos;
      }
   }
   
   std::set<Pos> visited;
   std::queue<std::pair<Pos,int>> q;
   q.push({{0,0},0});
   auto longest = 0;
   auto numGT1000 = 0;
   while (!q.empty()) { // do a bfs
      auto [pos,length] = q.front();
      q.pop();
      if (visited.count(pos))
         continue;
      visited.insert(pos);
      numGT1000 += (length>=1000);
      longest = std::max(length,longest);
      for(auto neighborPos:doors[pos])
         q.push({neighborPos,length+1});
   }
   cout << "Day 20 star 1: " << longest
   << "\nDay 20 star 2: " << numGT1000 << "\n";
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
   day10stars();
//   day11stars();
//   day12stars();
//   day13stars();
//   day14stars();
//   day15stars();
//   day16stars();
//   day17stars();
//   day18stars();
//   day19stars();
   day20stars();
//   day21stars();
   return 0;
}
