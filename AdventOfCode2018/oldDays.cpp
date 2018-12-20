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
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::get;

#include <range/v3/action/transform.hpp>
#include <range/v3/action/remove_if.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/max.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/min.hpp>
#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/algorithm/minmax.hpp>
#include <range/v3/algorithm/minmax_element.hpp>
#include <range/v3/algorithm/reverse.hpp>
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
#include <range/v3/view/concat.hpp>
#include <range/v3/view/cycle.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/for_each.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/partial_sum.hpp>
#include <range/v3/view/remove_if.hpp>
#include <range/v3/view/repeat.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>
#include <range/v3/view/zip_with.hpp>
#include <range/v3/view_interface.hpp>

//FWD and LIFT are from TartanLLama's blog
// at https://blog.tartanllama.xyz/passing-overload-sets/

#define FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define LIFT(X) [](auto &&... args) \
noexcept(noexcept(X(FWD(args)...)))  \
-> decltype(X(FWD(args)...)) \
{  \
return X(FWD(args)...); \
}

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
         return {};
      }
      if(sleep == -1) {
         sleep = rec.minute;
         return {};
      }
      Nap n{guard,sleep,rec.minute};
      sleep = -1;
      return rv::single(n);
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

void day6stars() {
   
   auto strToPair = [](const string &s) {
      std::istringstream sin(s);
      char _;
      int x,y;
      sin >> x >> _ >> y;
      return std::make_tuple(x,y);
   };
   
   std::ifstream fin(DIRECTORY+"day6");
   const auto coords = r::getlines(fin) | rv::transform(strToPair) | r::to_vector;
   
   auto [minXi,maxXi] = r::minmax_element(coords,std::less<>(),LIFT(std::get<0>));
   const auto minX = std::get<0>(*minXi);
   const auto maxX = std::get<0>(*maxXi);
   auto [minYi,maxYi] = r::minmax_element(coords,std::less<>(),[](auto t){return std::get<1>(t);});
   const auto minY = std::get<1>(*minYi);
   const auto maxY = std::get<1>(*maxYi);
   //   cout << "x: " << minX << "," << maxX << "   y: " << minY << "," << maxY <<  endl;
   
   const auto locations = rv::cartesian_product(rv::ints(minX,maxX),rv::ints(minY,maxY));
   const auto border = rv::concat(rv::cartesian_product(rv::ints(minX,maxX),rv::single(minY)),
                                  rv::cartesian_product(rv::ints(minX,maxX),rv::single(maxY)),
                                  rv::cartesian_product(rv::single(minX),rv::ints(minY+1,maxY-1)),
                                  rv::cartesian_product(rv::single(maxX),rv::ints(minY+1,maxY-1)));
   
   auto dist = [](auto c,auto l) {
      return abs(std::get<0>(c)-std::get<0>(l))+abs(std::get<1>(c)-std::get<1>(l));
   };
   
   auto indexOfMinIfUnique = [coords,dist](auto l) {
      auto checkNext = [l,&coords,dist] (auto && minp, const auto &c) -> auto & {
         if(dist(c,l)<dist(coords[minp.first],l))
            return minp = {&c-&coords.front(),true};
         if(dist(c,l)==dist(coords[minp.first],l))
            return minp = {minp.first,false};
         return minp;
      };
      
      auto [indexOfMin,unique] = r::accumulate(coords | rv::drop(1),std::make_pair(0,true),checkNext);
      return rv::repeat_n(indexOfMin,unique?1:0);
   };
   
   auto registerArea = [](auto &&areas, auto coordIndex) -> auto & {
      areas[coordIndex]++;
      return areas;
   };
   
   auto closestCoords = locations | rv::for_each(indexOfMinIfUnique);

   auto areas = r::accumulate(closestCoords,std::vector<int>(coords.size()),registerArea);
   
   auto borderClosestCoords = border | rv::for_each(indexOfMinIfUnique);
   
   auto borderAreas = r::accumulate(borderClosestCoords,
                                    std::vector<int>(coords.size()),
                                    registerArea);
   
   auto finiteAreas = rv::zip(areas,borderAreas) |
      rv::remove_if([](auto p){return p.second > 0;}) |
      rv::transform([](auto p){return std::get<0>(p);});
   
   cout << "Day 6 star 1: " << *r::max_element(finiteAreas) << endl;
   
   auto sumOfDistances = [coords,dist](const auto &l){
      return r::accumulate(rv::transform(coords,[dist,&l](const auto &c){return dist(c,l);}),0);
   };
   
   auto goodLocations = r::count_if(locations | rv::transform(sumOfDistances),
                                    [](auto d){return  d<10000;});
   
   cout << "Day 6 star 2: " << goodLocations << endl;
}

