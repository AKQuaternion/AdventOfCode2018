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
#include <range/v3/algorithm/any_of.hpp>
//#include <range/v3/algorithm/find_if.hpp>
//#include <range/v3/algorithm/max.hpp>
//#include <range/v3/algorithm/min.hpp>
//#include <range/v3/algorithm/max_element.hpp>
//#include <range/v3/algorithm/min_element.hpp>
//#include <range/v3/algorithm/minmax_element.hpp>
//#include <range/v3/algorithm/reverse.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/getlines.hpp>
//#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
//#include <range/v3/range_concepts.hpp>
//#include <range/v3/range_traits.hpp>
//#include <range/v3/to_container.hpp>
//#include <range/v3/utility/concepts.hpp>
//#include <range/v3/utility/iterator_concepts.hpp>
//#include <range/v3/utility/iterator_traits.hpp>
//#include <range/v3/view/all.hpp>
//#include <range/v3/view/cartesian_product.hpp>
//#include <range/v3/view/concat.hpp>
//#include <range/v3/view/cycle.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
//#include <range/v3/view/for_each.hpp>
//#include <range/v3/view/iota.hpp>
//#include <range/v3/view/join.hpp>
//#include <range/v3/view/partial_sum.hpp>
//#include <range/v3/view/remove_if.hpp>
//#include <range/v3/view/repeat.hpp>
//#include <range/v3/view/single.hpp>
//#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
//#include <range/v3/view/zip.hpp>
//#include <range/v3/view/zip_with.hpp>
//#include <range/v3/view_interface.hpp>

#include "oldDays.hpp"

namespace r=ranges;
namespace rv=r::view;



namespace day19 {
   std::vector<int> rr(6);
   auto ip = std::ref(rr[0]);
   
   void compile(const string &line){
      std::istringstream sin(line);
      std::string i;
      int op1, op2, op3;
      sin >> i >> op1 >> op2 >> op3;
      if (i=="addr") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")+rr.at("<<op2<<");\n";
      } else if (i=="addi") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")+"<<op2<<";\n";
      } else if (i=="mulr") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")*rr.at("<<op2<<");\n";
      } else if (i=="muli") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")*"<<op2<<";\n";
      } else if (i=="banr") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")&rr.at("<<op2<<");\n";
      } else if (i=="bani") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")&"<<op2<<";\n";
      } else if (i=="borr") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")|rr.at("<<op2<<");\n";
      } else if (i=="bori") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")|"<<op2<<";\n";
      } else if (i=="setr") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<");\n";
      } else if (i=="seti") {
         cout << "rr.at("<<op3<<") = "<<op1<<";\n";
      } else if (i=="gtir") {
         cout << "rr.at("<<op3<<") = "<<op1<<">rr.at("<<op2<<")?1:0;\n";
      } else if (i=="gtri") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")>"<<op2<<"?1:0;\n";
      } else if (i=="gtrr") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")>rr.at("<<op2<<")?1:0;\n";
      } else if (i=="eqir") {
         cout << "rr.at("<<op3<<") = "<<op1<<"==rr.at("<<op2<<")?1:0;\n";
      } else if (i=="eqri") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")=="<<op2<<"?1:0;\n";
      } else if (i=="eqrr") {
         cout << "rr.at("<<op3<<") = rr.at("<<op1<<")==rr.at("<<op2<<")?1:0;\n";
      } else throw 1;
   }
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
}


