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





void day17stars() {
   struct Vein {
      int minx, maxx, miny, maxy;
      static Vein lineToVein (const std::string &line) {
         std::istringstream sin(line);
         char single,_;
         int s1,d1,d2;
         sin >> single >> _ >> s1;
         sin.ignore(4);
         sin >> d1 >> _ >> _ >> d2;
         return single=='x' ? Vein{s1,s1,d1,d2} : Vein{d1,d2,s1,s1};
      }
   };
   
   std::ifstream fin(DIRECTORY+"day17");
   auto veins = r::getlines(fin) | rv::transform(Vein::lineToVein) | r::to_vector;

   class Scan {
      struct Position {
         int x, y;
         Position below() {return {x,y+1};}
         Position left() {return {x-1,y};}
         Position right() {return {x+1,y};}
      };
      
   public:
      Scan(const std::vector<Vein>& veins) {
         auto _minX = r::min(veins, std::less<>(), &Vein::minx).minx-1;
         auto _maxX = r::max(veins, std::less<>(), &Vein::maxx).maxx+1;
         // may need to allow water flow outside of leftmost and rightmost veins
         auto _minY = r::min(veins, std::less<>(), &Vein::miny).miny;
         auto _maxY = r::max(veins, std::less<>(), &Vein::maxy).maxy;
         _data = std::vector<std::string>(_maxY-_minY+1,std::string(_maxX-_minX+1,'.'));
         for(const auto &v:veins)
            for(int y=v.miny;y<=v.maxy;++y)
               for(int x=v.minx;x<=v.maxx;++x)
                  _data[y-_minY][x-_minX] = '#';
         dropFrom({500-_minX,0});
         cout << "Day 17 Star 1: " << _damp << ", Star 2: " << _wet << "\n";
      }
      
      char & at(Position p) {
         return _data[p.y][p.x];
      }
      
      void print() {
         for(const auto &row : _data)
            cout << row << "\n";
      }
      
      // the following bool functions return true if water can escape
      
      bool dropFrom(Position p) {
         if (at(p) == '|')
            return true;
         if (at(p) != '.')
            return false;
         ++_damp; at(p) = '|';
         if(p.y == _data.size()-1)
            return true;
         if(dropFrom(p.below()))
            return true;
         return fillFrom(p);
      }
      
      bool fillFrom(Position p) {
         auto leftFlood = spread(p,&Position::left);
         auto rightFlood = spread(p,&Position::right);
         if (!leftFlood && !rightFlood) {
            for(auto t=p;at(t)=='|';t=t.left()) {
               _wet++; at(t)='~';
            }
            for(auto t=p.right();at(t)=='|';t=t.right()) {
               _wet++; at(t)='~';
            }
            return false;
         }
         return true;
      }
      
      // dir will be Position::left() or Position::right()
      bool spread(Position p, Position (Position::*dir)()) {
         p=(p.*dir)();
         if (at(p) == '|')
            return true;
         if (at(p) != '.')
            return false;
         ++_damp;
         at(p) = '|';
         if(dropFrom(p.below()))
            return true;
         return spread(p,dir);
      }
      
   private:
      std::vector<std::string> _data;
      size_t _wet=0;
      size_t _damp=0;
   };
   
   Scan s{veins};
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