void day7stars() {
   
   auto strToEdge = [](const string &s) {
      std::istringstream sin(s);
      string _s;
      char from;
      char to;
      //   Step     X     must    be finished before step  P   can begin.
      sin >> _s >> from >> _s  >> _s >> _s >> _s >> _s >> to;
      return std::make_pair(from-'A',to-'A');
   };
   
   std::ifstream fin(DIRECTORY+"day7");
   const auto edges = r::getlines(fin) | rv::transform(strToEdge) | r::to_vector;
   
   std::vector<std::vector<int>> edgelists(26);
   std::vector<int> inEdges(26);
   
   for (auto e:edges)
      edgelists[e.first].push_back(e.second);
   
   for (auto e:edges)
      ++inEdges[e.second];
   
   std::priority_queue<int,std::vector<int>,std::greater<>> jobs;
   for(int ii=0;ii<26;++ii)
      if(inEdges[ii]==0)
         jobs.push(ii);
   
   std::string order;
   
   while(!jobs.empty()) {
      auto me = jobs.top();
      jobs.pop();
      order += me+'A';
      for(auto o:edgelists[me]) {
         --inEdges[o];
         if(inEdges[o]==0)
            jobs.push(o);
      }
   }
   
   cout << "Day 7 star 1: " << order << endl;
   
   for (auto e:edges)
      ++inEdges[e.second];
   
   std::priority_queue<int,std::vector<int>,std::greater<>> readyJobs;
   for(int ii=0;ii<26;++ii)
      if(inEdges[ii]==0)
         readyJobs.push(ii);
   
   // (timeComplete, jobNum)
   std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<>> runningJobs;
   int busyWorkers=0;
   
   int curTime = 0;
   do {
      while(!readyJobs.empty() && busyWorkers < 4) {
         auto newJob = readyJobs.top();
         readyJobs.pop();
         ++busyWorkers;
         cout << "Time " << curTime << " assigned worker " << busyWorkers
         << " to job " << char(newJob+'A') << " which will finish at " <<curTime+61+newJob<<endl;
         runningJobs.push(std::make_pair(curTime+61+newJob,newJob));
      }
      auto completedJob = runningJobs.top();
      runningJobs.pop();
      --busyWorkers;
      curTime = completedJob.first;
      cout << "Time " << curTime << " Job " <<char('A'+completedJob.second) << " complete. Busy workers: "
      << busyWorkers << endl;
      for(auto o:edgelists[completedJob.second]) {
         --inEdges[o];
         if(inEdges[o]==0) {
            readyJobs.push(o);
            cout << "Job " << char(o+'A') << " is now ready.\n";
         }
      }
   } while(!runningJobs.empty() || !readyJobs.empty());
   cout << "Day 7 star 2: " << curTime << endl;
}

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

void day9stars() {
   
   class MarbleRing {
      struct Node {
         unsigned long long prev,next;
      };
   public:
      MarbleRing(int players, int marbles):_scores(players),_marbles(marbles+1){
         _marbles[0]={0,0};
         auto curMarble = 0ull;
         auto startMarble = 0ull;
         for(int ii=1;ii<=marbles;++ii) {
            if (ii%23 != 0) {
               curMarble = _marbles[curMarble].next;
               _marbles[ii].prev = curMarble;
               _marbles[ii].next = _marbles[curMarble].next;
               _marbles[_marbles[ii].next].prev = ii;
               _marbles[_marbles[ii].prev].next = ii;
               assert(_marbles[curMarble].next == ii);
               curMarble = ii;
            } else {
               for(int p=0;p<7;++p)
                  curMarble = _marbles[curMarble].prev;
               if(curMarble==startMarble)
                  startMarble = _marbles[curMarble].next;
               _scores[ii%players] += ii+curMarble;
               _marbles[_marbles[curMarble].next].prev = _marbles[curMarble].prev;
               _marbles[_marbles[curMarble].prev].next = _marbles[curMarble].next;
               curMarble = _marbles[curMarble].next;
            }
            //            cout << "(" << ii%players << ") " << startMarble << " ";
            //            auto m = _marbles[startMarble].next;
            //            while (m != startMarble) {
            //               if (m==curMarble) cout << "+";
            //               cout << m << " ";
            //               m =_marbles[m].next;
            //            }
            //            cout << "\n";
         }
         //         cout << startMarble << ", ";
         //         auto m = _marbles[startMarble].next;
         //         while (m != startMarble) {
         //            cout << m << ", ";
         //            m =_marbles[m].next;
         //         }
         //         cout << "\n";
      }
      
      const std::vector<unsigned long long> & scores() {return _scores;}
      
   private:
      std::vector<Node> _marbles;
      std::vector<unsigned long long> _scores;
   };
   
   //   MarbleRing m(10,1618);
   MarbleRing m(493,7186300);
   //   MarbleRing m(9,50);
   //   MarbleRing m(17,1104);
   auto highScore = *r::max_element(m.scores());
   cout << "Day 9 star 1: " << highScore << "\n";
   
}