void day19stars() {
   using namespace day19;
   using std::string;

   string opcodes[] = {"addr","addi","mulr","muli","banr","bani","borr","bori","setr","seti","gtir","gtri","gtrr","eqir","eqri","eqrr"};
   
   std::ifstream fin(DIRECTORY+"day19");
   auto lines = r::getlines(fin) | r::to_vector;

   string _;
   int i;
   std::istringstream sin(lines[0]);
   sin >> _ >> i;
   ip = std::ref(rr[i]);

   rr = {1,0,0,0,0,0};
//   while (ip+1 < lines.size()) {
//      cout << "ip=" << ip << " [" << rr[0] << " " << rr[1] << " " << rr[2] << " " << rr[3] << " " << rr[4] << " " << rr[5] << "] " << lines[ip+1];
//      apply(lines[ip+1]);
//      cout << " [" << rr[0] << " " << rr[1] << " " << rr[2] << " " << rr[3] << " " << rr[4] << " " << rr[5] << "]" << endl;
//      ++ip;
//   }
//   cout << "Day 19 star 1: " << rr.at(0) << endl;
   
//   rr = {1,0,0,0,0,0};
//   while (ip+1 < lines.size()) {
//      //      cout << "ip=" << ip << " [" << rr[0] << " " << rr[1] << " " << rr[2] << " " << rr[3] << " " << rr[4] << " " << rr[5] << "] " << lines[ip+1];
//      cout << "case " << ip << ": \n\t";
//      compile(lines[ip+1]);
//      cout << "\t++ip;\n\tbreak;\n";
//      //      cout << " [" << rr[0] << " " << rr[1] << " " << rr[2] << " " << rr[3] << " " << rr[4] << " " << rr[5] << "]" << endl;
//      ++ip;
//   }

      rr = {1,0,0,0,0,0};
   while(ip+1 < lines.size()) {
//            cout << " [" << rr[0] << " " << rr[1] << " " << rr[2] << " " << rr[3] << " " << rr[4] << " " << rr[5] << "]" << endl;
   switch (ip ){
   case 0:
      rr.at(3) = rr.at(3)+16;
      ++ip;
      break;
   case 1:
      rr.at(2) = 1;
      ++ip;
      break;
   case 2:
      rr.at(4) = 1;
      ++ip;
      break;
   case 3:
      rr.at(1) = rr.at(2)*rr.at(4);
      ++ip;
      break;
   case 4:
      rr.at(1) = rr.at(1)==rr.at(5)?1:0;
      ++ip;
      break;
   case 5:
      rr.at(3) = rr.at(1)+rr.at(3);
      ++ip;
      break;
   case 6:
      rr.at(3) = rr.at(3)+1;
      ++ip;
      break;
   case 7:
      rr.at(0) = rr.at(2)+rr.at(0);
      ++ip;
      break;
   case 8:
      rr.at(4) = rr.at(4)+1;
      ++ip;
      break;
   case 9:
      rr.at(1) = rr.at(4)>rr.at(5)?1:0;
      ++ip;
      break;
   case 10:
      rr.at(3) = rr.at(3)+rr.at(1);
      ++ip;
      break;
   case 11:
      rr.at(3) = 2;
      ++ip;
      break;
   case 12:
      rr.at(2) = rr.at(2)+1;
      ++ip;
      break;
   case 13:
      rr.at(1) = rr.at(2)>rr.at(5)?1:0;
      ++ip;
      break;
   case 14:
      rr.at(3) = rr.at(1)+rr.at(3);
      ++ip;
      break;
   case 15:
      rr.at(3) = 1;
      ++ip;
      break;
   case 16:
      rr.at(3) = rr.at(3)*rr.at(3);
      ++ip;
      break;
   case 17:
      rr.at(5) = rr.at(5)+2;
      ++ip;
      break;
   case 18:
      rr.at(5) = rr.at(5)*rr.at(5);
      ++ip;
      break;
   case 19:
      rr.at(5) = rr.at(3)*rr.at(5);
      ++ip;
      break;
   case 20:
      rr.at(5) = rr.at(5)*11;
      ++ip;
      break;
   case 21:
      rr.at(1) = rr.at(1)+3;
      ++ip;
      break;
   case 22:
      rr.at(1) = rr.at(1)*rr.at(3);
      ++ip;
      break;
   case 23:
      rr.at(1) = rr.at(1)+12;
      ++ip;
      break;
   case 24:
      rr.at(5) = rr.at(5)+rr.at(1);
      ++ip;
      break;
   case 25:
      rr.at(3) = rr.at(3)+rr.at(0);
      ++ip;
      break;
   case 26:
      rr.at(3) = 0;
      ++ip;
      break;
   case 27:
      rr.at(1) = rr.at(3);
      ++ip;
      break;
   case 28:
      rr.at(1) = rr.at(1)*rr.at(3);
      ++ip;
      break;
   case 29:
      rr.at(1) = rr.at(3)+rr.at(1);
      ++ip;
      break;
   case 30:
      rr.at(1) = rr.at(3)*rr.at(1);
      ++ip;
      break;
   case 31:
      rr.at(1) = rr.at(1)*14;
      ++ip;
      break;
   case 32:
      rr.at(1) = rr.at(1)*rr.at(3);
      ++ip;
      break;
   case 33:
      rr.at(5) = rr.at(5)+rr.at(1);
      ++ip;
      break;
   case 34:
      rr.at(0) = 0;
      ++ip;
      break;
   case 35:
      rr.at(3) = 0;
      ++ip;
      break;
   }}
   cout << "Day 19 star 2: " << rr.at(0) << endl;
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
//   day15stars();
//   day16stars();
//   day17stars();
//   day18stars();
   day19stars();
   return 0;
}
