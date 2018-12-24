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

struct Nanobot {
   Int x,y,z,r;
   Int dist(const Nanobot &r) const {
      return (abs(x-r.x)+abs(y-r.y)+abs(z-r.z));
   }
   friend std::ostream& operator<<(std::ostream& os, const Nanobot &n) {
      return os << "(" << n.x << "," << n.y << "," << n.z << ")" << n.r;
   }
   bool intersects (const Nanobot & other) const {
      return dist(other) <= r+other.r;
   }
   std::vector<Nanobot> vertices() {
      std::vector<Nanobot> ret;
      ret.push_back({x-r,y,z,0});
      ret.push_back({x+r,y,z,0});
      ret.push_back({x,y-r,z,0});
      ret.push_back({x,y+r,z,0});
      ret.push_back({x,y,z-r,0});
      ret.push_back({x,y,z+r,0});
      return ret;
   }
};

struct BoundingBox {
   Int sx,sy,sz,bx,by,bz;
   bool intersects(const BoundingBox r) const {
      return sx<r.bx && r.sx<bx && sy<r.by && r.sy<by && sz<r.bz && r.sz<bz;
   }
   BoundingBox(const Nanobot &n):
   sx(n.x-n.r),bx(n.x+n.r),
   sy(n.y-n.r),by(n.y+n.r),
   sz(n.z-n.r),bz(n.z+n.r)
   {}
   BoundingBox() = default;
   BoundingBox(Int sx,Int sy,Int sz, Int bx, Int by, Int bz):
   sx(sx),sy(sy),sz(sz),bx(bx),by(by),bz(bz) {}
   
   friend BoundingBox intersect(const BoundingBox &b1, const BoundingBox &b2) {
      BoundingBox res;
      res.sx = std::max(b1.sx,b2.sx);
      res.sy = std::max(b1.sy,b2.sy);
      res.sz = std::max(b1.sz,b2.sz);
      res.bx = std::min(b1.bx,b2.bx);
      res.by = std::min(b1.by,b2.by);
      res.bx = std::min(b1.bx,b2.bz);
      return res;
   }
   friend std::ostream& operator<<(std::ostream &os, const BoundingBox &b) {
      return os << "x(" << b.sx << "," << b.bx << ") y(" << b.sy << "," << b.by << ") z(" << b.sz << "," << b.bz << ") [" <<
      b.bx-b.sx << "," << b.by-b.sy << "," << b.bz-b.sz << "]";
   }
};

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
Int closestDist = 1;
//
//int most(int i, const vector<int> &v, std::vector<int> & which, Nanobot& me) {
//   if(i==v.size())
//      return int(which.size());
//   Nanobot nWithout(me);
//   auto without = most(i+1,v,which,nWithout);
//   if(r::all_of(which,[i](auto w){return nanobots[i].intersects(nanobots[w]);})) {
//      which.push_back(i);
//      Nanobot nWith(me);
//      auto with = most(i+1,v,which,nWith);
//      which.pop_back();
//      if(with>without) {
//         me = nWith;
//         return with;
//      }
//   }
//   me = nWithout;
//   return without;
//}
//
void doSmall(const vector<int> v, int i, BoundingBox u) {
   if (v.size()-i < canSee)
      return;
   if(i==0)
      for(auto i:v) {
         cout << std::setw(4) << i << " " << BoundingBox(nanobots[i]) << endl;
      }
   BoundingBox b(nanobots[v[i]]);
   cout << "Counting nanobot " << i << " of " << v.size() << " " << b << endl;
   using std::max;
   using std::min;
   cout << -(max(b.sx,u.sx)-min(b.bx,u.bx)) * (max(b.sy,u.sy)-min(b.by,u.by)) * (max(b.sz,u.sy)-min(b.bz,u.bz)) << endl;
   for(auto x = max(b.sx,u.sx); x<=min(b.bx,u.bx); ++x)
      for(auto y = max(b.sy,u.sy); y<=min(b.by,u.by); ++y)
         for(auto z = max(b.sz,u.sy); z<=min(b.bz,u.bz); ++z) {
            Nanobot me{x,y,z,0};
            auto hereCanSee = r::count_if(v.begin()+i,v.end(),[&](auto t){return nanobots[t].dist(me) <= nanobots[t].r;});
            if(hereCanSee == canSee && me.dist({0,0,0,0}) < closestDist)
               closestDist = me.dist({0,0,0,0});
            else if(hereCanSee > canSee) {
               canSee = hereCanSee;
               closestDist = me.dist({0,0,0,0});
            }
         }
   cout << canSee << endl;
   doSmall(v,i+1,u);
//   vector<int> used;
//   auto howMany = most(0,v,used);
//   if(howMany == canSee && me.dist({0,0,0,0}) < closestDist)
//      closestDist = me.dist({0,0,0,0});
//   else if(howMany > canSee) {
//      canSee = howMany;

}

