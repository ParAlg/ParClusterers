#pragma once

#include <atomic>
#include "unionfind.h"
#include "linkage_types.h"
#include "matrix.h"
#include "kdtree.h"
#include "treeUtilities.h"
#include "cacheUtilities.h"

#include "parlay/sequence.h"
#include "parlay/parallel.h"
#include "parlay/primitives.h"

using namespace std;

#define ALLOCDOUBLE
#define LARGER_THAN_UB numeric_limits<double>::max()
// #define CHECK_TOKEN -1
// #define UNFOUND_TOKEN -2
#define MAX_CACHE_TABLE_SIZE_INIT 64
#define LINKAGE_LOADFACTOR 2.0
// #define CHECK_NO_CACHE(i) if(no_cache){cout << "no cache " << i << endl; exit(1);}
#define CHECK_NO_CACHE(i)

namespace research_graph {
namespace in_memory {
namespace internal {

namespace HACTree {

template<int dim, class distF, class Fr, class M>
class TreeNNFinder { //: public NNFinder<dim>
  public:
  // typedef iPoint<dim> pointT;
  typedef iPoint<dim> pointT;
  // typedef typename FINDNN::CLinkNodeInfo nodeInfo;
  typedef typename Fr::nodeInfo nodeInfo;
  // typedef FINDNN::AveLinkNodeInfo<dim, pointT *> nodeT;
  typedef Node<dim> nodeT;
  typedef HACTree::node<dim, pointT, kdNodeInfo > kdnodeT;
  typedef HACTree::tree<dim, pointT, kdNodeInfo > treeT;
  // typedef typename Fr::kdnodeT kdnodeT;
  // typedef typename Fr::kdtreeT treeT;
  typedef LDS::distCacheT distCacheT;
  typedef LDS::hashClusterAveET hashClusterAveET;
  typedef int intT;

  bool no_cache;
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

  CacheTables<nodeT> *cacheTables;
  // distCacheT **cacheTbs; // distance to clusters, store two copies of each distance
  // int hashTableSize=0;
  // distCacheT::eType *TA;

  parlay::sequence<nodeT> nodes; // preallocated space to store tree nodes
  parlay::sequence<nodeT *> activeNodes; // pointers to the active nodes, same order as activeClustersTODO
  edgeComparator2 EC2;
  double eps;
  atomic<intT> nodeIdx; // the index of next node to use for cluster trees

  distF *distComputer;
  parlay::sequence<pointT> centers; //used to rebuild kd-tree
  // M marker;

  intT NAIVE_THRESHOLD = 50;
  // intT MAX_CACHE_TABLE_SIZE = 128;

  TreeNNFinder(intT t_n, point<dim>* t_P, UnionFind::ParUF<intT> *t_uf, distF *_distComputer, 
    bool t_noCache, double t_eps, intT t_naive_thresh, intT t_cache_size): 
    uf(t_uf), n(t_n), no_cache(t_noCache), eps(t_eps), NAIVE_THRESHOLD(t_naive_thresh){
    EC2 = LDS::edgeComparator2(eps);
    C = n;
    activeClusters = parlay::sequence<int>(n, [&](int i){return i;});
    PP = parlay::sequence<pointT>(n, [&](int i){return pointT(t_P[i], i);});
    centers = parlay::sequence<pointT>(n, [&](int i){return PP[i];});

    // distComputer = new distF(PP, n, no_cache);
    distComputer = _distComputer;

    rootIdx = parlay::sequence<int>(n, [&](int i){return i;});
    nodes = parlay::sequence<nodeT>(2*n);
    // distComputer->initNodes(nodes.data(), n);
    parlay::parallel_for(0,n,[&](int i){nodes[i] = nodeT(i, t_P[i]);});
    
    flag = parlay::sequence<bool>(C);
    newClusters = parlay::sequence<int>(C);

    // edges =parlay::sequence<LDS::EDGE>(n, [&](int i){return LDS::EDGE();});
    edges = (EGDE *)aligned_alloc(sizeof(EDGE), n*sizeof(EDGE));
    parlay::parallel_for(0,n,[&](int i){edges[i] = EDGE();});

    cacheTables = new CacheTables(no_cache, n, t_cache_size);

    nodeIdx.store(n); // have used n nodes
    kdtree = build(PP);
    // marker = M(uf, nodes, rootIdx);

  }

