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

#include <range/v3/action/transform.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/min_element.hpp>
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
   
   auto closestCoords = locations | rv::for_each(indexOfMinIfUnique);
   
   auto registerArea = [](auto &&areas, auto coordIndex) -> auto & {
      areas[coordIndex]++;
      return areas;
   };
   
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
         auto minX = r::min_element(points, std::less<>(), &Point::x)->x;
         auto maxX = r::max_element(points, std::less<>(), &Point::x)->x;
         auto minY = r::min_element(points, std::less<>(), &Point::y)->y;
         auto maxY = r::max_element(points, std::less<>(), &Point::y)->y;
         cout << count << ": " << minX << "," << maxX << "   " << minY << "," << maxY << "\n";
         auto plotPoints = [minX,minY](auto & plot, auto c) -> auto & {
            plot[c.y-minY][c.x-minX] = '*';
            return plot;
         };
         std::vector<std::vector<char>> plot(maxY-minY+1,std::vector<char>(maxX-minX+1,' '));
         plot = r::accumulate(points,plot,plotPoints);
         for(const auto &row:plot) {
            for(auto c:row)
               cout << c;
            cout << endl;
         }
         break;
      }
      prevMinY = newMinY;
      points.swap(nextGen);
      ++count;
   }
}
