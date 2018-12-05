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
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::cout;
using std::endl;
using std::string;

#include <range/v3/all.hpp>
namespace r=ranges;
namespace rv=r::view;

//*********
//https://blog.tartanllama.xyz/passing-overload-sets/
//https://www.youtube.com/watch?v=L_QKlAx31Pw
#define FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define LIFT(X) [](auto &&... args) \
noexcept(noexcept(X(FWD(args)...)))  \
-> decltype(X(FWD(args)...)) \
{  \
return X(FWD(args)...); \
}
//*********

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
   auto numbers = r::getlines(fin) | rv::transform(LIFT(std::stoi)) | rv::partial_sum;
   cout << "Day 1 star 1: " << back(numbers) << endl;
}

template <typename Rng,CONCEPT_REQUIRES_(r::Range<Rng>())>
auto repeatedValuesHelper(Rng &&seen, Rng &&input)
     -> r::any_view<r::value_type_t<r::iterator_t<Rng>>> {
   if(input.begin() == input.end())
      return input;
   auto &&i = *input.begin();
   if(r::count(seen,i)>0)
      return rv::concat(rv::single(i),repeatedValuesHelper(r::any_view<int>(seen),r::any_view<int>(rv::drop(input,1))));
   return repeatedValuesHelper(r::any_view<int>(rv::concat(seen,rv::single(i))),r::any_view<int>(rv::drop(input,1)));
}

template <typename Rng,CONCEPT_REQUIRES_(r::Range<Rng>())>
auto repeatedValues(Rng input) {
   return repeatedValuesHelper( r::any_view<int>(rv::empty<int>()), r::any_view<int>(input));
}

void day1star2() {
   std::ifstream fin(DIRECTORY+"day1");
   
   auto numbers = r::istream_range<int>(fin) | r::to_vector;

   std::unordered_set<int> seen;
   auto repeats = rv::all(numbers)
                     | rv::cycle
                     | rv::partial_sum
                     | rv::remove_if([&seen](int i) {return seen.insert(i).second;}); //insert returns true if it's new
   
   cout << "Day 1 star 2: " << *repeats.begin() << endl;
}



void day2star1() {
   auto hasRep2Rep3 = [](std::string_view s) {
      std::unordered_map<char,int> times;
      for(auto c:s)
         times[c]++;
      auto twos = r::count_if(times,[](auto p){return p.second==2;});
      auto threes = r::count_if(times,[](auto p){return p.second==3;}); //Not very DRY
//    return std::make_pair(times | r::count(2,&pair<int,int>::second)>0,r::count_if(times,3,second)>0);
      return std::make_pair(twos>0?1:0,threes>0?1:0);
   };
   
   std::ifstream fin(DIRECTORY+"day2");
   auto t = r::accumulate(r::istream_range<string>(fin) | rv::transform(hasRep2Rep3),
                   std::make_pair(0,0),
                   [](auto p, auto q) {
                      return std::make_pair(p.first+q.first,p.second+q.second);
                   });
   cout << "Day 2 star 1: " << t.first * t.second << endl;
}


int main() {
//   day1star1();
//   day1star2();
   day2star1();
   return 0;
}
