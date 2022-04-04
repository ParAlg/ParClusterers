#pragma once

#include <atomic>

#include "point.h"
#include "node.h"
#include "kdtree.h"
#include "treeUtilities.h"

#include "parlay/sequence.h"
#include "parlay/parallel.h"
#include "parlay/utilities.h"


using namespace std;
namespace research_graph {
namespace in_memory {

namespace HACDIST{
template<class pointT>
inline double pointDist(pointT* p, pointT* q){
  return p->pointDist(q);
}

template<class pointT>
inline double pointDist(pointT p, pointT q){
  return p.pointDist(q);
}

template<class pointT>
inline double getPointId(pointT* p){
  return p->idx();
}

template<class pointT>
inline double getPointId(pointT p){
  return p.idx();
}

#define LINKAGE_AVE_BRUTE_THRESH 100000
    //find the average　distance between P1 and P2, return random pair of points
    template<class pointT, class ARR>
    inline pair<pair<int, int>, double> bruteForceAverage(ARR P1, ARR P2, int n1, int n2, bool div = true){
        pair<int, int> result = make_pair(getPointId(P1[0]), getPointId(P2[0]));
        std::atomic<double> total_d; total_d.store(0);
        long total_n = (long)n1 * (long)n2;
        // intT eltsPerCacheLine = 128 /sizeof(double);
        if(total_n < LINKAGE_AVE_BRUTE_THRESH){ 
            for(int i=0; i< n1; ++i){
                for(int j=0;j<n2; ++j){
                    total_d +=  pointDist<pointT>(P1[i], P2[j]);
                }
            }
        }else{
            if(n1 < n2){
                swap(n1, n2);
                swap(P1, P2);
            }
            int nBlocks1 =  parlay::num_workers() * 8;

            int BLOCKSIZE1 = max(1,n1/nBlocks1);

            if(n1 > nBlocks1*BLOCKSIZE1) BLOCKSIZE1 += 1;

            parlay::parallel_for(0, nBlocks1, [&](int ii){
                double thread_sum = 0;
                for (int i=ii*BLOCKSIZE1; i< min((ii+1)*BLOCKSIZE1, n1); ++i){
                    for (int j=0; j< n2; ++j){
                        thread_sum += pointDist<pointT>(P1[i], P2[j]);
                    }
                }
                parlay::write_add(&total_d, thread_sum);
            },1)

        }

        if(div) return make_pair(result, total_d/total_n);
        return make_pair(result, total_d);
    }

    // template<int dim, class pointT, class nodeInfo>
    // inline pair<pair<int, int>, double> bruteForceAverage(struct kdTree<dim, pointT, nodeInfo> *t1, struct kdTree<dim, pointT, nodeInfo> *t2, bool div = true){
    //     pointT** P1 = t1->items;
    //     pointT** P2 = t2->items;
    //     intT n1 = t1->getN();
    //     intT n2 = t2->getN();
    //     return bruteForceAverage<pointT, pointT**>(P1, P2, n1, n2, div);
    // }

    // template<int dim, class pointT, class nodeInfo>
    // inline pair<pair<int, int>, double> bruteForceAverage(struct kdNode<dim, pointT, nodeInfo> *t1, struct kdNode<dim, pointT, nodeInfo> *t2, bool div = true){
    //     pointT** P1 = t1->items;
    //     pointT** P2 = t2->items;
    //     intT n1 = t1->n;
    //     intT n2 = t2->n;
    //     return bruteForceAverage<pointT, pointT**>(P1, P2, n1, n2, div);
    // }

    // template<class pointT, class nodeT>
    // inline pair<pair<int, int>, double> bruteForceAverage(nodeT *inode,  nodeT *jnode, pointT **clusteredPts){
    //     pointT **P1 = clusteredPts + inode->getOffset();
    //     pointT **P2 = clusteredPts + jnode->getOffset();
    //     intT n1 = inode->n;
    //     intT n2 = jnode->n;
    //     return FINDNN::bruteForceAverage<pointT, pointT**>(P1, P2, n1, n2, true);
    // }

