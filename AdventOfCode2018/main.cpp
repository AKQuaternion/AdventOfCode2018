//
//  main.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/3/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <iomanip>
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
//#include <range/v3/algorithm/accumulate.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/count.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/all_of.hpp>
//#include <range/v3/algorithm/any_of.hpp>
//#include <range/v3/algorithm/find_if.hpp>
//#include <range/v3/algorithm/max.hpp>
//#include <range/v3/algorithm/min.hpp>
#include <range/v3/algorithm/minmax.hpp>
#include <range/v3/algorithm/max_element.hpp>
//#include <range/v3/algorithm/min_element.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
//#include <range/v3/algorithm/reverse.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/getlines.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
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
#include <range/v3/view/indices.hpp>
//#include <range/v3/view/join.hpp>
//#include <range/v3/view/partial_sum.hpp>
#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>
//#include <range/v3/view/zip_with.hpp>
//#include <range/v3/view_interface.hpp>

#include "oldDays.hpp"

namespace r=ranges;
namespace rv=r::view;

namespace day23 {
   using std::string;
   using Int = long long;
   
   class BoundingBox;
   
   struct Nanobot {
      Int x,y,z,r;
      Int dist(const Nanobot &r) const {
         return (abs(x-r.x)+abs(y-r.y)+abs(z-r.z));
      }
      friend std::ostream& operator<<(std::ostream& os, const Nanobot &n) {
         return os << "(" << n.x << "," << n.y << "," << n.z << ")" << n.r;
      }
//      bool intersects (const Nanobot & other) const {
//         return dist(other) <= r+other.r;
//      }
      bool contains (Int px, Int py, Int pz) const {
         return (abs(x-px)+abs(y-py)+abs(z-pz)<=r);
      }
      bool contains (const BoundingBox &b) const;
      bool intersects (const BoundingBox &b) const;
//      std::vector<Nanobot> vertices() {
//         std::vector<Nanobot> ret;
//         ret.push_back({x-r,y,z,0});
//         ret.push_back({x+r,y,z,0});
//         ret.push_back({x,y-r,z,0});
//         ret.push_back({x,y+r,z,0});
//         ret.push_back({x,y,z-r,0});
//         ret.push_back({x,y,z+r,0});
//         return ret;
//      }
   };
   
   struct BoundingBox {
      Int sx,sy,sz,bx,by,bz;
      bool intersects(const BoundingBox r) const {
         return sx<=r.bx && r.sx<=bx && sy<=r.by && r.sy<=by && sz<=r.bz && r.sz<=bz;
      }
//      BoundingBox(const Nanobot &n):
//      sx(n.x-n.r),bx(n.x+n.r),
//      sy(n.y-n.r),by(n.y+n.r),
//      sz(n.z-n.r),bz(n.z+n.r)
//      {}
      BoundingBox() = default;
      BoundingBox(Int sx,Int sy,Int sz, Int bx, Int by, Int bz):
         sx(sx),sy(sy),sz(sz),bx(bx),by(by),bz(bz) {}
      
      Int xSize() const { return bx-sx+1;}
      Int ySize() const { return by-sy+1;}
      Int zSize() const { return bz-sz+1;}

      bool contains(Int px, Int py, Int pz) const {
         return sx<px && px<bx && sy<py && py<by && sz<pz && pz<bz;
      }
      
      friend std::ostream& operator<<(std::ostream &os, const BoundingBox &b) {
         return os << "x(" << b.sx << "," << b.bx << ") y(" << b.sy << "," << b.by << ") z(" << b.sz << "," << b.bz << ") [" <<
         b.xSize() << "," << b.ySize() << "," << b.zSize() << "]";
      }
   };
   
