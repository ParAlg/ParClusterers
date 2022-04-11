#pragma once

#include <atomic>
#include "utils/unionfind.h"
#include "utils/chain.h"
#include "kdtree/kdtree.h"
#include "treeUtilities.h"
#include "cacheUtilities.h"

#include "parlay/sequence.h"
#include "parlay/parallel.h"
#include "parlay/primitives.h"

using namespace std;

namespace research_graph {
namespace in_memory {
namespace internal {
namespace HACTree {

template<int dim, class distF, class Fr>
class NNFinder {
  public:
  typedef iPoint<dim> pointT;
  typedef Node<dim> nodeT;
  typedef HACTree::node<dim, pointT, nodeInfo > kdnodeT;
  typedef HACTree::tree<dim, pointT, nodeInfo > treeT;
  typedef Table<hashCluster> distCacheT;
  typedef int intT;
  
  int C;// number of clusters
  int n;// number of points
  parlay::sequence<pointT> PP;
  parlay::sequence<int> rootIdx;
  UnionFind::ParUF<intT> *uf;
  parlay::sequence<int> activeClusters; // ids of connected components
  parlay::sequence<bool> flag;//used in updateActivateClusters
  parlay::sequence<int> newClusters;//used in updateActivateClusters to swap with activeClusters
  treeT *kdtree;
  EDGE *edges; //edges[i] stores the min neigh of cluster i

  bool no_cache;
  CacheTables<nodeT> *cacheTables;

  atomic<intT> nodeIdx; // the index of next node to use for cluster trees
  parlay::sequence<nodeT> nodes; // preallocated space to store tree nodes
  
  distF *distComputer;
  parlay::sequence<pointT> centers; //used to rebuild kd-tree

  int NAIVE_THRESHOLD = 10;
  double eps;
  edgeComparator2 EC2;

  parlay::sequence<int> natural_int_array(int n){
    auto A = parlay::sequence<int>(n); 
    parlay::parallel_for(0,n,[&](int i){A[i]=i;});
    return A;
  }

  NNFinder(int t_n, point<dim>* t_P, UnionFind::ParUF<intT> *t_uf, distF *_distComputer, 
    bool t_noCache, int t_cache_size=32, double t_eps = 0, int t_naive_thresh=10): 
    n(t_n), uf(t_uf), no_cache(t_noCache), NAIVE_THRESHOLD(t_naive_thresh), eps(t_eps){
    EC2 = edgeComparator2(eps);
    C = n;
    activeClusters = natural_int_array(n);
    PP = makeIPoint(t_P, n);
    centers = parlay::sequence<pointT>(n); parlay::parallel_for(0,n,[&](int i){centers[i]=PP[i];});

    distComputer = _distComputer;

    rootIdx = natural_int_array(n);
    nodes = parlay::sequence<nodeT>(2*n);
    parlay::parallel_for(0,n,[&](int i){nodes[i] = nodeT(i, t_P[i]);});
    
    flag = parlay::sequence<bool>(C);
    newClusters = parlay::sequence<int>(C);

    edges = (EDGE *)aligned_alloc(sizeof(EDGE), n*sizeof(EDGE));
    parlay::parallel_for(0,n,[&](int i){edges[i] = EDGE();});

    cacheTables = new CacheTables<nodeT>(no_cache, n, t_cache_size, this);

    nodeIdx.store(n); // have used n nodes
    kdtree = build<dim, pointT , nodeInfo>(PP, true);
  }

  ~NNFinder(){
    delete cacheTables;
    delete kdtree;
    free(edges);
  }

  inline int cid(nodeT* node){ return node->cId;}
  inline int idx(nodeT* node){ return node->idx;}
  inline nodeT *getNode(int cid){return &nodes[rootIdx[cid]];}
  inline distCacheT *getTable(int idx){return cacheTables->getTable(idx);}
  inline int idx(int cid){return idx(getNode(cid));}
  inline int leftIdx(int cid){return getNode(cid)->left->idx;}
  inline int rightIdx(int cid){return getNode(cid)->right->idx;}
  inline int cid(int idx){return cid(nodes[idx]);}
  inline bool justMerge(int cid, int round){
    return (size_t)round == getNode(uf->find(cid))->getRound();
  }


  inline double getDistNaive(nodeT *inode,  nodeT *jnode, double lb = -1, double ub = numeric_limits<double>::max(), bool par = true){
    return distComputer->getDistNaive(inode, jnode, lb, ub, par);
  }

  // i, j are cluster ids
  // find distance in table 
  // if not found, compute by bruteforce and insert 
  // true if found in table
  // used in merging 
  // range search has its own updateDist
  tuple<double, bool> getDist(int i,  int j, double lb = -1, double ub = numeric_limits<double>::max(), bool par = true){
    if(!no_cache){
    double d = cacheTables->find(i, j);
    if(d != UNFOUND_TOKEN && d != CHECK_TOKEN) return make_tuple(d, true);
    }
    return make_tuple(getDistNaive(getNode(i), getNode(j), lb, ub, par), false);
  }