void day10stars() {
   struct Point {
      int x, y, dx, dy; };
   
   auto stringToPoint = [](const string &s) {
      //position=<-20655, -20679> velocity=< 2,  2>
      std::istringstream sin(s);
      Point p;
      char _;
      sin.ignore(10);
      sin >> p.x >> _ >> p.y;
      sin.ignore(12);
      sin >> p.dx >> _ >> p.dy;
      return p;
   };
   
   std::ifstream fin(DIRECTORY+"day10");
   auto points = r::getlines(fin) | rv::transform(stringToPoint) | r::to_vector;
   auto prevMinY = r::min_element(points, std::less<>() ,&Point::y)->y;
   auto count =0;
   while(true) {
      auto nextGen{points};
      nextGen |= r::action::transform([](Point &p){p.x += p.dx; p.y += p.dy; return p;});
      auto newMinY = r::min_element(nextGen, std::less<>() ,&Point::y)->y;
      
      if (prevMinY > newMinY) {
         auto minX = r::min(points, std::less<>(), &Point::x).x;
         auto maxX = r::max(points, std::less<>(), &Point::x).x;
         auto minY = r::min(points, std::less<>(), &Point::y).y;
         auto maxY = r::max(points, std::less<>(), &Point::y).y;
         cout << minX << maxX << endl;
         auto plotPoints = [minX,minY](auto & plot, auto c) -> auto & {
            plot[c.y-minY][c.x-minX] = '*';
            return plot;
         };
         std::vector<std::vector<char>> plot(maxY-minY+1,std::vector<char>(maxX-minX+1,' '));
         plot = r::accumulate(points,plot,plotPoints);
         cout << "Day 10 star 1:\n";
         for(const auto &row:plot) {
            for(auto c:row)
               cout << c;
            cout << endl;
         }
         cout << "Day 10 star 2: " << count <<"\n";// << ": " << minX << "," << maxX << "   " << minY << "," << maxY << "\n";
         break;
      }
      prevMinY = newMinY;
      points.swap(nextGen);
      ++count;
   }
}

void day11stars() {
   //   Find the fuel cell's rack ID, which is its X coordinate plus 10.
   //   Begin with a power level of the rack ID times the Y coordinate.
   //   Increase the power level by the value of the grid serial number (your puzzle input).
   //   Set the power level to itself multiplied by the rack ID.
   //   Keep only the hundreds digit of the power level (so 12345 becomes 3; numbers with no hundreds digit become 0).
   //   Subtract 5 from the power level.
   const int sn = 3214;
   std::vector<std::vector<int>> powers;
   const int ySize=300;
   const int xSize=300;
   
   for(int y=0;y<ySize;++y) {
      powers.push_back({});
      for(int x=0;x<xSize;++x) {
         auto xc=x+1;
         auto yc=y+1;
         //   Find the fuel cell's rack ID, which is its X coordinate plus 10.
         auto rackId = xc+10;
         //   Begin with a power level of the rack ID times the Y coordinate.
         auto pl = rackId*yc;
         //   Increase the power level by the value of the grid serial number (your puzzle input).
         pl += sn;
         //   Set the power level to itself multiplied by the rack ID.
         pl *= rackId;
         //   Keep only the hundreds digit of the power level (so 12345 becomes 3; numbers with no hundreds digit become 0).
         pl = pl/100%10;
         //   Subtract 5 from the power level.
         pl -= 5;
         powers.back().push_back(pl);
         assert(powers.size()==yc && powers.back().size()==xc);
      }
   }
   
   auto max=0;
   std::string output;
   for(int filterSize = 1; filterSize<=300; ++filterSize) {
      auto totalPower = [filterSize](const auto &p, int x, int y) {
         auto sum=0;
         for(int ii=0;ii<filterSize;++ii)
            for(int jj=0;jj<filterSize;++jj)
               sum += p[y+ii][x+jj];
         return sum;
      };
      
      std::vector<std::vector<int>> gridPowers(ySize-filterSize+1,std::vector<int>(xSize-filterSize+1));
      
      for(int y=0;y<=ySize-filterSize;++y)
         for(int x=0;x<=xSize-filterSize;++x)
            gridPowers[y][x]=totalPower(powers,x,y);
      
      auto coords = rv::cartesian_product(rv::ints(1,ySize-filterSize+2),rv::ints(1,xSize-filterSize+2));
      auto elements = rv::zip(coords, gridPowers | rv::join) | r::to_vector;
      auto maxPos = r::max_element(elements,[](auto &x, auto &y) {return get<1>(x) < get<1>(y);});
      if (maxPos->second > max) {
         max = maxPos->second;
         output = std::to_string(get<1>(maxPos->first)) + "," +
         std::to_string(get<0>(maxPos->first)) + "," + std::to_string(filterSize);
      }
      if (filterSize == 3)
         cout << "Day 11 star 1: " << output << "\n";
   }
   cout << "Day 11 star 2: " << output <<"\n";
}

