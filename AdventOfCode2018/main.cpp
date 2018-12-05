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

void day1star2a() {
   std::ifstream fin(DIRECTORY+"day1");
   
   auto numbers = r::istream_range<int>(fin) | r::to_vector;

   std::unordered_set<int> seen;
   auto repeats = rv::all(numbers)
                     | rv::cycle
                     | rv::partial_sum
                     | rv::remove_if([&seen](int i) {return seen.insert(i).second;}); //insert returns true if it's new
   
   cout << "Day 1 star 2: " << *repeats.begin() << endl;
}

void day1star2b() {
   std::ifstream fin(DIRECTORY+"day1");
   
   auto numbers = r::istream_range<int>(fin) | r::to_vector;
   
   std::unordered_set<int> seen;
   auto frequencies = rv::all(numbers)
      | rv::cycle
      | rv::partial_sum;
   
   cout << "Day 1 star 2: " << (repeatedValues(frequencies) | ranges::view::take(1)) << endl;
}

int main() {
   day1star1();
   day1star2b();
//   static int const ints[] = {1,2,3,1,2,5,6,3,4,5,6,7,8,9};
//   auto intsRange = r::any_view<int>(ints);
//   cout << repeatedValues(intsRange) << "\n";
   return 0;
}