  tuple<double, bool> getDist(nodeT *inode,  nodeT *jnode, double lb = -1, double ub = numeric_limits<double>::max(), bool par = true){
    if(!no_cache){
    double d = cacheTables->find(inode, jnode);
    if(d != UNFOUND_TOKEN && d != CHECK_TOKEN) return make_tuple(d, true); 
    }
    return make_tuple(getDistNaive(inode, jnode, lb, ub, par), false);
  }

  //newc is a newly merged cluster
  // newc is new, rid is old
  inline double getNewDistO(int newc, int rid){
    nodeT* ql = getNode(newc)->left;
    int nql = ql->n;
    nodeT* qr = getNode(newc)->right;
    int nqr = qr->n;
    nodeT* rroot = getNode(rid);
    int nr = rroot->n;
    double dij = getNode(newc)->getHeight();

    double d1,d2; bool intable;
    tie(d1, intable) = getDist(ql,rroot);
    tie(d2, intable) = getDist(qr,rroot);
    return distComputer->updateDistO(d1, d2, nql, nqr, nr, dij);
  }


  // newc is a newly merged cluster
  // newc is new, rid is merged
  inline double getNewDistN(int newc, int rid){
    nodeT* ql = getNode(newc)->left;
    int nql = ql->n;
    nodeT* qr = getNode(newc)->right;
    int nqr = qr->n;
    double dij = getNode(newc)->getHeight();

    nodeT* rl = getNode(rid)->left;
    int nrl = rl->n;
    nodeT* rr = getNode(rid)->right;
    int nrr = rr->n;
    double dklr = getNode(rid)->getHeight();

    double d1,d2, d3, d4; bool intable;
    tie(d1, intable) = getDist(ql,rl);
    tie(d2, intable) = getDist(ql,rr);
    tie(d3, intable) = getDist(qr,rl);
    tie(d4, intable) = getDist(qr,rr);
    return distComputer->updateDistN(d1, d2, d3, d4, nql, nqr, nrl, nrr, dij, dklr);
  }

  // store the closest nn in edges
  // assume edges[cid] already has max written
  void getNN_naive(int cid, double ub = numeric_limits<double>::max(), int t_nn = -1){
    utils::writeMin(&edges[cid], EDGE(cid, t_nn, ub), EC2); 
    parlay::parallel_for(0, C, [&](int i){
      int cid2 = activeClusters[i];
      if(cid2 != cid){//if(cid2 < cid) { // the larger one might not be a terminal node only work if C == |terminal node|
        double tmpD;
        bool intable;
        tie(tmpD, intable) = getDist(cid, cid2, -1, edges[cid].getW(), true);
        utils::writeMin(&edges[cid], EDGE(cid,cid2,tmpD), EC2);
        utils::writeMin(&edges[cid2], EDGE(cid2,cid,tmpD), EC2); 
        if((!intable) && (!no_cache) && (tmpD != LARGER_THAN_UB)){
          cacheTables->insert(cid, cid2, tmpD);
        }
      }
    });
  }

  // store the closest nn in edges
  // assume edges[cid] already has max written
  inline void getNN(int cid, double ub = numeric_limits<double>::max(), int t_nn = -1){
    // if(edges[cid].getW() ==0){ can't stop, need the one with smallest id

    // after a round of C <= 50, we might not have all entries
    // in the table. We only have terminal nodes->all clusters
    if(C <= NAIVE_THRESHOLD){
      getNN_naive(cid, ub, t_nn);
      return;
    }

    // check in merge, no inserting merged entry
    // can't writemin to all edges first and then search
    // maybe because a bad neighbor can write to the edge and give a bad radius
    double minD = ub;
    int nn = t_nn;
    bool intable;
    Node<dim>* query = getNode(cid);

    if(ub == numeric_limits<double>::max()){
        typedef NNsingle<kdnodeT> Fs;
        Fs fs = Fs(uf, cid);
        pointT *centroid = new pointT(query->center, cid);
        treeT treetmp = treeT(centroid, cid); 
        // closest to a single point in cluster
        dualtree<kdnodeT, Fs>(&treetmp, kdtree, &fs);
        
        nn = uf->find(fs.e->second);
        tie(minD, intable) = getDist(cid, nn); 
        delete centroid;
#ifdef DEBUG
        if(minD==LARGER_THAN_UB){
          cout << "minD is inifinity" << endl;
          exit(1);
        }
#endif
        if((!intable) && (!no_cache)  && (minD != LARGER_THAN_UB)){
          cacheTables->insert(cid, nn, minD);
        }
    }

    utils::writeMin(&edges[cid], EDGE(cid, nn, minD), EC2); 
    utils::writeMin(&edges[nn], EDGE(nn, cid, minD), EC2);
    if(minD ==0){
      return;
    }
    double r = distComputer->getBall(query, minD)+eps; 
    Fr fr = Fr(cid, r, cacheTables, edges, distComputer, no_cache, C, eps); 
    HACTree::rangeTraverse<dim, iPoint<dim>, kdnodeT, Fr>(kdtree, query->center, r, &fr);

    if(fr.local){
    nn = fr.getFinalNN();
    minD = fr.getFinalDist();
    utils::writeMin(&edges[cid], EDGE(cid, nn, minD), EC2); 
    }
  }