   bool Nanobot::intersects (const BoundingBox &b) const {
//      return contains(b.sx,b.sy,b.sz) ||
//      contains(b.sx,b.sy,b.bz) ||
//      contains(b.sx,b.by,b.sz) ||
//      contains(b.sx,b.by,b.bz) ||
//      contains(b.bx,b.sy,b.sz) ||
//      contains(b.bx,b.sy,b.bz) ||
//      contains(b.bx,b.by,b.sz) ||
//      contains(b.bx,b.by,b.bz) ||
//      b.contains(x-r,y  ,z  ) ||
//      b.contains(x+r,y  ,z  ) ||
//      b.contains(x  ,y-r,z  ) ||
//      b.contains(x  ,y+r,z  ) ||
//      b.contains(x  ,y  ,z-r) ||
//      b.contains(x  ,y  ,z+r);
      Int cx, cy, cz;
      if(x < b.sx)
         cx = b.sx;
      else if (b.bx < x)
         cx = b.bx;
      else
         cx = x;
 
      if(y < b.sy)
         cy = b.sy;
      else if (b.by < y)
         cy = b.by;
      else
         cy = y;
 
      if(z < b.sz)
         cz = b.sz;
      else if (b.bz < z)
         cz = b.bz;
      else
         cz = z;
      
      return contains(cx,cy,cz);
   }


#define TRANSFORM(x, y) rv::transform([](auto &&x) { return y; })
   
   std::vector<Nanobot> nanobots;
   
   //size_t intersects = 0;
   //int lasti=1000;
   //void most(int i, const BoundingBox &bbox, std::vector<int> & which) {
   //   if(i%4==0 && i<300) cout << i << " " << intersects << "\n";
   //   if(i==nanobots.size()) {
   ////      cout << intersects << endl;
   //      intersects = std::max(intersects,which.size());
   //      return;
   //   }
   //   most(i+1,bbox,which);
   //   if(bbox.intersects(nanobots[i])) {
   //      if(i<lasti) cout << "Trying with " << (lasti=i) << endl;
   //      BoundingBox temp = intersect(bbox,nanobots[i]);
   //      which.push_back(i);
   //      most(i+1,temp,which);
   //      which.pop_back();
   //   }
   //}
   using std::pair;
   using std::vector;
   Int canSee=0;
   Int closestDist = 0;
  
   using Qvalue = std::tuple<int,BoundingBox>;
   auto compare = [](const Qvalue &l, const Qvalue &r) { return std::get<0>(l) < std::get<0>(r);};
   std::priority_queue<Qvalue,vector<Qvalue>,decltype(compare)> q(compare);
   
   template <typename T> int sgn(T val) {
      return (T(0) < val) - (val < T(0));
   }
   
   void split() {
      while (!q.empty()) {
         auto [num,b] = q.top();
         q.pop();
         if (num<=canSee)
            continue;
         if(b.xSize()<=1 && b.ySize()<= 1 && b.zSize() <= 1) {
//            cout << b.xSize() << " " << b.ySize() << " " << b.zSize() << " " << b.xSize()/50*b.ySize()/50*b.zSize()/50 << "\n";
            for(Int x=b.sx;x<=b.bx;++x)
               for(Int y=b.sy; y<=b.by; ++y)
                  for(Int z = b.sz; z<= b.bz; ++z) {
                     Nanobot me{x,y,z,0};
                     auto hereCanSee = r::count_if(nanobots,[me](const auto &n){return n.dist(me) <= n.r;});
                     if(hereCanSee == canSee && me.dist({0,0,0,0}) < closestDist)
                        closestDist = me.dist({0,0,0,0});
                     else if(hereCanSee > canSee) {
                        canSee = hereCanSee;
                        closestDist = me.dist({0,0,0,0});
                        cout << me << endl;
                     }
                  }
            continue;
         }
         if(b.xSize()>50 && b.ySize()>50 && b.zSize()>50)
            cout << q.size() << " " << b << " can see:" << canSee << " dist: " << closestDist << endl;
         if (b.xSize() > b.ySize() && b.xSize() > b.zSize()) {//split on x
            Int mx = b.sx + (b.bx-b.sx)/2;

            int left=0;
            int right=0;
            BoundingBox lBox{b};
            lBox.bx = mx;
            BoundingBox rBox{b};
            rBox.sx = mx+1;
            for(const auto &n:nanobots) {
               if(n.intersects(lBox))
                  ++left;
               if(n.intersects(rBox))
                  ++right;
            }
            if(left>canSee) q.push(std::make_tuple(left,lBox));
            if(right>canSee) q.push(std::make_tuple(right,rBox));
         }
         else if (b.ySize() > b.zSize()) {//split on y
            Int my = b.sy + (b.by-b.sy)/2;

            int left=0;
            int right=0;
            BoundingBox lBox{b};
            lBox.by = my;
            BoundingBox rBox{b};
            rBox.sy = my+1;
            for(const auto &n:nanobots) {
               if(n.intersects(lBox))
                  ++left;
               if(n.intersects(rBox))
                  ++right;
            }
            if(left>canSee) q.push(std::make_tuple(left,lBox));
            if(right>canSee) q.push(std::make_tuple(right,rBox));
         }
         else {//split on z
            Int mz = b.sz + (b.bz-b.sz)/2;

            int left=0;
            int right=0;
            BoundingBox lBox{b};
            lBox.bz = mz;
            BoundingBox rBox{b};
            rBox.sz = mz+1;
            for(const auto &n:nanobots) {
               if(n.intersects(lBox))
                  ++left;
               if(n.intersects(rBox))
                  ++right;
            }
            if(left>canSee) q.push(std::make_tuple(left,lBox));
            if(right>canSee) q.push(std::make_tuple(right,rBox));
         }
      }
   }
}//namespace day23

