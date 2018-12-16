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
#include <range/v3/view/all.hpp>
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
//      int opmap[] = {4,3,7,8,1,15,11,10,6,14,5,0,13,2,9,12}; // 503
//      int opmap[] = {4,3,7,8,1,15,11,12,6,14,5,0,13,2,9,10};   // 441
//      int opmap[] = {4,3,7,8,1,15,11,0,6,14,5,12,13,2,9,10}; //441
//      int opmap[] = {4,3,7,8,1,15,11,10,6,14,5,12,13,2,9,0}; //503
        int opmap[] = {4,3,7,8,1,15,11,10,6,14,5,0,13,2,9,12}; //503
void apply(const string &line){
   std::istringstream sin(line);
   std::string i;
   int op1, op2, op3;
   sin >> i >> op1 >> op2 >> op3;
   if (i=="addr") {
      rr.at(op3) = rr.at(op1)+rr.at(op2);
   } else if (i=="addi") {
      rr.at(op3) = rr.at(op1)+op2;
   } else if (i=="mulr") {
      rr.at(op3) = rr.at(op1)*rr.at(op2);
   } else if (i=="muli") {
      rr.at(op3) = rr.at(op1)*op2;
   } else if (i=="banr") {
      rr.at(op3) = rr.at(op1)&rr.at(op2);
   } else if (i=="bani") {
      rr.at(op3) = rr.at(op1)&op2;
   } else if (i=="borr") {
      rr.at(op3) = rr.at(op1)|rr.at(op2);
   } else if (i=="bori") {
      rr.at(op3) = rr.at(op1)|op2;
   } else if (i=="setr") {
      rr.at(op3) = rr.at(op1);
   } else if (i=="seti") {
      rr.at(op3) = op1;
   } else if (i=="gtir") {
      rr.at(op3) = op1>rr.at(op2)?1:0;
   } else if (i=="gtri") {
      rr.at(op3) = rr.at(op1)>op2?1:0;
   } else if (i=="gtrr") {
      rr.at(op3) = rr.at(op1)>rr.at(op2)?1:0;
   } else if (i=="eqir") {
      rr.at(op3) = op1==rr.at(op2)?1:0;
   } else if (i=="eqri") {
      rr.at(op3) = rr.at(op1)==op2?1:0;
   } else if (i=="eqrr") {
      rr.at(op3) = rr.at(op1)==rr.at(op2)?1:0;
   } else throw 1;
}

void day16stars() {
   using std::string;
//   6 =~ bani banr gtir borr bori gtrr muli addi setr mulr gtri seti addr
//   2 =~ bori
//   5 =~ bani banr gtir eqir eqrr gtrr muli setr mulr gtri
//   15 =~ bani gtir borr bori eqri gtrr muli setr addr
//   2 =~ bori
//   14 =~ borr bori addi seti
//   11 =~ muli addi mulr addr
//   9 =~ bani bori eqri muli setr
//   6 =~ bani banr gtir borr bori gtrr muli addi setr mulr gtri seti addr
   string opcodes[] = {"addr","addi","mulr","muli","banr","bani","borr","bori","setr","seti","gtir","gtri","gtrr","eqir","eqri","eqrr"};
//   string opcodes[] = {"bani","banr","gtir","borr","bori","eqir","eqri","eqrr","gtrr","muli","addi","setr","mulr","gtri","seti","addr"};

   for(int ii=0;ii<16;++ii)
      cout << ii << " " << opcodes[opmap[ii]] <<endl;
   std::ifstream fin(DIRECTORY+"day16");
   auto tests = r::getlines(fin) | r::to_vector;
   
   auto line=0ull;
   auto b=0;
   auto samples=0;
   std::map<int,std::set<int>> choices;
   std::set<int> s = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
   for(int ii=0;ii<16;++ii)
      choices[ii]=s;
//   for(int i=0;i<16;++i)
//      cout << i << " " << rv::all(choices[i)) << endl;
   bool bad=false;
   while (true) {
      std::istringstream lin1(tests[line++]);
      // Before: [0, 3, 1, 1)
      string _;
      char _c;
      std::vector<int> tt(4);
      lin1 >> _ >> _c >> tt[0] >> _c >> tt[1] >> _c >> tt[2] >>_c >> tt[3];
      if (_ != "Before:")
         break;
//      cout << tt[0) << tt[1) << tt[2) << tt[3) << endl;
      std::istringstream lin2(tests[line++]);
      int instr;
      lin2 >> instr;
      string rest;
      std::getline(lin2,rest);
//      cout << opcodes[instr) + " " + rest << endl;

      std::istringstream lin3(tests[line++]);
      std::vector<int> rs(4);
      lin3 >> _ >> _c >> rs[0] >> _c >> rs[1] >> _c >> rs[2] >>_c >> rs[3];
//      cout << rs[0) << rs[1) << rs[2) << rs[3) << endl;

//      string exec = opcodes[opmap[instr]] + " " + rest;
//      rr = tt;
//      apply(exec);
//      if(rr!=rs) {
//         bad=true;
//         cout << "***************************\n";
//      }
//      0 [4)
//      1 [3)
//      2 [7)
//      3 [8)
//      4 [1)
//      5 [15)
//      6 [11)
//      7 [0,10,12)
//      8 [6)
//      9 [14)
//      10 [5)
//      11 [0,12)
//      12 [13)
//      13 [2)
//      14 [9)
//      15 [0,10,12)
      int count=0;
      cout << instr << " =~ ";
      for(int i=0;i<16;++i) {
         string exec = opcodes[i] + " " + rest;
         rr = tt;
         apply(exec);
         if(rr!=rs) {
            choices[instr].erase(i);
            if(choices[instr].size()==1) {
               for(int jj=0;jj<16;++jj)
                  if(jj!=instr)
                     choices[jj].erase(*choices[instr].begin());
            }
         } else {
            cout << opcodes[i] << " ";
            ++count;
         }
      }
      cout << endl;
      if(count>=3)
         ++b;
      ++line;
      ++samples;
   }
   cout << "Day 14 star 1: " << b << endl;
   for(int i=0;i<16;++i) {
      cout << i << " ";
      for (auto s:choices[i])
         cout << opcodes[s] << " ";
      cout << endl;
   }
   cout << samples << " samples" <<  endl;
   if (!bad) cout << "All good!\n";
   ++line;
//   cout << tests[line)<<endl;
   rr = {0,0,0,0};
   for(;line<tests.size();++line) {
      //      0 [4)
      //      1 [3)
      //      2 [7)
      //      3 [8)
      //      4 [1)
      //      5 [15)
      //      6 [11)
      //      7 [0,10,12)
      //      8 [6)
      //      9 [14)
      //      10 [5)
      //      11 [0,12)
      //      12 [13)
      //      13 [2)
      //      14 [9)
      //      15 [0,10,12)
//      0 [4]
//      1 [3]
//      2 [7]
//      3 [8]
//      4 [1]
//      5 [15]
//      6 [11]
//      7 [0,10,12]
//      8 [6]
//      9 [14]
//      10 [5]
//      11 [0,12]
//      12 [13]
//      13 [2]
//      14 [9]
//      15 [0,10,12]
      std::istringstream lin2(tests[line]);
      int instr;
      lin2 >> instr;
      string rest;
      std::getline(lin2,rest);
      string exec = opcodes[opmap[instr]] + rest;
      apply(exec);
      cout << rr[0] << " " << rr[1] << " " << rr[2] << " " << rr[3] << " <- ";
      cout << exec << endl;
   }
   cout << "Day 16 star 2: " << rr.at(0) << endl;
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
   day16stars();
   return 0;
}
