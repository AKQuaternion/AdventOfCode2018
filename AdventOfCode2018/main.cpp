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
#include <tuple>
using std::get;
#include <utility>
#include <vector>
using std::cout;
using std::endl;
using std::string;

//#include <range/v3/action/transform.hpp>
//#include <range/v3/action/remove_if.hpp>
//#include <range/v3/algorithm/count_if.hpp>
//#include <range/v3/algorithm/find_if.hpp>
//#include <range/v3/algorithm/max_element.hpp>
//#include <range/v3/algorithm/min_element.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
//#include <range/v3/algorithm/reverse.hpp>
//#include <range/v3/algorithm/sort.hpp>
#include <range/v3/getlines.hpp>
//#include <range/v3/istream_range.hpp>
//#include <range/v3/numeric/accumulate.hpp>
//#include <range/v3/range_concepts.hpp>
//#include <range/v3/range_traits.hpp>
//#include <range/v3/to_container.hpp>
//#include <range/v3/utility/concepts.hpp>
//#include <range/v3/utility/iterator_concepts.hpp>
//#include <range/v3/utility/iterator_traits.hpp>
//#include <range/v3/view/any_view.hpp>
//#include <range/v3/view/cartesian_product.hpp>
//#include <range/v3/view/concat.hpp>
//#include <range/v3/view/cycle.hpp>
//#include <range/v3/view/drop.hpp>
//#include <range/v3/view/filter.hpp>
//#include <range/v3/view/for_each.hpp>
//#include <range/v3/view/iota.hpp>
//#include <range/v3/view/join.hpp>
//#include <range/v3/view/partial_sum.hpp>
//#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
//#include <range/v3/view/take.hpp>
//#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>
//#include <range/v3/view/zip_with.hpp>
//#include <range/v3/view_interface.hpp>

#include "oldDays.hpp"

namespace r=ranges;
namespace rv=r::view;
/*
 Addition:

addr (add register) stores into register C the result of adding register A and register B.
addi (add immediate) stores into register C the result of adding register A and value B.
Multiplication:

mulr (multiply register) stores into register C the result of multiplying register A and register B.
muli (multiply immediate) stores into register C the result of multiplying register A and value B.
Bitwise AND:

banr (bitwise AND register) stores into register C the result of the bitwise AND of register A and register B.
bani (bitwise AND immediate) stores into register C the result of the bitwise AND of register A and value B.
Bitwise OR:

borr (bitwise OR register) stores into register C the result of the bitwise OR of register A and register B.
bori (bitwise OR immediate) stores into register C the result of the bitwise OR of register A and value B.
Assignment:

setr (set register) copies the contents of register A into register C. (Input B is ignored.)
seti (set immediate) stores value A into register C. (Input B is ignored.)
Greater-than testing:

gtir (greater-than immediate/register) sets register C to 1 if value A is greater than register B. Otherwise, register C is set to 0.
gtri (greater-than register/immediate) sets register C to 1 if register A is greater than value B. Otherwise, register C is set to 0.
gtrr (greater-than register/register) sets register C to 1 if register A is greater than register B. Otherwise, register C is set to 0.
Equality testing:

eqir (equal immediate/register) sets register C to 1 if value A is equal to register B. Otherwise, register C is set to 0.
eqri (equal register/immediate) sets register C to 1 if register A is equal to value B. Otherwise, register C is set to 0.
eqrr (equal register/register) sets register C to 1 if register A is equal to register B. Otherwise, register C is set to 0.
*/
std::vector<int> rr(4);

void apply(const string &line){
   std::istringstream sin(line);
   std::string i;
   int op1, op2, op3;
   sin >> i >> op1 >> op2 >> op3;
   if (i=="addr") {
      rr[op3] = rr[op1]+rr[op2];
   } else if (i=="addi") {
      rr[op3] = rr[op1]+op2;
   } else if (i=="mulr") {
      rr[op3] = rr[op1]*rr[op2];
   } else if (i=="muli") {
      rr[op3] = rr[op1]*op2;
   } else if (i=="banr") {
      rr[op3] = rr[op1]&rr[op2];
   } else if (i=="bani") {
      rr[op3] = rr[op1]&op2;
   } else if (i=="borr") {
      rr[op3] = rr[op1]|rr[op2];
   } else if (i=="bori") {
      rr[op3] = rr[op1]|op2;
   } else if (i=="setr") {
      rr[op3] = rr[op1];
   } else if (i=="seti") {
      rr[op3] = op1;
   } else if (i=="gtir") {
      rr[op3] = op1>rr[op2];
   } else if (i=="gtri") {
      rr[op3] = rr[op1]>op2;
   } else if (i=="gtrr") {
      rr[op3] = rr[op1]+rr[op2];
   } else if (i=="eqir") {
      rr[op3] = op1==rr[op2];
   } else if (i=="eqri") {
      rr[op3] = rr[op1]==op2;
   } else if (i=="eqrr") {
      rr[op3] = rr[op1]==rr[op2];
   } else throw 1;
}

void day15stars() {
   using std::string;
   string opcodes[] = {"addr","addr","mulr","muli","banr","bani","borr","bori","setr","seti","gtir","gtri","gtrr","eqir","eqri","eqrr"};
   
   std::ifstream fin(DIRECTORY+"day15");
   auto tests = r::getlines(fin) | r::to_vector;
   
   auto line=0ull;
   auto b=0;
   while (true) {
      std::istringstream lin1(tests[line++]);
      // Before: [0, 3, 1, 1]
      string _;
      char _c;
      std::vector<int> tt(4);
      lin1 >> _ >> _c >> tt[0] >> _c >> tt[1] >> _c >> tt[2] >>_c >> tt[3];
      if (_ != "Before:")
         break;
      cout << tt[0] << tt[1] << tt[2] << tt[3] << endl;
      std::istringstream lin2(tests[line++]);
      int instr;
      lin2 >> instr;
      string rest;
      std::getline(lin2,rest);
      cout << opcodes[instr] + " " + rest << endl;

      std::istringstream lin3(tests[line++]);
      std::vector<int> rs(4);
      lin3 >> _ >> _c >> rs[0] >> _c >> rs[1] >> _c >> rs[2] >>_c >> rs[3];
      cout << rs[0] << rs[1] << rs[2] << rs[3] << endl;

      int count=0;
      for(int i=0;i<16;++i) {
         rr = tt;
         string exec = opcodes[i] + " " + rest;
         apply(exec);
         if(rr==rs)
            count++;
      }
      if(count>=3)
         ++b;
      ++line;
   }
   cout << b << endl;
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
   day15stars();
   return 0;
}
