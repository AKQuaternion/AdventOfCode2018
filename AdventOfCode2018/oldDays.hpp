//
//  oldDays.hpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/5/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#ifndef oldDays_hpp
#define oldDays_hpp

#include <string>

extern const std::string DIRECTORY;

void day1star1();
void day1star2();
void day2star1();
void day2star2();
void day3stars();
void day4stars();
void day5stars();
void day6stars();
void day7stars();
void day8stars();
void day9stars();
void day10stars();
void day11stars();
void day12stars();
void day13stars();
void day14stars();
void day15stars();
void day16stars();
void day17stars();
void day18stars();
void day19stars();
void day20stars();
void day21stars();
void day22stars();
void day23stars();
void day24stars();
void day25stars();

struct Point {//we blur distinction between point and vector
   int x,y;

   friend int dist(const Point &p1, const Point &p2){return p1.dist(p2);}
   Point & operator+=(const Point &rhs) {x += rhs.x; y += rhs.y; return *this;}
   Point operator+(const Point &rhs) const {return {x+rhs.x,y+rhs.y};}
   bool operator==(const Point &rhs) const {return x==rhs.x && y==rhs.y;}
   bool operator<(const Point &rhs) const {return x==rhs.x?y<rhs.y:x<rhs.x;}
   std::string to_string() const {
      return "["+std::to_string(x)+","+std::to_string(y)+"]";
   }
   int dist(const Point& p) const {return abs(x-p.x)+abs(y-p.y);}
   friend std::ostream & operator<<(std::ostream &os, const Point &p) {
      return os << p.to_string();
   }
};


#endif /* oldDays_hpp */