  ~TreeNNFinder(){
    // if(!no_cache){
    //   parallel_for(intT i=0; i<2*n; ++i) {
    //     delete cacheTbs[i];
    //   }
    //   free(TA);
    //   free(cacheTbs);
    // }
    delete cacheTables;
    delete kdtree;
    free(edges);
    // delete distComputer;
  }

  inline intT cid(nodeT* node){ return node->cId;}
  inline intT idx(nodeT* node){ return node->idx;}
  inline nodeT *getNode(intT cid){return nodes+rootIdx[cid];}
  inline distCacheT *getTable(intT cid){return cacheTbs[rootIdx[cid]];}
  inline intT idx(intT cid){return idx(getNode(cid));}
  inline intT leftIdx(intT cid){return getNode(cid)->left->idx;}
  inline intT rightIdx(intT cid){return getNode(cid)->right->idx;}
  inline intT cid(intT idx){return cid(nodes[idx]);}


  inline double getDistNaive(nodeT *inode,  nodeT *jnode, double lb = -1, double ub = numeric_limits<double>::max(), bool par = true){
    return distComputer->getDistNaive(inode, jnode, lb, ub, par);
  }

  // inline double getDistNaive(intT i, intT j, double lb = -1, double ub = numeric_limits<double>::max(), bool par = true){
  //   return distComputer->getDistNaive(i, j, lb, ub, par);
  // }

  // inline double getDistNaiveDebug(intT i, intT j){
  //   return distComputer->getDistNaive(getNode(i), getNode(j), -1,numeric_limits<double>::max(), false );
  // }

  // i, j are cluster ids
  // find distance in table 
  // if not found, compute by bruteforce and insert 
  // true if found in table
  // used in merging 
  // range search has its own updateDist
  tuple<double, bool> getDist(intT i,  intT j, double lb = -1, double ub = numeric_limits<double>::max(), bool par = true){
    if(!no_cache){
    double d = cacheTables->find(i, j);
    // if(d == CHECK_TOKEN){cout << "find check token" << endl; exit(1);} // might find is in singleNN step in getNN
    if(d != UNFOUND_TOKEN && d != CHECK_TOKEN) return make_tuple(d, true);
    }
    // if(distComputer->id_only) return make_tuple(getDistNaive(i,  j, lb, ub, par), false);
    return make_tuple(getDistNaive(getNode(i),  getNode(j), lb, ub, par), false);
  }

  tuple<double, bool> getDist(nodeT *inode,  nodeT *jnode, double lb = -1, double ub = numeric_limits<double>::max(), bool par = true){
    if(!no_cache){
    double d = cacheTables->find(inode, jnode);
    // if(d == CHECK_TOKEN){cout << "find check token" << endl; exit(1);} // might find is in singleNN step in getNN
    if(d != UNFOUND_TOKEN && d != CHECK_TOKEN) return make_tuple(d, true); 
    }
    return make_tuple(getDistNaive(inode, jnode, lb, ub, par), false);
  }

  //newc is a newly merged cluster
  // newc is new, rid is old
  inline double getNewDistO(intT newc, intT rid){
    nodeT* ql = getNode(newc)->left;
    intT nql = ql->n;
    nodeT* qr = getNode(newc)->right;
    intT nqr = qr->n;
    nodeT* rroot = getNode(rid);
    intT nr = rroot->n;
    double dij = getNode(newc)->getHeight();

    // double n1 = (double)nql * (double)nr;
    // double n2 = (double)nqr * (double)nr;
    // double alln = n1 + n2 ;

    double d1,d2; bool intable;
    tie(d1, intable) = getDist(ql,rroot);
    // d1 = n1 / alln * d1;

    tie(d2, intable) = getDist(qr,rroot);
    // d2 = n2 / alln * d2;

    // return d1 + d2;
    return distComputer->updateDistO(d1, d2, nql, nqr, nr, dij);
  }