    // merge two clusters in dendrogram
    // u, v are cluster ids
  inline void merge(int u, int v, int newc, int round, double height){
    int rootNodeIdx = nodeIdx.fetch_add(1);
    nodes[rootNodeIdx] = nodeT(newc, round, rootNodeIdx, getNode(u), getNode(v), height);
    rootIdx[newc] = rootNodeIdx;
  }

  inline void updateDist(int newc){
    CHECK_NO_CACHE(497)
    int idx1 = leftIdx(newc);
    int idx2 = rightIdx(newc);
    int cid1 = getNode(newc)->left->cId;
    int cid2 = getNode(newc)->right->cId;

    auto tb1 = cacheTables->getTable(idx1);//cacheTbs[idx1];
    auto tb2 = cacheTables->getTable(idx2);//cacheTbs[idx2];

    auto TAR1 = tb1->entries();
    auto TAR2 = tb2->entries();
    // if(C < 100)cout << "sizes in merge: " << TAR1.n+TAR2.n << "/" << tb1->m + tb2->m << endl;
    auto newtb = cacheTables->getTable(idx(newc));//cacheTbs[idx(newc)];

    parlay::parallel_for(0, (TAR1.size()+TAR2.size()), [&](int i){
      auto TA = TAR1.data();
      int offset = 0;
      if((size_t)i >= TAR1.size()){ //switch to process tb2
        TA = TAR2.data();
        offset = TAR1.size();
      }
      int j = i-offset;
      int newc2 = -1;
      int storedIdx = TA[j].idx; 
      // if storedIdx is inconsistant, the newc2 has changed and we do not calculate
      newc2 = uf->find(TA[j].first);
      if(newc2 != cid1 && newc2 != cid2){

          bool success = false;
          double d;
          // if newc2 is a merged cluster
          if(getNode(newc2)->round == getNode(newc)->round){
            // TA[j].first should == left idx or  right idx
            if(storedIdx == idx(getNode(newc2)->left) || storedIdx == idx(getNode(newc2)->right)){
              success = cacheTables->insert_check(newc, newc2, true, false); // table might not be symmetric, faster than no ins check
              if(success) d = getNewDistN(newc, newc2); 
            }
            
          }else{
            // TA[j].first should == idx
            if(storedIdx == idx(getNode(newc2))){
              success = cacheTables->insert_check(newc, newc2, false, false);
              if(success) d = getNewDistO(newc, newc2);
            }
          }
        if(success) { // only insert duplicated entries once 
          cacheTables->insert_helper(newc, newc2, d, newtb, cacheTables->getTable(idx(newc2)));
        }
      }

    });
  }

  // find new activeClusters array based on uf, update C
  inline void updateActiveClusters(int round){
    parlay::parallel_for(0,C,[&](int i){
      int cid  = activeClusters[i];
      flag[i] = (uf->find(cid)==cid);
    });
    
    C = parlay::pack_into(make_slice(activeClusters).cut(0,C), flag, make_slice(newClusters).cut(0,C));
    swap(activeClusters, newClusters);

    if(distComputer->doRebuild()){
      parlay::parallel_for(0, C, [&](int i){
        int cid = activeClusters[i];
        centers[i] = pointT(getNode(cid)->center, cid); //consider making an array of active node pointers
      });

      delete kdtree;
      kdtree = build<dim, pointT , nodeInfo>(centers.cut(0, C), true); //TODO optimize to rebuild
    }
  }

  // edges[i] stores the ith nn  of point i
  // initialize the chain in info
  inline void initChain(TreeChainInfo *info){
    if(distComputer->squared){
      typedef AllPtsNN<kdnodeT, true> F;
      F *f = new F(edges, eps);
      dualtree<kdnodeT, F>(kdtree, kdtree, f, false);
      delete f;
    }else{
      typedef AllPtsNN<kdnodeT, false> F;
      F *f = new F(edges, eps);
      dualtree<kdnodeT, F>(kdtree, kdtree, f, false);
      delete f;
    }
    parlay::parallel_for(0,n,[&](int i){
      info->updateChain(edges[i].first, edges[i].second, edges[i].getW());
    });
    if(!no_cache){
    parlay::parallel_for(0,n,[&](int cid){
      cacheTables->insert_helper(cid, edges[cid].second,  edges[cid].getW(), cacheTables->getTable(cid), cacheTables->getTable(edges[cid].second));
    });
    }
  }
}; // finder end

}
}
}
}