    template<class pointT, class nodeT>
    inline pair<pair<int, int>, double> bruteForceAverage(nodeT *inode,  nodeT *jnode, pointT *clusteredPts){
        pointT *P1 = clusteredPts + inode->getOffset();
        pointT *P2 = clusteredPts + jnode->getOffset();
        int n1 = inode->n;
        int n2 = jnode->n;
        return FINDNN::bruteForceAverage<pointT, pointT*>(P1, P2, n1, n2, true);
    }
}

enum Method { WARD, COMP, AVG, SINGLE };


// same as distComplete1, uses array instead of cache
template<int dim>
struct distComplete {
    typedef node<dim, iPoint<dim>, kdNodeInfo> kdnodeT;
  static const Method method = COMP;

  using M = FINDNN::MarkClusterId<dim, Fr>; //mark the all points kdtree

  M marker;
  distComplete(UnionFind::ParUF<int> *t_uf){
      marker = M(t_uf);
  }
  inline static void printName(){
    cout << "distComplete" << endl;
  }
  inline static bool doRebuild(){return false;}
  inline double updateDistO(double d1, double d2, double nql, double nqr, double nr, double dij){
    return max(d1,d2);
  }

  inline double updateDistN(double d1, double d2, double d3, double d4, 
                       double nql, double nqr, double nrl, double nrr,
                       double dij, double dklr){
    return max(max(max(d1,d2), d3), d4);
  }

    // range query radius
    inline double getBall(Node<dim>* query, double beta){
        return beta;
    }

    template<class F>
    inline void postProcess(F *finder){}

    void update(int round, F *finder){
        if(marker.doMark(C, round)){ //TODO: move marker to distComputation
            HACTree::singletree<kdnodeT, M, typename M::infoT>(finder->kdtree->root, &marker, marker.initVal);
        }
    }
};

template<class T>
struct distWard {
  Method method = WARD;
  using MCenter = HACTree::MarkMinN<dim, pointT, nodeInfo>;


  inline static void printName(){
    cout << "distWARD" << endl;
  }
  inline static bool doRebuild(){return true;}
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

  inline double getDistNaive(Node<dim> *inode,  Node<dim> *jnode, 
                          double lb = -1, double ub = numeric_limits<double>::max(), 
                          bool par = true){ 
    double ni = inode->n; 
    double nj = jnode->n;
    if(ni + nj > 2) return sqrt(2*(ni*nj)*inode->dist(jnode)/(ni + nj));
    return sqrt(inode->dist(jnode));
  }

    // range query radius
    inline double getBall(Node<dim>* query, double beta){
        double n = query->size();
        return beta*sqrt((n+1)/2.0/n);
    }
    template<class F>
    inline void postProcess(F *finder){}

    template<class F>
    inline void update(int round, F *finder){
        if(marker.doMark(C, round)){ //TODO: move marker to distComputation
            HACTree::singletree<kdnodeT, M, typename M::infoT>(kdtree->root, &marker, marker.initVal);
        }
    }
};

template<int dim>
struct distAverage {
  typedef point<dim> pointT;
  typedef Node<dim> nodeT;

  Method method = AVG;

  pointT *clusteredPts1;
  pointT *clusteredPts2;//clusteredPts point to one of the two
  paraly::sequence<int> clusterOffsets;  //same order as activeClusters in finder
  pointT *clusteredPts;

  distAverage(pointT *t_PP, intT n, bool t_no_cache):PP(t_PP){
    clusteredPts1 = (pointT *) malloc(n* sizeof(pointT));
    clusteredPts2 = (pointT *) malloc(n* sizeof(pointT));
    clusteredPts = clusteredPts1;
    parlay::parallel_for(0,n,[&](int i){
      clusteredPts1[i]=PP[i];
    });
    clusterOffsets = paraly::sequence<int>(n, [&](int i){return i;});
  }

