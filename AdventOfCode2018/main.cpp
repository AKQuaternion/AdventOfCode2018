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
auto back(Rng r) -> r::value_type_t<r::iterator_t<Rng>> {
   r::value_type_t<r::iterator_t<Rng>> ret{};
   for(auto && v:r)
      ret=std::move(v);
   return ret;
}

void day1star1() {
   std::ifstream fin(DIRECTORY+"day1");
   auto numbers = r::getlines(fin) | rv::transform(LIFT(std::stoi)) | rv::partial_sum();
   cout << "day1 star1: " << back(numbers) << endl;
}

void day1star2() {
   std::ifstream fin(DIRECTORY+"day1");
   
   std::vector<int> numbers;
   ranges::copy(r::getlines(fin) | rv::transform(LIFT(std::stoi)), ranges::back_inserter(numbers));
   
   std::unordered_set<int> seen;
   auto notSeenBefore = [&seen](int i) {
      if(seen.count(i)==0) {
         seen.insert(i);
         return true;
      }
      return false;
   };
   
   auto repeats = rv::all(numbers) | rv::cycle | rv::partial_sum() | rv::remove_if(notSeenBefore);
   cout << "day1 star2: " << *repeats.begin() << endl;
}

int main() {
   day1star1();
   day1star2();
   return 0;
}