void day12stars() {
   // This is still a mess and should be cleaned up
   
   std::ifstream fin(DIRECTORY+"day12");
   string _;
   string state;
   fin >> _ >> _ >> state;
   
   fin.ignore();
   fin.ignore();
   
   auto notes = r::getlines(fin) | r::to_vector;
   //   r::sort(notes);
   auto updateNumByPot = [](int &n, char p) {
      n = (2*n+(p=='#'?1:0))%32;
   };
   
   auto fivePotsToNum = [updateNumByPot](std::string_view s) {
      int n=0;
      for(auto c:s)
         updateNumByPot(n,c);
      return n;
   };
   
   std::vector<char> rules(32);
   for(const auto &s : notes) {
      std::istringstream sin(s);
      std::string rule;
      char result;
      sin >> rule >> _ >> result;
      rules[fivePotsToNum(rule)]=result;
   }
   
   //   const auto generations=50'000'000'000ull;
   const auto generations=100ull;
   //      const auto generations=2000000ull;
   std::string pots(generations+state.size()+generations+2,'.');
   r::copy(state,pots.begin()+generations);
   cout << "Initial plants built." << endl;
   auto sm2=0ll;
   auto sm1=0ll;
   for(auto g=0ll;g<generations;++g) {
      auto num=0;
      auto start = generations-g;
      auto stop = start + 2*g + state.size()+3;
      updateNumByPot(num,pots[start]);
      updateNumByPot(num,pots[start+1]);
      for(auto p=start+2ull;p<stop;++p) {
         updateNumByPot(num,pots[p]);
         pots[p-2] = rules[num];
      }
      auto sum=0ll;
      for(auto p=0ll;p<pots.size();++p) {
         if (pots[p]=='#')
            sum += p-generations;
      }
      if(g==20-1)
         cout << "Day 12 star 1: " << sum << "\n";
      if(sum-sm1==sm1-sm2) {
         cout << "Day 12 star 2: " << sum+(50000000000ll-g-1)*(sum-sm1) << "\n";
         break;
         }
      sm2=sm1;
      sm1=sum;
   }
}

namespace day13 {
   std::vector<std::string> tracks;
   std::vector<std::string> cartMap;
   bool firstCollision = true;
   
   class Cart {
      friend bool operator<(const Cart &lhs, const Cart &rhs) {
         return std::tie(lhs._y,lhs._x) < std::tie(rhs._y,rhs._x);
      }

   public:
      Cart(int x,int y,int direction):_x(x),_y(y),_direction(direction)
      {}
      bool gone() const {
         return _gone;
      }
      std::string positionStr() const {
         return std::to_string(_x) + "," + std::to_string(_y);
      }
      void goneIfCrashed(const Cart &c) {
         if(_x==c._x && _y == c._y) {
            cartMap[_y][_x] = ' ';
            _gone = true;
         }
      }
      bool update() {
         if (_gone)
            return false;
         cartMap[_y][_x] = ' ';
         _x += directions[_direction].first;
         _y += directions[_direction].second;
         if (cartMap[_y][_x] != ' ') {
            if (firstCollision) {
               cout << "Day 13 star 1: " << _x << "," << _y << "\n";
               firstCollision = false;
            }
            return true;
         }
         switch (tracks[_y][_x]) {
            case '/':
               _direction ^= 1; ////swap U<->R and D<->L
               break;
            case '\\':
               _direction = 3-_direction; //swap U<->L and D<->R
               break;
            case '+':
               _direction = (_direction+turnChoices[_turnStatus])%4;
               _turnStatus = (_turnStatus+1)%3;
               break;
            case ' ':
               break;
            default:
               throw std::runtime_error("Cart::update() moved cart onto unknown track character");
         }
         cartMap[_y][_x] = symbols[_direction];
         return false;
      }
   private:
      int _x, _y;
      int _direction;
      int _turnStatus=0;
      bool _gone=false;
      static constexpr std::pair<int,int> directions[4] = {{0,-1},{1,0},{0,1},{-1,0}};//URDL
      static constexpr int turnChoices[3]={3,0,1};//Left, straight, right
      static constexpr char symbols[4] = {'^','>','v','<'};
   };
}

