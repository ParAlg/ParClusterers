#pragma once

#include <iostream>
#include <fstream> 
#include <math.h>
#include <atomic>
#include <tuple>
#include "point.h"
#include "parlay/utilities.h"
#include "utils.h"

#define SIZE_T_MAX std::numeric_limits<size_t>::max()
#define NO_NEIGH -1


using namespace std;
namespace research_graph {
namespace in_memory {

namespace internal {

namespace HACTree{
struct EDGE{//nodes unordered
    volatile int first;
    volatile int second;
    volatile double w;//weight

    EDGE(int t_u, int t_v, double t_w):first(t_u), second(t_v), w(t_w){}
    EDGE():first(NO_NEIGH), second(NO_NEIGH), w(std::numeric_limits<double>::max()){}

    inline void print(){
        std::cout << "(" << first << ", " <<  second << "):" << w << std::endl;
    }

    inline double getW() const {return w;}
    inline std::pair<int, int> getE() const {return std::make_pair(first,second);}
    inline void update(int t_u, int t_v, double t_w){
      first = t_u;
      second = t_v;
      w = t_w;
    }
};


struct edgeComparator2{
    double eps = 1e-20;
    edgeComparator2(){}
    edgeComparator2(double _eps):eps(_eps){}
    bool operator () (EDGE i, EDGE j) {
      // if(abs(i.getW() - j.getW()) < i.getW()*std::numeric_limits<double>::epsilon()) return i.second < j.second;
      if(abs(i.getW() - j.getW()) <= eps) return i.second < j.second;
      return i.getW() < j.getW();
      }
};

}



struct dendroLine{
    int id1;
    int id2;
    double height;
    int size;
    dendroLine(int _id1, int _id2, double _height, int _size):id1(_id1), id2(_id2), height(_height), size(_size){}
    dendroLine(){}

    void print(std::ofstream &file_obj){
        file_obj << id1 << " " << id2 << " " << std::setprecision(20) << height << " " << size << std::endl; 
    }

    void print(){
        std::cout << id1 << " " << id2 << " " << height << " " << size << std::endl; 
    }
};

struct TreeChainInfo{
  parlay::sequence<int> terminal_nodes;// must be cluster id
  parlay::sequence<int> chain;//chain[i] is cluster i's nn, NO_NEIGH for unknown or invalid// -1 for unknown, -2 for invalid
  parlay::sequence<bool> is_terminal;
  parlay::sequence<bool> flag;
  int chainNum;

  parlay::sequence<pair<double, long>> chainRev;// reverse chain, indexed by cid TODO: change to just double?
  parlay::sequence<pair<int, double>> revTerminals; // indexed by ith temrinal nodes
  static const pair<double, long> invalid_rev = make_pair(numeric_limits<double>::max(), (long)-1);


  TreeChainInfo(int n, double _eps = 1e-20){
    terminal_nodes = parlay::sequence<int>(n);
    parlay::parallel_for(0,n,[&](int i){terminal_nodes[i] = i;});
    chain = parlay::sequence<int>(n, NO_NEIGH);
    is_terminal = parlay::sequence<bool>(n, true);
    flag = parlay::sequence<bool>(n);
    chainNum = n;

    chainRev = parlay::sequence<pair<double, long>>(n, invalid_rev); //note: changing!
    revTerminals = parlay::sequence<pair<int, double>>(n);

    EC2 = LDS::PairComparator21<pair<long, double>>(_eps);
  }

  inline void updateChain(int cid, int nn, double w){
    chain[cid] = nn;
    utils::writeMin(&chainRev[nn], make_pair(w,(long)cid), std::less<pair<double, long>>()); 
  }
  // then in checking, only check for -1
  inline void invalidate(int cid, int code){
    chain[cid] = code;
  }

  inline void invalidateRev(int cid){
    chainRev[cid] = invalid_rev;
  }

  //get the rev of ith terminal nodes 
  inline pair<int, double> getChainPrev(int i){
    return revTerminals[i];
  }

  inline int getNN(int cid){return chain[cid];}
  inline bool isTerminal(int cid){return is_terminal[cid];}

  // update findNN, terminal_nodes and chainNum
  template<class F>
  inline void next(F *finder){
    parlay::parallel_for(0, finder->n, [&](int i){
      is_terminal[i] = false;
    });
    int C = finder->C;
    parlay::parallel_for(0, C, [&](int i){
      int cid = finder->activeClusters[i];
      flag[i] = getNN(cid) == NO_NEIGH || getNN(getNN(cid)) == NO_NEIGH ;// only merged clusters have negative neighbor in chain ok because -2 won't be in active clusters
      is_terminal[cid] = flag[i];
    });
    chainNum = parlay::pack_into(make_slice(finder->activeClusters).cut(0,C), flag, terminal_nodes);

    parlay::parallel_for(0, chainNum, [&](int i){
      revTerminals[i] =  make_pair((int)chainRev[terminal_nodes[i]].second, chainRev[terminal_nodes[i]].first);
    });
  }

};


}  // namespace internal
}  // namespace in_memory
}  // namespace research_graph