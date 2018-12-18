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

namespace day15 {
struct Point {//we blur distinction between point and vector
   int x,y;
   Point operator+(const Point &rhs) {return {x+rhs.x,y+rhs.y};}
   bool operator==(const Point &rhs) {return x==rhs.x && y==rhs.y;}
   std::string to_string() const {
      return "["+std::to_string(x)+","+std::to_string(y)+"]";
   }
   int dist(const Point& p) const {return abs(x-p.x)+abs(y-p.y);}
};

class Unit {
   friend class Cave;
   friend bool operator<(const Unit &lhs, const Unit &rhs) {
      return std::tie(lhs._loc.y,lhs._loc.x)<std::tie(rhs._loc.y,rhs._loc.x);
   }
public:
   Unit(int x, int y, char type) :_loc{x,y},_type(type)
   {}
   
   std::string to_string() const {
      return _hp<1 ? std::string{}
         : _type + ("@" + _loc.to_string() +"(" + std::to_string(_hp) + ")");
   }
   
private:
   char other() const {
      if(_type == 'E') return 'G';
      else if (_type == 'G') return 'E';
      throw 1;
   }
   
   bool isTargetOf(char t, Point loc) const {
      return _hp>0 && _loc.dist(loc)==1 && t == other();
   }
   
   Point _loc;
   char _type;
   int _hp=200;
};

class Cave {
public:
   Cave(const std::vector<std::string> &cave):_cave(cave) {
      for(int y=0;y<cave.size();++y)
         for(int x=0;x<cave[y].size();++x)
            if (auto s = cave[y][x]; s == 'E' || s == 'G')
               _units.emplace_back(x,y,s);
      print();
   }
   
   void print() const {
      for(const auto &l:_cave)
         cout << l << endl;
      
      for(auto &unit : _units)
         cout << unit.to_string() << " ";
      cout << endl;
   }
   
   bool doRound() {
      std::sort(begin(_units),end(_units));
      for(Unit &me:_units) {
         if (me._hp < 1)
            continue;
         if(!hasOpponents(me._type))
            return true;
         doMovement(me);
         doAttack(me);
      }

      std::sort(begin(_units),end(_units));
      return false;
   }
   int score() {
      return r::accumulate(_units,0,[](int i, const Unit &u){return i+(u._hp>0?u._hp:0);});
   }
private:
   
   void doMovement(Unit& me) {
      if (hasTargets(me._type,me._loc))
         return;
      
      std::vector<Point> dirs{{0,-1},{-1,0},{1,0},{0,1}};
      
      struct Info {
         Point firstMove,pos;
      };
      
      std::queue<Info> info;
      
      std::set<std::pair<int,int>> visited;
      for(const auto &d:dirs) {
         auto pos=me._loc+d;
         if(at(pos) == '.') {
            info.push(Info{d,pos});
            visited.insert({pos.x,pos.y});
         }
      }
      
      bool show=false;
      while(!info.empty()) {
         if(show) {
            auto i=info;
            while(!i.empty()) {
               auto x = i.front();
               i.pop();
               cout << x.firstMove.to_string() << " "
               << x.pos.to_string() << "\n";
            }
            cout << "-----------------------------------\n";
         }
         auto spot = info.front();
         info.pop();
         if (hasTargets(me._type,spot.pos)) {
            assert(at(me._loc)==me._type);
            assert(me._hp > 0);
            at(me._loc) = '.';
            me._loc = me._loc + spot.firstMove;
            at(me._loc) = me._type;
            return;
         }
         for(const auto &d:dirs) {
            Point next = spot.pos + d;
            if (!visited.count({next.x,next.y}) && at(next) == '.') {
               info.push(Info{spot.firstMove,next});
               visited.insert({next.x,next.y});
            }
         }
      }
   }
   
   void doAttack(Unit const &me) {
      auto targets = _units
      | rv::transform([](Unit &u){return std::ref(u);})
      | rv::filter([&me](const Unit &u){return u.isTargetOf(me._type, me._loc);})
      | r::to_vector;
      
      if(!targets.empty()) {
         r::sort(targets,std::less<>(),&Unit::_hp);
         auto &target = targets.front().get();
         assert(me._loc.dist(target._loc)==1);
         assert(me.other() == target._type);
         assert(target._hp >0);
         target._hp -= 3;
         if (target._hp <= 0)
            at(target._loc) = '.';
      }
   }
   
   bool hasTargets(char type, Point loc) {
      auto targets = _units
         | rv::filter([type,loc](const Unit &u){
               return u.isTargetOf(type,loc);});
      return !targets.empty();
   }
   bool hasOpponents(char type) {
      auto targets = _units
         | rv::filter([type](const Unit &u){return u._hp>0 && type==u.other();});
      return !targets.empty();
   }
   char & at(Point loc)  {return _cave[loc.y][loc.x];}
   std::vector<std::string> _cave;
   std::vector<Unit> _units;
};
}

void day15stars() {
   using namespace day15;
   Unit u{1,5,'G'};
   std::ifstream fin(DIRECTORY+"day15");
   std::vector<std::string> cave = r::getlines(fin);
 
   Cave cavern(cave);
   int i=0;
   while(!cavern.doRound()) {
      cout << "After round " << ++i << "\n";
      cavern.print();
   }

   cout << "After\n";
   cavern.print();

   cout << "Day 15 star 1: " << cavern.score() << "*" << i << " = " << cavern.score()*i << "\n";
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
   day15stars();
//   day16stars();
//   day17stars();
//   day18stars();
   return 0;
}