void day13stars() {
   using namespace day13;
   std::ifstream fin(DIRECTORY+"day13");
   tracks = r::getlines(fin) | r::to_vector;
   
   std::vector<Cart> carts;
   for(int y=0;y<tracks.size();++y) {
      cartMap.push_back({});
      for(int x=0;x<tracks[y].size();++x) {
         switch (tracks[y][x]) {
            case '^':
               carts.push_back({x,y,0});
               cartMap.back().push_back('^');
               tracks[y][x]=' ';
               break;
            case '>':
               carts.push_back({x,y,1});
               cartMap.back().push_back('>');
               tracks[y][x]=' ';
               break;
            case 'v':
               carts.push_back({x,y,2});
               cartMap.back().push_back('v');
               tracks[y][x]=' ';
               break;
            case '<':
               carts.push_back({x,y,3});
               cartMap.back().push_back('<');
               tracks[y][x]=' ';
               break;
            case '/':
            case '\\':
            case '+':
            case ' ':
               cartMap.back().push_back(' ');
               break;
            case '-':
            case '|':
               cartMap.back().push_back(' ');
               tracks[y][x]=' ';
               break;
            default:
               throw std::runtime_error("day13stars: unknown symbol in track map ["+std::string(1,tracks[y][x])+"]");
         }
      }
   }
   while(true) {
      std::sort(carts.begin(),carts.end());
      for(auto &c:carts)
         if(c.update())
            for(auto &c2:carts)
               c2.goneIfCrashed(c);
      //      cout << carts.size() << " ";
      carts |= r::action::remove_if(&Cart::gone);
      //      cout << carts.size() << "\n";
      if (carts.size() == 1) {
         cout << "Day 13 star 2: " << carts.front().positionStr() << "\n";
         return;
      }
   }
}

void day14stars() {
   using std::string;
   using std::to_string;
   
   size_t elf1=0;
   size_t elf2=1;
   string recipies{"37"};
   auto asNum = [](auto c){return c-'0';};
   const int input=209231;
   const string inputString = std::to_string(input);
   auto tailSize = inputString.size()+1;
   while(recipies.size() < input+10 ||
         recipies.find(inputString,recipies.size()-tailSize) == string::npos) {
      recipies.append(to_string(asNum(recipies[elf1])+asNum(recipies[elf2])));
      elf1 = (elf1 + 1 + asNum(recipies[elf1]))%recipies.size();
      elf2 = (elf2 + 1 + asNum(recipies[elf2]))%recipies.size();
   }
   cout << "Day 14 star 1: " << recipies.substr(input,10) << "\n";
   cout << "Day 14 star 2: " << recipies.find(std::to_string(input)) << "\n";
}



namespace day16 {
   // This is a HUGE mess as I was going for speed. Got 277 for star 1, then
   // had a horrible bug that took me hours for star 2.
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
}