  //newc is a newly merged cluster
  // newc is new, rid is merged
  //todo: cids on cluster tree do not need to be marked
  // overwrite entry?
  // TODO: consider changing hashtable to idx -> (idx, dist)
  inline double getNewDistN(intT newc, intT rid){
    nodeT* ql = getNode(newc)->left;
    intT nql = ql->n;
    nodeT* qr = getNode(newc)->right;
    intT nqr = qr->n;
    double dij = getNode(newc)->getHeight();

    nodeT* rl = getNode(rid)->left;
    intT nrl = rl->n;
    nodeT* rr = getNode(rid)->right;
    intT nrr = rr->n;
    double dklr = getNode(rid)->getHeight();

    double d1,d2, d3, d4; bool intable;
    tie(d1, intable) = getDist(ql,rl);
    // d1 = n1 / alln * d1;

    tie(d2, intable) = getDist(ql,rr);
    // d2 = n2 / alln * d2;

    tie(d3, intable) = getDist(qr,rl);
    // d3 = n3 / alln * d3;

    tie(d4, intable) = getDist(qr,rr);
    // d4 = n4 / alln * d4;

    // return d1  + d2  + d3 + d4;
    return distComputer->updateDistN(d1, d2, d3, d4, nql, nqr, nrl, nrr, dij, dklr);
  }

  // store the closest nn in edges
  // assume edges[cid] already has max written
  void getNN_naive(intT cid, double ub = numeric_limits<double>::max(), intT t_nn = -1){
    utils::writeMin(&edges[cid], LDS::EDGE(cid, t_nn, ub), EC2); 
    parallel_for(intT i = 0; i < C; ++i){
      intT cid2 = activeClusters[i];
      if(cid2 != cid){//if(cid2 < cid) { // the larger one might not be a terminal node only work if C == |terminal node|
        double tmpD;
        bool intable;
        tie(tmpD, intable) = getDist(cid, cid2, -1, edges[cid].getW(), true);
        utils::writeMin(&edges[cid], LDS::EDGE(cid,cid2,tmpD), EC2);
        utils::writeMin(&edges[cid2], LDS::EDGE(cid2,cid,tmpD), EC2); 
        if((!intable) && (!no_cache) && (tmpD != LARGER_THAN_UB)){
          cacheTables->insert(cid, cid2, tmpD);
        }
      }
    }
    // return make_pair(-1, -1); 
  }

  // store the closest nn in edges
  // assume edges[cid] already has max written
  inline void getNN(intT cid, double ub = numeric_limits<double>::max(), intT t_nn = -1){
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
    intT nn = t_nn;
    bool intable;
    Node<dim> query = getNode(cid);

    if(ub == numeric_limits<double>::max()){
        typedef FINDNN::NNsingle<dim, kdnodeT> Fs;
        Fs fs = Fs(uf, cid, eps);
        // if(distComputer->nn_process){
        //   distComputer->getRadius(cid, kdtree->root, &fs);
        // }else{
        pointT centroid = pointT(query->center, cid);
        treeT treetmp = treeT(&centroid, 1, false);
        // closest to a single point in cluster
        FINDNNP::dualtree<kdnodeT, Fs>(treetmp.root, kdtree->root, &fs);
        // }
        
        nn = uf->find(fs.e->second);
        tie(minD, intable) = getDist(cid, nn); 
#ifdef DEBUG
        if(minD==LARGER_THAN_UB){
          cout << "minD is inifinity" << endl;
          exit(1);
        }
#endif
        if((!intable) && (!no_cache)  && (minD != LARGER_THAN_UB)){
          insert(cid, nn, minD);
        }
    }

    utils::writeMin(&edges[cid], LDS::EDGE(cid, nn, minD), EC2); 
    utils::writeMin(&edges[nn], LDS::EDGE(nn, cid, minD), EC2);
    if(minD ==0){
      return;
    }
    double r = distComputer->getBall(query, minD+eps); //TODO: changed to dist, need to set r in range function

    Fr fr = Fr(uf, cid, r, nodes, rootIdx, cacheTbs, edges, distComputer, no_cache, C, eps); 
    HACTree::rangeTraverse<dim, iPoint<dim>, kdnodeT, Fr>(kdtree->root, query->center, r, &fr);

    if(fr.local){ //TODO: optimize out to simplify code
    nn = fr.getFinalNN();
    minD = fr.getFinalDist();
    utils::writeMin(&edges[cid], LDS::EDGE(cid, nn, minD), EC2); 
    }
  }

