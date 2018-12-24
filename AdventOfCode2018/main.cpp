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
#include <range/v3/algorithm/count_if.hpp>
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
//#include <range/v3/numeric/accumulate.hpp>
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


namespace day22 {
   using std::string;
   using std::vector;
   using std::ostream;
   
   struct Loc {
      int x, y, tool;
      bool operator<(const Loc rhs) const {
         return std::tie(x,y,tool) < std::tie(rhs.x, rhs.y, rhs.tool);
      }
      friend ostream & operator<<(ostream &os, const Loc &l) {
         return os << "("<<l.x<<","<<l.y<<") tool: "<<l.tool;
      }
   };
   
}

void day22stars() {
   //depth: 11109
   //target: 9,731
   using namespace day22;
   //
   //   const int maxx=10;
   //   const int maxy=10;
   //   const auto depth = 510;
   const int maxx   = 9;
   const int maxy   = 731;
   const auto depth = 11109;
   
   vector<vector<int>> gi(4*maxy+1,vector<int>(6*maxx+1));
   vector<vector<int>> el(gi);
   vector<vector<int>> type(6*maxx+1,vector<int>(4*maxy+1));
   
   
   //   The region at 0,0 (the mouth of the cave) has a geologic index of 0.
   //   The region at the coordinates of the target has a geologic index of 0.
   //   If the region's Y coordinate is 0, the geologic index is its X coordinate times 16807.
   //   If the region's X coordinate is 0, the geologic index is its Y coordinate times 48271.
   //   Otherwise, the region's geologic index is the result of multiplying the erosion levels of the regions at X-1,Y and X,Y-1.}
   unsigned long long sum=0;
   for(auto y=0;y<gi.size();++y)
      for(auto x = 0;x<gi[y].size();++x) {
         if (x==0&&y==0) gi[y][x]=0;
         else if (y==maxy && x == maxx) gi[y][x] = 0;
         else if(y==0) gi[y][x] = x*16807%20183;
         else if(x==0) gi[y][x] = y*48271%20183;
         else gi[y][x] = (el[y][x-1]*el[y-1][x])%20183;
         
         el[y][x] = ((gi[y][x]+depth)%20183);
         type[x][y] = el[y][x]%3;
         if(x<=maxx & y<=maxy)
            sum += type[x][y];
      } //At (6,15) tool: 1 with cost 35
   cout << "star 1 " << sum << endl;
   using std::pair;
   
   //   cout << type[0][0] << type[1][0] << endl;
   //   cout << type[0][1] << type[1][1] << endl;
   
   using std::make_pair;
   std::map<Loc,vector<pair<Loc,int>>> m;
   for(auto y=0;y<gi.size()-1;++y)
      for(auto x = 0;x<gi[y].size()-1;++x) {
         Loc l1{x,y,type[x][y]};
         Loc l2{x,y,(type[x][y]+1)%3};
         m[l1].push_back(make_pair(l2,7));
         //         cout << l1 << " --> " << l2 << " cost " << 7 << endl;
         m[l2].push_back(make_pair(l1,7));
         
         auto locr1 =Loc{x+1,y,type[x+1][y]};
         auto locr2 =Loc{x+1,y,(type[x+1][y]+1)%3};
         
         auto connect = [&m](auto && x, auto &&y) {
            if(x.tool == y.tool) {
               m[x].push_back(make_pair(y,1));
               //               cout << x << " --> " << y << " cost " << 1 << endl;
               m[y].push_back(make_pair(x,1));
               //               cout << x << " --> " << y << endl;
            }
         };
         
         connect(l1,locr1);
         connect(l2,locr1);
         connect(l1,locr2);
         connect(l2,locr2);
         
         auto locd1 =Loc{x,y+1,type[x][y+1]};
         auto locd2 =Loc{x,y+1,(type[x][y+1]+1)%3};
         
         connect(l1,locd1);
         connect(l2,locd1);
         connect(l1,locd2);
         connect(l2,locd2);
      }
   
   std::set<Loc> visited;
   std::priority_queue<pair<int,Loc>,vector<pair<int,Loc>>,std::greater<pair<int,Loc>>> q;
   q.push(make_pair(0,Loc{0,0,0}));
   while (!q.empty()) { // do Dijkstra-dfs
      auto [cost,loc] = q.top();
      q.pop();
      if (visited.count(loc))
         continue;
      visited.insert(loc);
      if (loc.x==maxx && loc.y==maxy & loc.tool == 0) {
         cout << "star 2 " << cost << "\n";
         return;
      }
      for(auto [newloc,dcost]:m[loc])
         q.push(make_pair(cost+dcost,newloc));
   }
}

#include <iomanip>

using Int = long long;

