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
#include <range/v3/utility/concepts.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/range_traits.hpp>

template <typename Rng,CONCEPT_REQUIRES_(!ranges::BoundedRange<Rng>())>
auto back(Rng &&r) -> ranges::value_type_t<ranges::iterator_t<Rng>> {
   ranges::value_type_t<ranges::iterator_t<Rng>> ret{};
   for(auto && v:r)
      ret=std::move(v);
      return ret;
}

void day1star1();
void day1star2();
void day2star1();
void day2star2();
void day3stars();

extern const std::string DIRECTORY;

#endif /* oldDays_hpp */