    // merge two clusters in dendrogram
    // u, v are cluster ids
  inline void merge(intT u, intT v, intT newc, intT round, double height){
    intT rootNodeIdx = nodeIdx.fetch_add(1);
    nodes[rootNodeIdx] = nodeT(newc, round, rootNodeIdx, getNode(u), getNode(v), height);
    rootIdx[newc] = rootNodeIdx;
  }

  inline void updateDist(intT newc){
    CHECK_NO_CACHE(497)
    intT idx1 = leftIdx(newc);
    intT idx2 = rightIdx(newc);
    intT cid1 = getNode(newc)->left->cId;
    intT cid2 = getNode(newc)->right->cId;

    auto tb1 = cacheTables->getTable(idx1);//cacheTbs[idx1];
    auto tb2 = cacheTables->getTable(idx2);//cacheTbs[idx2];

    auto TAR1 = tb1->entries();
    auto TAR2 = tb2->entries();
    // if(C < 100)cout << "sizes in merge: " << TAR1.n+TAR2.n << "/" << tb1->m + tb2->m << endl;
    auto newtb = cacheTables->getTable(idx(newc));//cacheTbs[idx(newc)];

    parlay::parallel_for(0, (TAR1.size()+TAR2.size()), [&](int i){
      distCacheT::eType *TA = TAR1.data();
      intT offset = 0;
      if(i >= TAR1.size()){ //switch to process tb2
        TA = TAR2.data();
        offset = TAR1.size();
      }
      intT j = i-offset;
      intT newc2 = -1;
      intT storedIdx = TA[j].idx; 
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
          cacheTables->insert_helper(newc, newc2, d, newtb, cacheTbs[idx(newc2)]);
        }
      }

    });
    // free(TAR1.A);
    // free(TAR2.A);
  }

  // find new activeClusters array based on uf, update C
  inline void updateActiveClusters(int round){
#ifdef DEBUG
    UTIL::PrintVec(activeClusters, C);
#endif
    
    parlay::parallel_for(0,C,[&](int i){
      int cid  = activeClusters[i];
      flag[i] = (uf->find(cid)==cid);
    });
// chainNum = parlay::pack_into(make_slice(finder->activeClusters).cut(0,C), flag, terminal_nodes);
    
    C = parlay::pack_into(make_slice(activeClusters).cut(0,C), flag, make_slice(newClusters).cut(0,C));
    swap(activeClusters, newClusters);
    // moved to distComputer->update();
    // if(marker.doMark(C, round)){ //TODO: move marker to distComputation
    //   FINDNN::singletree<kdnodeT, M, typename M::infoT>(kdtree->root, &marker, marker.initVal);
    // }

    if(distComputer->doRebuild()){
      parlay::parallel_for(0, C, [&](int i){
        int cid = activeClusters[i];
        centers[i] = pointT(getNode(cid)->center, cid); //consider making an array of active node pointers
      });
      kdtree->kdTreeRebuild(centers, C);
    }

#ifdef DEBUG
    cout <<  "pack activeClusters" << endl;
    UTIL::PrintVec(activeClusters, C);
#endif
  }

  // edges[i] stores the ith nn  of point i
  // initialize the chain in info
  inline void initChain(TreeChainInfo<dim> *info){
    typedef AllPtsNN<kdnodeT> F;
    F *f = new F(edges, eps);
    FINDNNP::dualtree<kdnodeT, F>(kdtree->root, kdtree->root, f, false);
    parlay::parallel_for(0,n,[&](int i){
      info->updateChain(edges[i].first, edges[i].second, edges[i].getW());
    });
#ifdef DEBUG
    UTIL::PrintVec2<LDS::EDGE>(edges, n);
#endif
    delete f;
    if(!no_cache){
    parlay::parallel_for(0,n,[&](int cid){
      cacheTables->insert_helper(cid, edges[cid].second,  edges[cid].getW(), cacheTbs[cid], cacheTbs[edges[cid].second]);
    });
    }
  }
}; // finder end

}
}
}
}