struct Nanobot {
   Int x,y,z,r;
   Int dist(const Nanobot &r) const {
      return (abs(x-r.x)+abs(y-r.y)+abs(z-r.z));
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

void doSmall(const vector<int> &v, int i, const BoundingBox &u) {
   if(i==v.size()-1) return; //!!! if (i > v.size()-canSee)
   BoundingBox b(nanobots[v[i]]);
   cout << "Counting nanobot " << i << " of " << v.size() << " " << b << endl;
   using std::max;
   using std::min;
   for(auto x = max(b.sx,u.sx); x<=min(b.bx,u.bx); ++x)
      for(auto y = max(b.sy,u.sy); y<=min(b.by,u.by); ++y)
         for(auto z = max(b.sz,u.sy); z<=min(b.bz,u.bz); ++z) {
            Nanobot me{x,y,z,0};
            auto hereCanSee = r::count_if(v.begin()+i+1,v.end(),[&](auto t){return nanobots[t].dist(me) <= nanobots[t].r;}); //
            if(hereCanSee == canSee && me.dist({0,0,0,0}) < closestDist)
               closestDist = me.dist({0,0,0,0});
            else if(hereCanSee > canSee) {
               canSee = hereCanSee;
               closestDist = me.dist({0,0,0,0});
            }
         }
   doSmall(v,i+1,u);
}

void solveFor(const vector<int> v, BoundingBox b) {
   Int mx = b.sx + (b.bx-b.sx)/2;
   Int my = b.sy + (b.by-b.sy)/2;
   Int mz = b.sz + (b.bz-b.sz)/2;
   int xc=0,yc=0,zc=0;
   for(auto i:v) {
      auto &n=nanobots[i];
//      if(n.x+n.r < midX) ++xl;
//      else if (n.x-n.r > midX) ++xr;
//      else ++xc;
      if (n.x+n.r > mx && n.x-n.r < mx) ++xc;
      if (n.y+n.r > my && n.y-n.r < my) ++yc;
      if (n.z+n.r > mz && n.z-n.r < mz) ++zc;
   }
   cout << xc << " " << yc << " " << zc << "\n";
   if (v.size() == xc && xc == yc && yc==zc) {
      cout << "Can't split further " << xc << " " << b << endl;
      doSmall(v,0,b);
      return;
   }
   if (xc<=yc && xc<=zc) {//split on x
      vector<int> left;
      vector<int> right;
      for(auto i:v) {
         auto &n=nanobots[i];
         if(n.x-n.r < mx)
            left.push_back(i);
         if(n.x+n.r > mx)
            right.push_back(i);
      }
      BoundingBox lBox{b};
      lBox.bx = mx;
      BoundingBox rBox{b};
      rBox.sx = mx;
      cout << v.size() << " -x-> " << left.size() << "," << right.size() << " can see:" << canSee << endl;
      if(left.size()>=canSee) solveFor(left, lBox);
      if(right.size()>=canSee) solveFor(right, rBox);
   }
   else if (yc <= zc) {//split on y
      vector<int> left;
      vector<int> right;
      for(auto i:v) {
         auto &n=nanobots[i];
         if(n.y-n.r < my)
            left.push_back(i);
         if(n.y+n.r > my)
            right.push_back(i);
      }
      BoundingBox lBox{b};
      lBox.by = my;
      BoundingBox rBox{b};
      rBox.sy = my;
      cout << v.size() << " -y-> " << left.size() << "," << right.size() << " can see:" << canSee << endl;

      if(left.size()>=canSee) solveFor(left, lBox);
      if(right.size()>=canSee) solveFor(right, rBox);
   }
   else {// split on z
      vector<int> left;
      vector<int> right;
      for(auto i:v) {
         auto &n=nanobots[i];
         if(n.z-n.r < mz)
            left.push_back(i);
         if(n.z+n.r > mz)
            right.push_back(i);
      }
      BoundingBox lBox{b};
      lBox.bz = mz;
      BoundingBox rBox{b};
      rBox.sz = mz;
      cout << v.size() << " -z-> " << left.size() << "," << right.size() << " can see:" << canSee << " dist: " << closestDist << endl;
      if(left.size()>=canSee) solveFor(left, lBox);
      if(right.size()>=canSee) solveFor(right, rBox);
   }
}


void day23stars() {
   using std::string;
   
   auto strToNanoBot = [](const string &s) {
      std::istringstream sin(s);
      char _;
      int x,y,z,r;
      //      pos=<63050011,17169307,53101295>, r=61836378
      //      pos=<19230077,48430355,4672540>, r=65947720
      sin >>_>>_>>_>>_>>_ >> x >>_ >> y >>_ >> z >>_>>_>>_>>_ >> r;
      return Nanobot{x,y,z,r};
   };
   
   std::ifstream fin(DIRECTORY + "day23test");
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
   day23stars();
   return 0;
}
