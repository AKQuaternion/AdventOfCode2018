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

#include <range/v3/getlines.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/action/remove_if.hpp>
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

namespace day13 {
   std::vector<std::string> tracks;
   std::vector<std::string> cartMap;
   bool firstCollision = true;
   
   class Cart {
      friend bool operator<(const Cart &lhs, const Cart &rhs) {
         return std::tie(lhs._y,lhs._x) < std::tie(rhs._y,rhs._x);
      }

   public:
      Cart(int x,int y,int direction):_x(x),_y(y),_direction(direction)
      {}
      bool gone() const {
         return _gone;
      }
      std::string positionStr() const {
         return std::to_string(_x) + "," + std::to_string(_y);
      }
      void goneIfCrashed(const Cart &c) {
         if(_x==c._x && _y == c._y) {
            cartMap[_y][_x] = ' ';
            _gone = true;
         }
      }
      bool update() {
         if (_gone)
            return false;
         cartMap[_y][_x] = ' ';
         _x += directions[_direction].first;
         _y += directions[_direction].second;
         if (cartMap[_y][_x] != ' ') {
            if (firstCollision) {
               cout << "Day 13 star 1: " << _x << "," << _y << "\n";
               firstCollision = false;
            }
            return true;
         }
         switch (tracks[_y][_x]) {
            case '/':
               _direction ^= 1; ////swap U<->R and D<->L
               break;
            case '\\':
               _direction = 3-_direction; //swap U<->L and D<->R
               break;
            case '+':
               _direction = (_direction+turnChoices[_turnStatus])%4;
               _turnStatus = (_turnStatus+1)%3;
               break;
            case ' ':
               break;
            default:
               throw std::runtime_error("Cart::update() moved cart onto unknown track character");
         }
         cartMap[_y][_x] = symbols[_direction];
         return false;
      }
   private:
      int _x, _y;
      int _direction;
      int _turnStatus=0;
      bool _gone=false;
      static constexpr std::pair<int,int> directions[4] = {{0,-1},{1,0},{0,1},{-1,0}};//URDL
      static constexpr int turnChoices[3]={3,0,1};//Left, straight, right
      static constexpr char symbols[4] = {'^','>','v','<'};
   };
}

void day13stars() {
   using namespace day13;
   std::ifstream fin(DIRECTORY+"day13");
   tracks = r::getlines(fin) | r::to_vector;
   
   std::vector<Cart> carts;
   for(int y=0;y<tracks.size();++y) {
      cartMap.push_back({});
      for(int x=0;x<tracks[y].size();++x) {
         switch (tracks[y][x]) {
            case '^':
               carts.push_back({x,y,0});
               cartMap.back().push_back('^');
               tracks[y][x]=' ';
               break;
            case '>':
               carts.push_back({x,y,1});
               cartMap.back().push_back('>');
               tracks[y][x]=' ';
               break;
            case 'v':
               carts.push_back({x,y,2});
               cartMap.back().push_back('v');
               tracks[y][x]=' ';
               break;
            case '<':
               carts.push_back({x,y,3});
               cartMap.back().push_back('<');
               tracks[y][x]=' ';
               break;
            case '/':
            case '\\':
            case '+':
            case ' ':
               cartMap.back().push_back(' ');
               break;
            case '-':
            case '|':
               cartMap.back().push_back(' ');
               tracks[y][x]=' ';
               break;
            default:
               throw std::runtime_error("day13stars: unknown symbol in track map ["+std::string(1,tracks[y][x])+"]");
         }
      }
   }
   while(true) {
      std::sort(carts.begin(),carts.end());
      for(auto &c:carts)
         if(c.update())
            for(auto &c2:carts)
               c2.goneIfCrashed(c);
//      cout << carts.size() << " ";
      carts |= r::action::remove_if([](auto &&c){return c.gone();});
//      cout << carts.size() << "\n";
      if (carts.size() == 1) {
         cout << "Day 13 star 2: " << carts.front().positionStr() << "\n";
         return;
      }
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
   day13stars();
//   day14stars();
   return 0;
}
