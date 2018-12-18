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
using std::get;
#include <utility>
#include <vector>
using std::cout;
using std::endl;
using std::string;

//#include <range/v3/action/transform.hpp>
//#include <range/v3/action/remove_if.hpp>
//#include <range/v3/algorithm/count_if.hpp>
//#include <range/v3/algorithm/find_if.hpp>
//#include <range/v3/algorithm/max.hpp>
//#include <range/v3/algorithm/min.hpp>
//#include <range/v3/algorithm/max_element.hpp>
//#include <range/v3/algorithm/min_element.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
//#include <range/v3/algorithm/reverse.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/getlines.hpp>
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

namespace r=ranges;
namespace rv=r::view;

//namespace day15 {
//   class Unit {
//      friend bool operator<(const Unit &lhs, const Unit &rhs) {
//         return std::tie(lhs._y,lhs._x)<std::tie(rhs._y,rhs._x);
//      }
//   public:
//      Unit(int x, int y, char type) :_x(x),_y(y),_type(type)
//      {}
//
//      std::string to_string() const {
//         return _type
//            + ("@[" + std::to_string(_x) + "," +std::to_string(_y) + "]("
//            + std::to_string(_hp) + ")");
//      }
//
//      char other() const {
//         if(_type == 'E') return 'G';
//         else if (_type == 'G') return 'E';
//         throw 1;
//      }
//
//   private:
//      int _x,_y;
//      char _type;
//      int _hp=200;
//   };
//
//}
//
//void day15stars() {
//   using namespace day15;
//   Unit u{1,5,'G'};
//   std::ifstream fin(DIRECTORY+"day15test1");
//   std::vector<std::string> cave = r::getlines(fin);
//
//   for(const auto &l:cave)
//      cout << l << endl;
//
//   std::vector<Unit> units;
//   for(int y=0;y<cave.size();++y)
//      for(int x=0;x<cave[y].size();++x)
//         if (auto s = cave[y][x]; s == 'E' || s == 'G')
//            units.emplace_back(x,y,s);
//
//   while(true) {
//      std::sort(begin(units),end(units));
//      for(auto &unit : units) {
//         cout << unit.to_string() << " ";
//         if(cave[_y-1][_x] == other())
//         cout << endl;
//      }
//
//
//      break;
//    }
//}

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
   day18stars();
   return 0;
}
