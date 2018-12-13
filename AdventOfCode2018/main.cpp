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

#include <range/v3/action/transform.hpp>
#include <range/v3/getlines.hpp>
#include <range/v3/to_container.hpp>
//#include <range/v3/algorithm/sort.hpp>
//#include <range/v3/algorithm/count_if.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/istream_range.hpp>
//#include <range/v3/view/for_each.hpp>
#include <range/v3/numeric/accumulate.hpp>
//#include <range/v3/view/any_view.hpp>
//#include <range/v3/view/cartesian_product.hpp>
//#include <range/v3/view/iota.hpp>
//#include <range/v3/view/concat.hpp>
//#include <range/v3/view/drop.hpp>
//#include <range/v3/view/filter.hpp>
//#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
#include <range/v3/view/split.hpp>
//#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>

#include "oldDays.hpp"
#include <sstream>

namespace r=ranges;
namespace rv=r::view;

void day10stars() {
   struct Point {
      int x, y, dx, dy; };
   
   auto stringToPoint = [](const string &s) {
//position=<-20655, -20679> velocity=< 2,  2>
      std::istringstream sin(s);
      Point p;
      char _;
      sin.ignore(10);
      sin >> p.x >> _ >> p.y;
      sin.ignore(12);
      sin >> p.dx >> _ >> p.dy;
      return p;
   };

   std::ifstream fin(DIRECTORY+"day10");
   auto points = r::getlines(fin) | rv::transform(stringToPoint) | r::to_vector;
   auto prevMinY = r::min_element(points, std::less<>() ,&Point::y)->y;
   auto count =0;
   while(true) {
      auto nextGen{points};
      nextGen |= r::action::transform([](Point &p){p.x += p.dx; p.y += p.dy; return p;});
      auto newMinY = r::min_element(nextGen, std::less<>() ,&Point::y)->y;

      if (prevMinY > newMinY) {
         auto minX = r::min_element(points, std::less<>(), &Point::x)->x;
         auto maxX = r::max_element(points, std::less<>(), &Point::x)->x;
         auto minY = r::min_element(points, std::less<>(), &Point::y)->y;
         auto maxY = r::max_element(points, std::less<>(), &Point::y)->y;
         cout << count << ": " << minX << "," << maxX << "   " << minY << "," << maxY << "\n";
         auto plotPoints = [minX,minY](auto & plot, auto c) -> auto & {
            plot[c.y-minY][c.x-minX] = '*';
            return plot;
         };
         std::vector<std::vector<char>> plot(maxY-minY+1,std::vector<char>(maxX-minX+1,'.'));
         plot = r::accumulate(points,plot,plotPoints);
         for(const auto &row:plot) {
            for(auto c:row)
               cout << c;
            cout << endl;
         }
         break;
      }
      prevMinY = newMinY;
      points.swap(nextGen);
      ++count;
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
   day10stars();
   return 0;
}