void day16stars() {
   using namespace day16;
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

void day17stars() {
   struct Vein {
      int xLeft, xRight, yTop, yBottom;
      static Vein lineToVein (const std::string &line) {
         std::istringstream sin(line);
         char single,_;
         int s1,d1,d2;
         sin >> single >> _ >> s1 >> _ >> _ >> _ >> d1 >> _ >> _ >> d2;
         return single=='x' ? Vein{s1,s1,d1,d2} : Vein{d1,d2,s1,s1};
      }
   };
   
   std::ifstream fin(DIRECTORY+"day17");
   std::vector<Vein> veins = r::getlines(fin) | rv::transform(Vein::lineToVein);
   
   class Scan {
      struct Position {
         int x, y;
         Position below() {return {x,  y+1};}
         Position left()  {return {x-1,y  };}
         Position right() {return {x+1,y  };}
      };
      
   public:
      Scan(const std::vector<Vein>& veins) {
         // may need to allow water flow outside of leftmost and rightmost veins
         auto _minX = r::min(veins, std::less<>(), &Vein::xLeft).xLeft-1;
         auto _maxX = r::max(veins, std::less<>(), &Vein::xRight).xRight+1;
         auto _minY = r::min(veins, std::less<>(), &Vein::yTop).yTop;
         auto _maxY = r::max(veins, std::less<>(), &Vein::yBottom).yBottom;
         _data = std::vector<std::string>(_maxY-_minY+1,std::string(_maxX-_minX+1,'.'));
         for(const auto &v:veins)
            for(int y=v.yTop;y<=v.yBottom;++y)
               for(int x=v.xLeft;x<=v.xRight;++x)
                  _data[y-_minY][x-_minX] = '#';
         dropFrom({500-_minX,0});
         cout << "Day 17 Star 1: " << _damp << ", Star 2: " << _wet << "\n";
      }
      
      const char at(Position p) {
         return _data[p.y][p.x];
      }
      
      void make(Position p, char c) {
         _data[p.y][p.x] = c;
         if (c=='|') ++_damp;
         else if (c=='~') ++_wet;
      }
      
      void print() {
         for(const auto &row : _data)
            cout << row << "\n";
      }
      
      // the following bool functions return true if water can escape or join other damp areas
      bool dropFrom(Position p) {
         if (at(p) == '|')
            return true;
         if (at(p) != '.')
            return false;
         make(p,'|');
         return p.y == _data.size()-1 || dropFrom(p.below()) || fillFrom(p);
      }
      
      // dir will be Position::left() or Position::right()
      bool spread(Position p, Position (Position::*dir)()) {
         if (at(p) == '|')
            return true;
         if (at(p) != '.')
            return false;
         make(p,'|');
         return dropFrom(p.below()) || spread((p.*dir)(),dir);
      }

      bool fillFrom(Position p) {
         auto leftFlood = spread(p.left(),&Position::left);
         auto rightFlood = spread(p.right(),&Position::right);
         if (!leftFlood && !rightFlood) {
            while (at(p.left()) == '|')
               p = p.left();
            for ( ; at(p) == '|'; p=p.right())
               make(p,'~');
         }
         return leftFlood || rightFlood;
      }

   private:
      std::vector<std::string> _data;
      size_t _wet=0;
      size_t _damp=0;
   };
   
   Scan s{veins};
}

namespace day18 {
   class Lumberyard {
   public:
      Lumberyard(std::vector<std::string> yard):_yard(std::move(yard))
      {}
      void run() {
         std::map<int,unsigned long long> lastSeen;
         auto gen=0;
         auto timesAgo=-1;
         int streakLength=0;
         int target = -1;
         while(true) {
            update();
            ++gen;
            auto v = value();
            
            if (gen==10)
               cout << "Day 18 star 1: " << v << "\n";
            if (gen%timesAgo==target) {
               cout << "Day 18 star 2: " << v << "\n";
               return;
            }
            if (lastSeen.count(v)) {
               if(gen-lastSeen[v] != timesAgo) {
                  timesAgo = int(gen-lastSeen[v]);
                  streakLength = 1;
               } else {
                  ++streakLength;
                  if (streakLength == timesAgo)
                     target = 1000000000ull%timesAgo;
               }
            }
            else {
               timesAgo = -1;
               streakLength = 0;
            }
            
            lastSeen[v] = gen;
         }
      }
   private:
      void update() {
         auto yard2=_yard;
         for(int y=0;y<yard2.size();++y)
            for(int x=0;x<yard2[y].size();++x)
               switch (yard2[y][x]) {
                  case '.' :
                     if (count(x,y,'|') >= 3)
                        yard2[y][x] = '|';
                     break;
                  case '|' :
                     if (count(x,y,'#') >= 3)
                        yard2[y][x] = '#';
                     break;
                  case '#' :
                     if (count(x,y,'#') < 1 || count(x,y,'|') < 1)
                        yard2[y][x] = '.';
                     break;
               }
         _yard.swap(yard2);
      }
      
      int value() {
         auto trees=0;
         auto lumber=0;
         for(int y=0;y<_yard.size();++y)
            for(int x=0;x<_yard[y].size();++x) {
               if (_yard[y][x]=='|')
                  ++trees;
               if (_yard[y][x]=='#')
                  ++lumber;
            }
         return trees*lumber;
      }
      
      char at(int x, int y) const {
         if(y<0 || y>=_yard.size() || x<0 || x>=_yard[y].size())
            return '\0';
         return _yard[y][x];
      }
      
      int count(int x, int y, char c) {
         return  (at(x-1,y-1)==c)
         + (at(x-1,y  )==c)
         + (at(x-1,y+1)==c)
         + (at(x  ,y-1)==c)
         + (at(x  ,y+1)==c)
         + (at(x+1,y-1)==c)
         + (at(x+1,y  )==c)
         + (at(x+1,y+1)==c);
      }
      std::vector<std::string> _yard;
   };
}

void day18stars()
{
   using namespace day18;
   std::ifstream fin(DIRECTORY+"day18");
   std::vector<std::string> yard = r::getlines(fin);
   Lumberyard l{yard};
   l.run();
}

namespace day15 {
   struct Point {//we blur distinction between point and vector
      int x,y;
      Point operator+(const Point &rhs) {return {x+rhs.x,y+rhs.y};}
      bool operator==(const Point &rhs) {return x==rhs.x && y==rhs.y;}
      std::string to_string() const {
         return "["+std::to_string(x)+","+std::to_string(y)+"]";
      }
      int dist(const Point& p) const {return abs(x-p.x)+abs(y-p.y);}
   };
   
   class Unit {
      friend class Cave;
      friend bool operator<(const Unit &lhs, const Unit &rhs) {
         return std::tie(lhs._loc.y,lhs._loc.x)<std::tie(rhs._loc.y,rhs._loc.x);
      }
   public:
      Unit(int x, int y, char type) :_loc{x,y},_type(type)
      {}
      
      std::string to_string() const {
         return _hp<1 ? std::string{}
         : _type + ("@" + _loc.to_string() +"(" + std::to_string(_hp) + ")");
      }
      
   private:
      char other() const {
         if(_type == 'E') return 'G';
         else if (_type == 'G') return 'E';
         throw 1;
      }
      
      bool isTargetOf(char t, Point loc) const {
         return _hp>0 && _loc.dist(loc)==1 && t == other();
      }
      
      Point _loc;
      char _type;
      int _hp=200;
   };
   
   class Cave {
   public:
      Cave(const std::vector<std::string> &cave,int elfAttack)
      :_cave(cave),_elfAttack(elfAttack) {
         for(int y=0;y<cave.size();++y)
            for(int x=0;x<cave[y].size();++x)
               if (auto s = cave[y][x]; s == 'E' || s == 'G')
                  _units.emplace_back(x,y,s);
         //      print();
      }
      
      void print() const {
         for(const auto &l:_cave)
            cout << l << endl;
         
         for(auto &unit : _units)
            cout << unit.to_string() << " ";
         cout << endl;
      }
      
      bool doRound() {
         std::sort(begin(_units),end(_units));
         for(Unit &me:_units) {
            if (me._hp < 1)
               continue;
            if(!hasOpponents(me._type))
               return true;
            doMovement(me);
            doAttack(me);
         }
         
         std::sort(begin(_units),end(_units));
         return false;
      }
      bool deadElves() {
         return r::any_of(_units,[](auto &u){return u._type == 'E' && u._hp < 1;});
      }
      int score() {
         return r::accumulate(_units,0,[](int i, const Unit &u){return i+(u._hp>0?u._hp:0);});
      }
   private:
      
      void doMovement(Unit& me) {
         if (hasTargets(me._type,me._loc))
            return;
         
         std::vector<Point> dirs{{0,-1},{-1,0},{1,0},{0,1}};
         
         struct Info {
            Point firstMove,pos;
            int length;
            bool operator<(const Info &r) const {
               return std::tie(length,pos.y,pos.x,firstMove.y,firstMove.x) < std::tie(r.length,r.pos.y,r.pos.x,r.firstMove.y,r.firstMove.x);
            }
            bool operator==(const Info &r) const {
               return std::tie(length,pos.y,pos.x,firstMove.y,firstMove.x) == std::tie(r.length,r.pos.y,r.pos.x,r.firstMove.y,r.firstMove.x);
            }
         };
         
         std::queue<Info> info;
         
         std::set<std::pair<int,int>> visited;
         for(const auto &d:dirs) {
            auto pos=me._loc+d;
            if(at(pos) == '.') {
               info.push(Info{d,pos,1});
               visited.insert({pos.x,pos.y});
            }
         }
         
         //      bool show=false;
         
         std::vector<Info> goodPaths;
         while(!info.empty()) {
            //         if(show) {
            //            auto i=info;
            //            while(!i.empty()) {
            //               auto x = i.front();
            //               i.pop();
            //               cout << x.firstMove.to_string() << " "
            //               << x.pos.to_string() << "\n";
            //            }
            //            cout << "-----------------------------------\n";
            //         }
            auto spot = info.front();
            info.pop();
            if (hasTargets(me._type,spot.pos)) {
               goodPaths.push_back(spot);
               continue;
            }
            for(const auto &d:dirs) {
               Point next = spot.pos + d;
               if (!visited.count({next.x,next.y}) && at(next) == '.') {
                  info.push(Info{spot.firstMove,next,spot.length+1});
                  visited.insert({next.x,next.y});
               }
            }
         }
         
         if(goodPaths.empty())
            return;
         //      auto first = goodPaths.front();
         std::sort(goodPaths.begin(),goodPaths.end());
         //      if (!(first == goodPaths.front())) {
         //         print();
         //         cout << "Error moving " << me.to_string() << "\n";
         //         cout << "***Good:" << first.firstMove.to_string() << " " << first.pos.to_string() << "\n";
         //         cout << "*** Bad:" << goodPaths.front().firstMove.to_string() << " " << goodPaths.front().pos.to_string() << "\n";
         //         exit(1);
         //      }
         assert(at(me._loc)==me._type);
         assert(me._hp > 0);
         at(me._loc) = '.';
         me._loc = me._loc + goodPaths.front().firstMove;
         at(me._loc) = me._type;
      }
      
      void doAttack(Unit const &me) {
         auto targets = _units
         | rv::transform([](Unit &u){return std::ref(u);})
         | rv::filter([&me](const Unit &u){return u.isTargetOf(me._type, me._loc);})
         | r::to_vector;
         
         if(!targets.empty()) {
            r::sort(targets,std::less<>(),&Unit::_hp);
            auto &target = targets.front().get();
            assert(me._loc.dist(target._loc)==1);
            assert(me.other() == target._type);
            assert(target._hp >0);
            target._hp -= (me._type == 'E' ? _elfAttack : 3);
            if (target._hp <= 0)
               at(target._loc) = '.';
         }
      }
      
      bool hasTargets(char type, Point loc) {
         auto targets = _units
         | rv::filter([type,loc](const Unit &u){
            return u.isTargetOf(type,loc);});
         return !targets.empty();
      }
      bool hasOpponents(char type) {
         auto targets = _units
         | rv::filter([type](const Unit &u){return u._hp>0 && type==u.other();});
         return !targets.empty();
      }
      char & at(Point loc)  {return _cave[loc.y][loc.x];}
      std::vector<std::string> _cave;
      std::vector<Unit> _units;
      int _elfAttack;
   };
}

void day15stars() {
   using namespace day15;
   Unit u{1,5,'G'};
   std::ifstream fin(DIRECTORY+"day15");
   //2 27730
   //3 36334
   //4 39514
   //5 27755
   //6 28944
   //7 18740
   //8 Elf moves right (https://www.reddit.com/r/adventofcode/comments/a7fhax/day_15_can_anyone_give_me_a_hint/)
   //9 Goblin moves down ^^^
   std::vector<std::string> cave = r::getlines(fin);
   
   Cave cavern(cave,3);
   int i=0;
   while(!cavern.doRound()) {
      ++i;
      //      cout << "After round " << ++i << "\n";
      //      cavern.print();
   }
   cout << "Day 15 star 1: " << cavern.score() << "*" << i << " = " << cavern.score()*i << "\n";
   
   for(int e=4;;++e) {
      Cave cavern(cave,e);
      int i=0;
      while(!cavern.doRound()) {
         ++i;
         //      cout << "After round " << ++i << "\n";
         //      cavern.print();
      }
      if (!cavern.deadElves()) {
         cout << "Day 15 star 2: " << cavern.score() << "*" << i << " = " << cavern.score()*i << "\n";
         return;
      }
   }
}

namespace day19 {
   std::vector<int> rr(6);
   auto ip = std::ref(rr[0]);
   
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
   
   rr = {0,0,0,0,0,0};
   while (ip+1 < lines.size()) {
      //      cout << "ip=" << ip << " [" << rr[0] << " " << rr[1] << " " << rr[2] << " " << rr[3] << " " << rr[4] << " " << rr[5] << "] " << lines[ip+1];
      apply(lines[ip+1]);
      //      cout << " [" << rr[0] << " " << rr[1] << " " << rr[2] << " " << rr[3] << " " << rr[4] << " " << rr[5] << "]" << endl;
      ++ip;
   }
   cout << "Day 19 star 1: " << rr.at(0) << endl;
   
   rr = {1,0,0,0,0,0};
   int instructions=0;
   while (instructions<100) {
      apply(lines[ip+1]);
      ++ip;
      ++instructions;
   }
   
   auto number = r::max(rr);
   int sum=0;
   for(int i=1;i<=number;++i)
      if(number%i==0)
         sum += i;
   cout << "Day 19 star 2: " << sum << endl;
}
