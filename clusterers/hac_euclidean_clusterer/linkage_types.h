#pragma once

#include <iostream>
#include <fstream> 
#include <math.h>
#include <atomic>
#include <tuple>
#include "point.h"
#include "parlay/utilities.h"

#define SIZE_T_MAX std::numeric_limits<size_t>::max()
#define NO_NEIGH SIZE_T_MAX


using namespace std;
namespace research_graph {
namespace in_memory {

namespace internal {

// Node data structure, used for dendrogram tree, each node represents a intermediate cluster
// TODO: changed size_t to int, but need to check overflow for multication computation
template<int dim>
struct Node{
    int cId; // cluster id corresponds to the union find id
    int round = 0; // merged at which round. if 0, a sigleton cluster
    int idx; // node idx, unique for each node
    double n = 1; // use double because need to do floating point computation using n
    double height = 0;
    Node<dim> *left = nullptr;
    Node<dim> *right = nullptr;
    point<dim> pMin, pMax;
    int offset = -1;  // the offset of this node in the 2*datasize-1 arrays of all dendrogram nodes
    point<dim> center;

    //get the node from merged left and right nodes
    // if height=0, assume the left and right are identical
    Node(int t_cid, int t_round, int t_idx, Node<dim> *t_left, Node<dim> *t_right, double _height):
        cId(t_cid),
        round(t_round),
        idx(t_idx),
        height(_height),
        left(t_left),
        right(t_right){
            n = t_left->n + t_right->n;
            //compute bounding box and center
            pMin = left->pMin;
            pMax = left->pMax;
            pMin.minCoords(right->pMin);
            pMax.maxCoords(right->pMax);
            if(height ==0){
                center = t_left->center; //important to avoid numerical error
            }else{
                for (int i=0; i<dim; ++i) {
                    center.updateX(i, 
                                    (t_left->n    / n * t_left->center[i] ) + 
                                    (t_right->n   / n * t_right->center[i]) );
                }
            }
        }
    
    // initialize a singleton node for a point
    Node(std::size_t  t_cid, point<dim> p):
        cId(t_cid),
        idx(t_cid),
        offset(t_cid){
                pMin = p;
                pMax = p;
                center = p;
            }
    Node(){}

    inline bool isLeaf() const {return n == 1;}
    inline double getHeight() const {return height;}
    inline std::size_t getRound() const {return round;}
    inline std::size_t getIdx() const {return idx;}
    inline std::size_t size() const {return n;}
    inline int getOffset(){return offset;}
    inline void setOffset(int i){offset = i;}
    inline double dist(Node<dim> *node){return center.pointDistSq(node->center);}
};

// used for dendrogram sorting 
template<int dim>
struct nodeComparator{
  double eps;
  nodeComparator(double _eps):eps(_eps) {
  }

  bool operator() (const Node<dim> i, const Node<dim> j) const {
      if(abs(i.getHeight() - j.getHeight()) <= eps) return i.getRound() < j.getRound();
     return i.getHeight() < j.getHeight();
  }
};


struct kdNodeInfo{
    typedef std::tuple<int, int> infoT; // <cid, min_n>
    int cId;
    std::atomic<double> ub;
    int round = 0;
    int idx = -1; // node idx
    int min_n = 1; //used for ward's linkage

    kdNodeInfo(){
        cId = -1;
        ub.store(numeric_limits<double>::max()); // used for allptsnn
    }

    inline double getUB(){return ub.load();}
    inline void updateUB(double tmp){
        parlay::write_min(&ub, tmp, std::less<double>());
    }
    inline void resetUB(){
        ub.store(numeric_limits<double>::max());
    }
    inline int getCId(){return cId;}
    inline void setCId(int id){cId = id;}
    inline int getRound(){return round;}
    inline void setRound(int r){round = r;}
    inline int getIdx(){return idx;}
    inline void setIdx(int r){idx = r;}
    inline infoT initInfoVal() {return make_tuple(-1, numeric_limits<int>::max());}
    inline void setInfo(infoT info){
      tie(cId, min_n) = info;
    }
    inline double getMinN(){return (double)min_n;} //return double because it's used in floating point computation
    inline void setMinN(int nn){min_n = nn;}

};

struct EDGE{//nodes unordered
    volatile std::size_t first;
    volatile std::size_t second;
    volatile double w;//weight

    EDGE(std::size_t t_u, std::size_t t_v, double t_w):first(t_u), second(t_v), w(t_w){}
    EDGE():first(NO_NEIGH), second(NO_NEIGH), w(std::numeric_limits<double>::max()){}

    inline void print(){
        std::cout << "(" << first << ", " <<  second << "):" << w << std::endl;
    }

