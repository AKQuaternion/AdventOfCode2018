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
#include <range/v3/algorithm/max.hpp>
#include <range/v3/algorithm/min.hpp>
//#include <range/v3/algorithm/max_element.hpp>
//#include <range/v3/algorithm/min_element.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
//#include <range/v3/algorithm/reverse.hpp>
//#include <range/v3/algorithm/sort.hpp>
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
#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>
//#include <range/v3/view/zip_with.hpp>
//#include <range/v3/view_interface.hpp>

#include "oldDays.hpp"

namespace r=ranges;
namespace rv=r::view;

namespace day17 {
using std::vector;

struct Vein {
   int minx, maxx, miny, maxy;
};

Vein lineToVein (const std::string &line) {
   std::istringstream sin(line);
   char single,_;
   int s1,d1,d2;
   sin >> single >> _ >> s1;
   sin.ignore(4);
   sin >> d1 >> _ >> _ >> d2;
   if (single == 'x')
      return {s1,s1,d1,d2};
   else
      return {d1,d2,s1,s1};
}

class Scan {
   struct Position {
      int x, y;
      Position below() {return {x,y+1};}
      Position above() {return {x,y-1};}
      Position left() {return {x-1,y};}
      Position right() {return {x+1,y};}
   };
   
public:
   template<typename Range>
   Scan(Range veins) {
      _minX = r::min(veins, std::less<>(), &Vein::minx).minx;
      _maxX = r::max(veins, std::less<>(), &Vein::maxx).maxx;
      _minY = r::min(veins, std::less<>(), &Vein::miny).miny;
      _maxY = r::max(veins, std::less<>(), &Vein::maxy).maxy;
      _data = vector<vector<char>>(_maxY-_minY+1,vector<char>(_maxX-_minX+1,'.'));
      for(const auto &v:veins)
         for(int y=v.miny;y<=v.maxy;++y)
            for(int x=v.minx;x<=v.maxx;++x)
               _data[y-_minY][x-_minX] = '#';

      dropFrom({6,0});
   }

   char & at(Position p) {
      return _data[p.y][p.x];
   }
   
   void print(Position p={0,0}) {
      auto c=at(p);
      at(p) = '*';
      for(auto &&row : _data) {
         for(auto c:row)
            cout << c;
         cout << "\n";
      }
      at(p) = c;
      cout << "\n";
   }
   
   // the following bool functions return true if water can escape
   
   bool dropFrom(Position p) {
      print(p);
      if (at(p.below()) == '.') {
         at(p) = '|';
         p = p.below();
         if(p.y > _maxY)
            return true;
      }
      fillFrom(p);
      return false;
   }
   
   void fillFrom(Position p) {
//      if (at(p) != '.')
//         return;
      at(p) = '|';
      print(p);
      if(!spreadLeft(p) && !spreadRight(p))
         fillFrom(p.above());
   }
   
   bool spreadRight(Position p) {
      print(p);
      p = p.right();
      while (at(p)=='.' && at(p.below()) != '.') {
         at(p) = '|';
         p = p.right();
      }
      if(at(p.below()) == '.') {
         at(p) = '|';
         dropFrom(p);
         return true;
      }
      return false;
   }
   

   bool spreadLeft(Position p) {
      print(p);
      p = p.left();
      while (at(p)=='.' && at(p.below()) != '.') {
         at(p) = '|';
         p = p.left();
      }
      if(at(p.below()) == '.') {
         at(p) = '|';
         dropFrom(p);
         return true;
      }
      return false;
   }

private:
   vector<vector<char>> _data;
   int _minX;
   int _maxX;
   int _minY;
   int _maxY;
};
   
}

void day17stars() {
   using namespace day17;
   
   std::ifstream fin(DIRECTORY+"day17test");
   
   auto veins = r::getlines(fin) | rv::transform(lineToVein) | r::to_vector;
   
   Scan s{veins};
   s.print();
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
   //   day16stars();
   day17stars();
   return 0;
}
