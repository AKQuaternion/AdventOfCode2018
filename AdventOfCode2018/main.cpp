//
//  main.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/3/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::get;

#include <range/v3/all.hpp>
namespace r=ranges;
namespace rv=r::view;

const string DIRECTORY = "/Users/hartman/Documents/Xcode projects/AdventOfCode2018/";

template <typename Rng,CONCEPT_REQUIRES_(!r::BoundedRange<Rng>())>
auto back(Rng &&r) -> r::value_type_t<r::iterator_t<Rng>> {
   r::value_type_t<r::iterator_t<Rng>> ret{};
   for(auto && v:r)
      ret=std::move(v);
      return ret;
}

void day1star1() {
   std::ifstream fin(DIRECTORY+"day1");
   auto numbers = r::istream_range<int>(fin) | rv::partial_sum;
   cout << "Day 1 star 1: " << back(numbers) << endl;
}

void day1star2() {
   std::ifstream fin(DIRECTORY+"day1");
   
   auto numbers = r::istream_range<int>(fin) | r::to_vector;
   
   std::unordered_set<int> seen;
   auto repeats = rv::all(numbers)
   | rv::cycle
   | rv::partial_sum
   | rv::filter([&seen](int i) {return !seen.insert(i).second;}) //insert returns true if it's new
   | rv::take(1);
   cout << "Day 1 star 2: " << repeats << endl;
}

void day2star1() {
   auto hasRep2Rep3 = [](std::string_view s) {
      std::unordered_map<char,int> times;
      for(auto c:s)
         times[c]++;
      auto twice = r::count_if(times,[](auto p){return p.second==2;});
      auto thrice = r::count_if(times,[](auto p){return p.second==3;});
      return std::make_pair(twice>0,thrice>0);
   };
   
   std::ifstream fin(DIRECTORY+"day2");
   auto t = r::accumulate(r::istream_range<string>(fin) | rv::transform(hasRep2Rep3),
                          std::make_pair(0,0),
                          [](auto p, auto q) {
                             if(q.first) ++p.first;
                             if(q.second) ++p.second;
                             return p;
                          });
   
   cout << "Day 2 star 1: " << t.first * t.second << endl;
}

void day2star2() {
   std::ifstream fin(DIRECTORY+"day2");
   auto ids = r::istream_range<string>(fin) | r::to_vector;

   auto hammingDistance = [](auto &&id1, auto &&id2) {
      return r::accumulate(rv::zip_with([](auto c1, auto c2){
         return c1==c2?0:1;
      },id1,id2),0);
   };
   
   auto matches = [](std::string_view s1, std::string_view s2) {
      return r::accumulate(rv::zip(s1,s2),std::string{},
               [](auto s, auto p) {
                  if(p.first==p.second) s+= p.first; return s;
               });
   };
   
   auto word = rv::cartesian_product(ids,ids)
               | rv::filter([&](const auto &p){
                  const auto &[id1,id2]=p; return hammingDistance(id1,id2)==1;
                  })
               | rv::transform([&](const auto &p){
                  const auto &[id1,id2]=p; return matches(id1,id2);})
               | rv::take(1);
   
   cout << "Day 2 star 2: " << word << "\n";
}

void day3stars() {
   struct Claim {
      int elf,left,top,width,height;
   };
   auto str2Claim = [](const string &s) {
      //#14 @ 851,648: 13x15
      std::istringstream sin(s);
      char _;
      Claim c;
      sin >> c.elf >> _ >> c.left >> _ >> c.top >> _ >> c.width >> _ >> c.height;
      return c;
   };
   
   std::ifstream fin(DIRECTORY+"day3");
   auto claims = r::getlines(fin) | rv::transform(str2Claim);

   static const int DIM=1000;
   using Cloth = std::vector<int>;
   
   auto cover = [](Cloth &cloth, const Claim &c) -> Cloth & {
      for(auto x=0; x<c.width; ++x)
         for(auto y=0; y<c.height; ++y)
            cloth[DIM*(c.left+x)+c.top+y]++;
      return cloth;
   };
   
   Cloth freshCloth(DIM*DIM);
   freshCloth = r::accumulate(claims,freshCloth,cover);

   cout << "Day 3 star 1: " << r::count_if(freshCloth,[](auto n){return n>1;}) << endl;
//   auto intact = r::find_if(claims,[&freshCloth](const Claim &c){
//                              for(auto x=0; x<c.width; ++x)
//                                 for(auto y=0; y<c.height; ++y)
//                                    if(freshCloth[DIM*(c.left+x)+c.top+y] != 1)
//                                       return false;
//                              return true;});
//   cout << "Day 3 star 2: " << (*intact).elf << endl;
}

int main() {
//   day1star1();
//   day1star2();
//   day2star1();
//   day2star2();
   day3stars();
   return 0;
}
