//
//  main.cpp
//  AdventOfCode2018
//
//  Created by Chris Hartman on 12/3/18.
//  Copyright © 2018 Chris Hartman. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <set>
#include <tuple>
using std::get;
#include <utility>
#include <vector>
using std::cout;
using std::endl;
using std::string;

//#include <range/v3/getlines.hpp>
#include <range/v3/to_container.hpp>
//#include <range/v3/algorithm/sort.hpp>
//#include <range/v3/algorithm/count_if.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
//#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/istream_range.hpp>
//#include <range/v3/view/for_each.hpp>
//#include <range/v3/numeric/accumulate.hpp>
//#include <range/v3/view/any_view.hpp>
//#include <range/v3/view/cartesian_product.hpp>
//#include <range/v3/view/iota.hpp>
//#include <range/v3/view/concat.hpp>
//#include <range/v3/view/drop.hpp>
//#include <range/v3/view/filter.hpp>
//#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
#include <range/v3/view/split.hpp>
//#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>

#include "oldDays.hpp"
#include <sstream>

namespace r=ranges;
namespace rv=r::view;



void day8stars() {
   class Tree {
   public:
      Tree(std::vector<int> data):_data(std::move(data))
      {}
      
      int sumOfMeta() {
         auto i=_data.cbegin();
         auto sum = sumOfMetaHelper(i);
         assert(i==_data.cend());
         return sum;
      }
      
      int value() {
         auto i=_data.cbegin();
         auto sum = valueHelper(i);
         assert(i==_data.cend());
         return sum;
      }

   private:
      int sumOfMetaHelper(std::vector<int>::const_iterator &i) {
         auto numChildren = *i++;
         auto numMeta = *i++;
         auto sum = 0;
         for(int c=0;c<numChildren;++c)
            sum += sumOfMetaHelper(i);
         for(int m=0;m<numMeta;++m)
            sum += *i++;
         return sum;
      }
      
      int valueHelper(std::vector<int>::const_iterator &i) {
         auto numChildren = *i++;
         auto numMeta = *i++;
         if (numChildren==0) {
            auto sumOfMeta = 0;
            for(int m=0;m<numMeta;++m)
               sumOfMeta += *i++;
            return sumOfMeta;
         }
         std::vector<int> childValues;
         for(int c=0;c<numChildren;++c)
            childValues.push_back(valueHelper(i));
         auto sumValue = 0;
         for(int m=0;m<numMeta;++m) {
            auto meta = *i++ - 1;
            if (meta < childValues.size())
               sumValue += childValues[meta];
         }
         return sumValue;
      }
      std::vector<int> _data;
   };

   std::ifstream fin(DIRECTORY+"day8");
   
   Tree t{r::istream_range<int>(fin)};
//   auto sin = std::istringstream{"2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2"};
//   Tree t{r::istream_range<int>{sin}};
   cout << "Day 8 star 1: " << t.sumOfMeta() << "\n";
   cout << "Day 8 star 2: " << t.value() << "\n";
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
   day8stars();
   return 0;
}
