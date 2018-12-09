//
//  main.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/3/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::get;

#include <range/v3/getlines.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/view/for_each.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/any_view.hpp>

#include "oldDays.hpp"
#include <sstream>

namespace r=ranges;
namespace rv=r::view;

void day4star1() {
//   [1518-11-01 00:00] Guard #10 begins shift
//   [1518-11-01 00:05] falls asleep
//   [1518-11-01 00:25] wakes up
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

int main() {
//   day1star1();
//   day1star2();
//   day2star1();
//   day2star2();
//   day3stars();
   day4star1();
   return 0;
}