void day23stars() {
   using namespace day23;
   
   auto strToNanoBot = [](const string &s) {
      std::istringstream sin(s);
      char _;
      int x,y,z,r;
      //      pos=<63050011,17169307,53101295>, r=61836378
      //      pos=<19230077,48430355,4672540>, r=65947720
      sin >>_>>_>>_>>_>>_ >> x >>_ >> y >>_ >> z >>_>>_>>_>>_ >> r;
      return Nanobot{x,y,z,r};
   };
   
   std::ifstream fin(DIRECTORY + "day23");
   nanobots = r::getlines(fin) | rv::transform(strToNanoBot) | r::to_vector;
   r::sort(nanobots,r::less{},&Nanobot::r);
   
   const auto strongest = *r::max_element(nanobots, std::less<>{}, &Nanobot::r);
   const auto star1 = r::count_if(nanobots,[strongest](auto &&n){return strongest.dist(n) <= strongest.r;});
   cout << "Day 23 star 1: " << star1 << "\n";
   
#define TRANSFORM(x, y) rv::transform([](auto &&x) { return y; })
   auto [minX,maxX] = r::minmax(nanobots | TRANSFORM(l,l.x));
   auto [minY,maxY] = r::minmax(nanobots | TRANSFORM(l,l.y));
   auto [minZ,maxZ] = r::minmax(nanobots | TRANSFORM(l,l.z));
   
   cout << "minX = " << minX << " maxX = " << maxX << "\n";
   cout <<(maxX-minX)<<" "<<(maxY-minY)<<" "<<(maxZ-minZ)<<"\n";
   unsigned long long sum=0;
   for(auto &&n:nanobots)
      sum+= n.r*n.r*n.r;
   cout << sum << " = sum of radii cubed\n";
   
   int count=0;
   for(auto &n:nanobots) {
      Nanobot me{30476708,15032337,25975595,0};
      if(n.dist(me) <= n.r)
         ++count;
   }
   
   cout << count << "\n";
   
   q.push(std::make_tuple(nanobots.size(),BoundingBox{minX,minY,minZ,maxX,maxY,maxZ}));
   split();
   cout << canSee << " <-see closestDist-> " << closestDist << "\n";
   
   //68945139 is too low
   //79582523 still too high

   cout << closestDist << "\n";
   
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
   //   day20stars();
   //   day21stars();
   //   day22stars();
   day23stars();
   //   day24stars();
   return 0;
}