  ~distAverage(){
      free(clusteredPts1);
      free(clusteredPts2);
  }

  inline static void printName(){
    cout << "distAverage" << endl;
  }
  inline static bool doRebuild(){return true;}
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

    double getDistNaive(Node<dim> *inode,  Node<dim> *jnode, 
                          double lb = -1, double ub = numeric_limits<double>::max(), 
                          bool par = true){ 
    pair<pair<int, int>, double> result = FINDNN::bruteForceAverage(inode, jnode, clusteredPts);
    return result.second;
    }


    // range query radius
    inline double getBall(Node<dim>* query, double beta){
        return beta;
    }

    // copy points from oldArray[oldOffset:oldOffset+n] to newArray[newOffset:newOffset+n]
    inline void copyPoints(pointT *oldArray, pointT *newArray, int copyn, int oldOffset, int newOffset){
        parlay::parallel_for(0, copyn,[&](int j){
          newArray[newOffset + j] = oldArray[oldOffset+j];
      });
    }

    template<class F>
    inline void update(intT round, F *finder){
    int *activeClusters = finder->activeClusters.data();
    int  C = finder->C;
    //  put points into clusters
    parlay::parallel_for(0, C, [&](int i){
      clusterOffsets[i] = finder->getNode(activeClusters[i])->size();
    });
    parlay::scan_inclusive_inplace(clusterOffsets.cut(0,C));
    // sequence::prefixSum(clusterOffsets, 0, C);
    pointT *oldArray = clusteredPts;
    pointT *newArray = clusteredPts1;
    if(clusteredPts == clusteredPts1){
      newArray  = clusteredPts2;
    }

    parlay::parallel_for(0,C,[&](int i){
      int cid  = activeClusters[i];
      nodeT *clusterNode = finder->getNode(cid);
      int oldOffset = clusterNode->getOffset(); // must save this before setting new
      int newOffset = clusterOffsets[i];
      clusterNode->setOffset(newOffset);
      if(clusterNode->round == round){//merged this round, copy left and right
        nodeT *clusterNodeL = clusterNode->left;
        oldOffset = clusterNodeL->getOffset();
        copyPoints(oldArray, newArray, clusterNodeL->n, oldOffset, newOffset);
        newOffset += clusterNodeL->n;

        nodeT *clusterNodeR = clusterNode->right;
        oldOffset = clusterNodeR->getOffset();
        copyPoints(oldArray, newArray, clusterNodeR->n, oldOffset, newOffset);

      }else{//not merged this round, just copy
        copyPoints(oldArray, newArray, clusterNode->n, oldOffset, newOffset);
      } 
    });
    clusteredPts = newArray;
    }
    template<class F>
    inline void postProcess(F *finder){}
};


//todo: is this wrong? the update should update on sqaured distances
template<int dim>
struct distAverageSq {
  Method method = AVGSQ;

  inline static void printName(){
    cout << "distAverage" << endl;
  }
  inline static bool doRebuild(){return true;}
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

    inline double getDistNaive(Node<dim> *inode,  Node<dim> *jnode, 
                            double lb = -1, double ub = numeric_limits<double>::max(), 
                            bool par = true){ 
    return sqrt(inode->center.pointDistSq(jnode->center) + inode->var + jnode->var);
    }

    // range query radius
    inline double getBall(Node<dim>* query, double beta){
        return sqrt(beta);
    }

    template<class F>
    inline void update(intT round, F *finder){

    }

    template<class F>
    inline void postProcess(F *finder){
        parallel_for(int i=0; i<2*finder->n-1; ++i) {finder->nodes[i].height = finder->nodes[i].height * finder->nodes[i].height;}
        parallel_for(int i=0; i<finder->n; ++i) {finder->uf->values[i] = finder->uf->values[i]  * finder->uf->values[i];}
    }
};

}
}