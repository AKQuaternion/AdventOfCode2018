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

using std::get;
#include <utility>
#include <vector>
using std::cout;
using std::endl;
using std::string;

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
#include <range/v3/numeric/accumulate.hpp>
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
#include <range/v3/view/filter.hpp>
//#include <range/v3/view/for_each.hpp>
//#include <range/v3/view/iota.hpp>
//#include <range/v3/view/join.hpp>
//#include <range/v3/view/partial_sum.hpp>
//#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
//#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>
//#include <range/v3/view/zip_with.hpp>
//#include <range/v3/view_interface.hpp>

#include "oldDays.hpp"

namespace r=ranges;
namespace rv=r::view;

namespace day20 {
   using std::vector;
   using std::pair;
   using std::map;
   using coord=pair<int,int>;
   
   std::string chars;
   map<pair<int,int>,std::vector<pair<int,int>>> doors;
   
   void connect(coord &a, const coord &b) {
      doors[a].push_back(b);
//      doors[b].push_back(a);
      a = b;
   }
   
   const auto size=300ull;
   vector<string> m(size,string(size,' '));
                       
   char & at(coord s) {
      return m[s.second+size/2][s.first+size/2];
   }
   
//   operator+(coord a, coord b) {
//      return {a.first+b.first,a.second+b.second};
//   }
   
   void read(int &i, pair<int,int> s) {
      auto orig=s;
      
      auto & [x,y] = s;
      while(true) {
         switch(chars[i++]) {
            case 'N':
               connect(s,{x,y-2});
               break;
            case 'E':
               connect(s,{x+2,y});
               break;
            case 'S':
               connect(s,{x,y+2});
               break;
            case 'W':
               connect(s,{x-2,y});
               break;
            case '(':
               read(i,s);
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

   std::set<coord> visited;
   
   auto big = 0;
   auto longp = 0;
   void bfs(coord c,int len) {
      if (len>big)
         big=len;
      std::queue<pair<coord,int>> q;
      q.push({c,len});
      while (!q.empty()) {
         auto [n,len] = q.front();
         q.pop();
         if (visited.count(n))
            exit(1);//continue;
         if (len>=1000) ++longp;
         visited.insert(n);
         if (len>big)
            big=len;
         for(auto n2:doors[n])
            q.push({n2,len+1});
      }
   }
}


void day20stars() {
   using namespace day20;
   std::ifstream fin(DIRECTORY+"day20.txt");

   fin >> chars;
//   chars = "^WNE$";
//   chars = "^ENWWW(NEEE|SSE(EE|N))$";//|SSE(EE|N))$";
//   chars = "^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$";
//   for(auto c:chars)
//      cout << c << endl;
   assert(chars[0]=='^');
   int x=1;
   read(x,{0,0});


   for(auto [loc,v]:doors) {
      cout << loc.first << "," << loc.second << " ---> ";
      at(loc) = 'o';
      for(auto p:v) {
         at({(loc.first + p.first)/2,(loc.second+p.second)/2}) = '+';
         cout << "[" << p.first << "," << p.second << "] ";
      }
      cout << endl;
      cout << endl;
   }
   for(auto row:m)
      cout << row << endl;

   //   auto notes = r::getlines(fin) | r::to_vector;
   //   auto numbers = r::istream_range<char>(fin) | rv::to_vector;
   bfs({0,0},0);
   cout << "Day 20 star 1: " << big << endl;
   cout << "Day 20 star 2: " << longp << endl;
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
   day20stars();
//   day20b();
   return 0;
}
