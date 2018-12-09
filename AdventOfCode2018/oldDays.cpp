//
//  oldDays.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/5/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include "oldDays.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stack>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::cout;
using std::endl;
using std::string;

#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/getlines.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/utility/concepts.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/view/any_view.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/cycle.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/for_each.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/partial_sum.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>
#include <range/v3/view/zip_with.hpp>
#include <range/v3/view_interface.hpp>

template <typename Rng,CONCEPT_REQUIRES_(!ranges::BoundedRange<Rng>())>
auto back(Rng &&r) -> ranges::value_type_t<ranges::iterator_t<Rng>> {
   ranges::value_type_t<ranges::iterator_t<Rng>> ret{};
   for(auto && v:r)
      ret=std::move(v);
      return ret;
}

namespace r=ranges;
namespace rv=r::view;

const std::string DIRECTORY = "/Users/hartman/Documents/Xcode projects/AdventOfCode2018/";

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
      return rv::zip(s1,s2)
      | rv::filter([](auto p){return p.first==p.second;})
      | rv::transform([](auto p){return p.first;})
      | r::to_<std::string>();
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
      sin >> _ >> c.elf >> _ >> c.left >> _ >> c.top >> _ >> c.width >> _ >> c.height;
      return c;
   };
   
   std::ifstream fin(DIRECTORY+"day3");
   auto claims = r::getlines(fin) | rv::transform(str2Claim) | r::to_vector;
   
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
   auto intact = r::find_if(claims,[&freshCloth](const Claim &c){
      for(auto x=0; x<c.width; ++x)
         for(auto y=0; y<c.height; ++y)
            if(int a=freshCloth[DIM*(c.left+x)+c.top+y] != 1)
               return false;
      return true;});
   cout << "Day 3 star 2: " << (*intact).elf << endl;
}

void day4stars() {
   std::ifstream fin(DIRECTORY+"day4");
   auto records = r::getlines(fin) | r::to_vector;
   r::sort(records);
   
   struct Record {
      int guard,minute;
      Record(const std::string &s) {
         std::istringstream sin(s);
         string _s; char _c; int _i; //ignore this data
         sin >> _s >> _i >> _c >> minute >> _c >> _s >> _c;
         if (_s == "Guard")
            sin >> guard;
         else
            guard = -1;
      }
   };
   
   struct Nap {
      int guard,sleep,wake;
   };
   
   auto naps = records
   | rv::transform([](const string &s){return Record(s);})
   | rv::for_each([](const auto &rec) -> r::any_view<Nap> {
      static int guard;
      static int sleep = -1;
      if (rec.guard != -1) {
         guard = rec.guard;
         return r::yield_from(r::any_view<Nap>{});
      }
      if(sleep == -1) {
         sleep = rec.minute;
         return r::yield_from(r::any_view<Nap>{});
      }
      Nap n{guard,sleep,rec.minute};
      sleep = -1;
      return r::yield(n);
   });
   
   auto sleep = [](auto & st, const Nap& n) ->std::unordered_map<int,int>&{
      st[n.guard] += n.wake-n.sleep;
      return st;
   };
   
   std::unordered_map<int,int> sleepTimes;
   
   sleepTimes = r::accumulate(naps,sleepTimes,sleep);
   
   auto bestGuard = r::max_element(sleepTimes,std::less<>(),&std::pair<const int,int>::second)->first;
   
   struct GuardMinuteNaps {
      int guard, minute, naps;
   };
   
   auto guardNapAccumulator = [naps](int guard) {
      auto record = [guard](auto & v, const Nap&n) -> std::vector<int> & {
         if (n.guard != guard)
            return v;
         for (int ii=n.sleep; ii<n.wake;++ii)
            v[ii]++;
         return v;
      };
      return record;
   };
   
   auto guardHighestMinuteNaps = [&](int guard) {
      std::vector<int> minutes(60);
      minutes = r::accumulate(naps,minutes,guardNapAccumulator(guard));
      auto bestMinute = r::max_element(minutes);
      return GuardMinuteNaps{guard,int(bestMinute-minutes.begin()),*bestMinute};
   };
   
   cout << "Day 4 star 1: "
   << bestGuard*guardHighestMinuteNaps(bestGuard).minute << endl;
   
   auto guardsNapTimes = sleepTimes
   | rv::transform([](const auto & p){return p.first;})
   | rv::transform(guardHighestMinuteNaps);
   
   auto mostnap = *r::max_element(guardsNapTimes ,
                                  std::less<>(),
                                  &GuardMinuteNaps::naps);
   
   cout << "Day 4 star 2: " << mostnap.guard * mostnap.minute << endl;
}

void day5stars() {
   std::ifstream fin(DIRECTORY+"day5");
   auto polymer = r::istream_range<char>(fin) | r::to_vector;
   
   auto react = [](auto && reaction, auto c) -> auto & {
      if (reaction.empty() || abs(reaction.top()-c) != abs('a'-'A'))
         reaction.push(c);
      else
         reaction.pop();
      return reaction;
   };
   
   auto reactSize = [react](auto p) {
      return r::accumulate(p,std::stack<char>{},react).size();};
   
   cout << "Day 5 star 1: " << reactSize(polymer) << "\n";
   
   auto removeReagent = [polymer](auto r) {
      return polymer | rv::remove_if([r](auto c){return toupper(c)==r;});};
   
   auto reagents = polymer
   | rv::transform([](auto c){return toupper(c);})
   | r::to_<std::set<char>>();
   
   auto polymersRemovedSizes = reagents
   | rv::transform(removeReagent)
   | rv::transform(reactSize);
   
   auto shortest = *r::min_element(polymersRemovedSizes);
   
   cout << "Day 5 star 2: " << shortest << "\n";
}