    inline double getW() const {return w;}
    inline std::pair<std::size_t, std::size_t> getE() const {return std::make_pair(first,second);}
    inline void update(std::size_t t_u, std::size_t t_v, double t_w){
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


enum Method { WARD, COMP, AVG, SINGLE };


// same as distComplete1, uses array instead of cache
template<class T>
struct distComplete {
  Method method = COMP;

  inline static void printName(){
    cout << "distComplete" << endl;
  }
  inline double updateDistO(double d1, double d2, double nql, double nqr, double nr, double dij){
    return max(d1,d2);
  }

  inline double updateDistN(double d1, double d2, double d3, double d4, 
                       double nql, double nqr, double nrl, double nrr,
                       double dij, double dklr){
    return max(max(max(d1,d2), d3), d4);
  }
};

template<class T>
struct distSingle {
  Method method = SINGLE;

  inline static void printName(){
    cout << "distSingle" << endl;
  }
  inline double updateDistO(double d1, double d2, double nql, double nqr, double nr, double dij){
    return min(d1,d2);
  }

  inline double updateDistN(double d1, double d2, double d3, double d4, 
                       double nql, double nqr, double nrl, double nrr,
                       double dij, double dklr){
    return min(min(min(d1,d2), d3), d4);
  }
};

template<class T>
struct distWard {
  Method method = WARD;

  inline static void printName(){
    cout << "distWARD" << endl;
  }
  inline double updateDistO(double dik, double djk, double ni, double nj, double nk, double dij){
    double ntotal = ni + nj + nk;
    double d = sqrt( ( ((ni + nk)  * dik * dik) + ((nj + nk) * djk * djk) - (nk * dij * dij) )/ ntotal );
    return d;
  }

  inline double updateDistN(double dikl, double dikr, double djkl, double djkr, 
                       double ni, double nj, double nkl, double nkr,
                       double dij, double dklr ){
    double dik = updateDistO(dikl, dikr, nkl, nkr, ni, dklr);
    double djk = updateDistO(djkl, djkr, nkl, nkr, nj, dklr);
    return updateDistO(dik, djk, ni, nj, (nkl + nkr), dij);
  }
};

template<class T>
struct distAverage {
  Method method = AVG;

  inline static void printName(){
    cout << "distAverage" << endl;
  }
  inline double updateDistO(double d1, double d2, double nql, double nqr, double nr, double dij){
    double n1 = (double)nql * (double)nr;
    double n2 = (double)nqr * (double)nr;
    double alln = n1 + n2 ;
    d1 = n1 / alln * d1;
    d2 = n2 / alln * d2;
    return d1 + d2;
  }

  inline double updateDistN(double d1, double d2, double d3, double d4, 
                       double nql, double nqr, double nrl, double nrr,
                       double dij, double dklr){
    double n1 = (double)nql * (double)nrl;
    double n2 = (double)nql * (double)nrr;
    double n3 = (double)nqr * (double)nrl;
    double n4 = (double)nqr * (double)nrr;
    double alln = n1 + n2 + n3 + n4;
    d1 = n1 / alln * d1;
    d2 = n2 / alln * d2;
    d3 = n3 / alln * d3;
    d4 = n4 / alln * d4;
    return d1  + d2  + d3 + d4;
  }
};


struct dendroLine{
    std::size_t id1;
    std::size_t id2;
    double height;
    std::size_t size;
    dendroLine(std::size_t _id1, std::size_t _id2, double _height, std::size_t _size):id1(_id1), id2(_id2), height(_height), size(_size){}
    dendroLine(){}

    void print(std::ofstream &file_obj){
        file_obj << id1 << " " << id2 << " " << std::setprecision(20) << height << " " << size << std::endl; 
    }

    void print(){
        std::cout << id1 << " " << id2 << " " << height << " " << size << std::endl; 
    }
};

struct TreeChainInfo{
  parlay::sequence<std::size_t> terminal_nodes;// must be cluster id
  parlay::sequence<std::size_t> chain;//chain[i] is cluster i's nn, NO_NEIGH for unknown or invalid// -1 for unknown, -2 for invalid
  parlay::sequence<bool> is_terminal;
  parlay::sequence<bool> flag;
  std::size_t chainNum;

  TreeChainInfo(std::size_t n){
    terminal_nodes = parlay::sequence<std::size_t>(n);
    parlay::parallel_for(0,n,[&](std::size_t i){terminal_nodes[i] = i;});
    chain = parlay::sequence<std::size_t>(n, NO_NEIGH);
    is_terminal = parlay::sequence<bool>(n, true);
    flag = parlay::sequence<bool>(n);
    chainNum = n;
  }

  inline void updateChain(std::size_t cid, std::size_t nn, double w){
    chain[cid] = nn;
  }
  // then in checking, only check for -1
  inline void invalidate(std::size_t cid, std::size_t code){
    chain[cid] = code;
  }
  inline std::size_t getNN(std::size_t cid){return chain[cid];}
  inline bool isTerminal(std::size_t cid){return is_terminal[cid];}

  // update findNN, terminal_nodes and chainNum
  template<class F>
  inline void next(F *finder){
    parlay::parallel_for(0, finder->n, [&](std::size_t i){
      is_terminal[i] = false;
    });
    std::size_t C = finder->C;
    parlay::parallel_for(0, C, [&](std::size_t i){
      std::size_t cid = finder->activeClusters[i];
      flag[i] = getNN(cid) == NO_NEIGH || getNN(getNN(cid)) == NO_NEIGH ;// only merged clusters have negative neighbor in chain ok because -2 won't be in active clusters
      is_terminal[cid] = flag[i];
    });
    chainNum = parlay::pack_into(make_slice(finder->activeClusters).cut(0,C), flag, terminal_nodes);
  }

};


}  // namespace internal
}  // namespace in_memory
}  // namespace research_graph