void solveFor(const vector<int> v, BoundingBox b) {
   if(v.size()<=canSee)
      return;
   Int mx = b.sx + (b.bx-b.sx)/2;
   Int my = b.sy + (b.by-b.sy)/2;
   Int mz = b.sz + (b.bz-b.sz)/2;
   int xc=0,yc=0,zc=0;
   for(auto i:v) {
      auto &n=nanobots[i];
//      if(n.x+n.r < midX) ++xl;
//      else if (n.x-n.r > midX) ++xr;
//      else ++xc;
      if (n.x+n.r >= mx && n.x-n.r <= mx) ++xc;
      if (n.y+n.r >= my && n.y-n.r <= my) ++yc;
      if (n.z+n.r >= mz && n.z-n.r <= mz) ++zc;
   }
//   cout << xc << " " << yc << " " << zc << "\n";
   if (v.size() == xc && xc == yc && yc==zc) {
      cout << "Can't split further " << v.size() << " " << b << endl;
      doSmall(v,0,b);
      cout << canSee << " <-see closestDist-> " << closestDist << "\n";
      return;
   }
   if (xc<=yc && xc<=zc) {//split on x
      vector<int> left;
      vector<int> right;
      for(auto i:v) {
         auto &n=nanobots[i];
         if(n.x-n.r <= mx)
            left.push_back(i);
         if(n.x+n.r >= mx)
            right.push_back(i);
      }
      BoundingBox lBox{b};
      lBox.bx = mx;
      BoundingBox rBox{b};
      rBox.sx = mx;
      cout << v.size() << " -x-> " << left.size() << "," << right.size() << b << "->" << lBox << ":" << rBox << " can see:" << canSee << " dist: " << closestDist << endl;
      solveFor(left, lBox);
      solveFor(right, rBox);
   }
   else if (yc <= zc) {//split on y
      vector<int> left;
      vector<int> right;
      for(auto i:v) {
         auto &n=nanobots[i];
         if(n.y-n.r <= my)
            left.push_back(i);
         if(n.y+n.r >= my)
            right.push_back(i);
      }
      BoundingBox lBox{b};
      lBox.by = my;
      BoundingBox rBox{b};
      rBox.sy = my;
      cout << v.size() << " -y-> " << left.size() << "," << right.size() << b << "->" << lBox << ":" << rBox << " can see:" << canSee << " dist: " << closestDist << endl;

      solveFor(left, lBox);
      solveFor(right, rBox);
   }
   else {// split on z
      vector<int> left;
      vector<int> right;
      for(auto i:v) {
         auto &n=nanobots[i];
         if(n.z-n.r <= mz)
            left.push_back(i);
         if(n.z+n.r >= mz)
            right.push_back(i);
      }
      BoundingBox lBox{b};
      lBox.bz = mz;
      BoundingBox rBox{b};
      rBox.sz = mz;
      cout << v.size() << " -z-> " << left.size() << "," << right.size() << b << "->" << lBox << ":" << rBox << " can see:" << canSee << " dist: " << closestDist << endl;
      solveFor(left, lBox);
      solveFor(right, rBox);
   }
}

}
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
   
   
   cout << (maxX-minX)*(maxY-minY)*(maxZ-minZ) << " = number of points in space \n";
   cout << "minX = " << minX << " maxX = " << maxX << "\n";
   
   unsigned long long sum=0;
   for(auto &&n:nanobots)
      sum+= n.r*n.r*n.r;
   cout << sum << " = sum of radii cubed\n";
   
      vector<int> allIndices = rv::iota(0, nanobots.size()) | r::to_vector;
      solveFor(allIndices,BoundingBox{minX,minY,minZ,maxX,maxY,maxZ});
      cout << canSee << " <-see closestDist-> " << closestDist << "\n";

   //68945139 is too low
   cout << closestDist << "\n";
   //   std::vector<int> v;
   //   most(0,BoundingBox{minX,minY,minZ,maxX,maxY,maxZ},v);
   //   cout << intersects << endl;
   //   Int canSee=0;
   //   std::vector<Nanobot> ctm;
   //   using std::min;
   //   using std::max;
   //
   //   auto count=0;
   //   Int closestDist = 1;
   //   for(auto i=0;i<nanobots.size();++i) {
   //      cout << i << " i\n";
   //      for(auto j=i+1;j<nanobots.size();++j) {
   //         cout << i << " " <<  j << " " << closestDist << "\n";
   //         auto && ni = nanobots[i];
   //         auto && nj = nanobots[j];
   //         if (ni.dist(nj) <= ni.r+nj.r) {
   //            ++count;
   //            auto rix = ni.r;
   //            auto rjx = nj.r;
   //            for(auto x = max(ni.x-rix,nj.x-rjx); x<= min(ni.x+rix,nj.x+rjx); ++x) {
   //               auto riy = rix-abs(ni.x-x);
   //               auto rjy = rjx-abs(nj.x-x);
   //               for(auto y = max(ni.y-rix,nj.y-rjy); y<= min(ni.y+riy,nj.y+rjy); ++y) {
   //                  auto riz = riy-abs(ni.y-y);
   //                  auto rjz = rjy-abs(nj.y-y);
   //                  for(auto z = max(ni.z-riz,nj.z-rjz); z<= min(ni.z+riz,nj.z+rjz); ++z) {
   //                     Nanobot me{x,y,z,0};
   //                     auto hereCanSee = r::count_if(nanobots,[me](auto &&n){return n.dist(me) <= n.r;});
   //                     if(hereCanSee == canSee && me.dist({0,0,0,0}) < closestDist)
   //                        closestDist = me.dist({0,0,0,0});
   //                     else if(hereCanSee > canSee) {
   //                        canSee = hereCanSee;
   //                        closestDist = me.dist({0,0,0,0});
   //                     }
   //                  }
   //               }
   //            }
   //         }
   //      }
   //   }
   //   cout << count << " nanobots overlapping ranges" << "\n";
   //
   //   cout << closestDist << "\n";
   
   //   for(auto x = minX; x<=maxX; ++x) {
   ////      cout << x << "\n";
   //      for(auto y = minY; y<=maxY; ++y)
   //         for(auto z = minZ; z<=maxZ; ++z) {
   //            Nanobot me{x,y,z,0};
   //            auto hereCanSee = r::count_if(nanobots,[me](auto &&n){return n.dist(me) <= n.r;});
   //            if(hereCanSee == canSee)
   //               ctm.push_back(me);
   //            else if(hereCanSee > canSee) {
   //               canSee = hereCanSee;
   //               ctm = {me};
   //            }
   //         }
   //   }
   //
   //   auto best = r::min(ctm,r::less{},[](auto &&n){return n.dist({0,0,0,0});});
   //
   //   cout << best.dist({0,0,0,0}) << "\n";
   
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
   day24stars();
   return 0;
}
