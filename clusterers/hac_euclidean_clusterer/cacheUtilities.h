#pragma once

#include <limits>
#include <fstream>
#include <string>
#include <vector>

#include "utils/node.h"
#include "utils/utils.h"
#include "utils/hashtable_parallel.h"

#include "parlay/utilities.h"



using namespace std;
namespace research_graph {
namespace in_memory {
namespace internal {
namespace HACTree {

// #define CHECK_NO_CACHE(i) if(no_cache){cout << "no cache " << i << endl; exit(1);}
#define CHECK_NO_CACHE(i)
#define CHECK_TOKEN -1
#define UNFOUND_TOKEN -2

//   struct hashClusterId { 
//     typedef pair<int,int> eType;
//     typedef int kType;
//     eType empty() {return pair<int,int>(-1,-1);}
//     kType getKey(eType v) { return v.first; }
//     std::size_t hash(kType v) { return static_cast<std::size_t>(parlay::hash32((std::uint32_t)v)); }
//     int cmp(int v, int b) {return (v > b) ? 1 : ((v == b) ? 0 : -1);}
//     bool replaceQ(eType s, eType s2) {cout << "wrong replace Q" << endl;
//         exit(1);return 0;}//return s.second > s2.second;}
//     bool replaceQ(eType s, eType* TA, std::size_t h) {
//         utils::writeAdd(&(TA[h].second), s.second);
//     return false;}
//   };

  struct hashClusterET {
    volatile int first;
    volatile int idx;
    volatile double dist;
    hashClusterET():first(-1), idx(-1), dist(UNFOUND_TOKEN){}
    hashClusterET(int ii, int jj, double dd):first(ii), idx(jj), dist(dd){}
    void print(){
      cout << first << " " << idx << " " << dist << endl;
    }

    bool operator== (const hashClusterET& y) const
   { return first == y.first && idx == y.idx && dist == y.dist;//should only be used for detect empty, so no eps compare
   }

       bool operator!= (const hashClusterET& y) const
   {  return first != y.first || idx != y.idx || dist != y.dist;//should only be used for detect empty, so no eps compare
   }

  };

  struct hashCluster{ 
      typedef hashClusterET eType; // (cid, (idx, dist))
      typedef int kType;
      eType empty() {return eType(-1,-1,UNFOUND_TOKEN);}
      kType getKey(eType v) { return v.first; }
      std::size_t hash(kType v) { return static_cast<std::size_t>(parlay::hash32((std::uint32_t)v)); }
      int cmp(kType v, kType b) {return (v > b) ? 1 : ((v == b) ? 0 : -1);}
      bool replaceQ(eType s, eType s2) { // s is the new value
        if(s.dist == CHECK_TOKEN){ // insert check
          if(s.idx == s2.idx){ return false;} // already exist, no need to compute
          else{return true;}// wrong index, need to compute
        }
        // a real insert, replace
        return true;
        // if non TOKEN s is updating in rangeQuery, current entry must have been inserted TOKEN
        // or already exist  with valid idx, so this update cannot be overwritten by a TOKEN update
      }//return s.second > s2.second;}
  };

// TODO: consider changing hashtable to idx -> (idx, dist)
template<class nodeT>
struct CacheTables{
    typedef int intT;
    typedef Table<hashCluster> distCacheT;

    std::size_t CACHE_TABLE_SIZE = 64;
    std::size_t T_SIZE_INIT = 64;

    bool no_cache;
    std::size_t n;
    nodeT* nodes;
    int *rootIdx;
    distCacheT **cacheTbs; // distance to clusters, store two copies of each distance
    std::size_t hashTableSize=0;
    distCacheT::eType *TA;

    template<class Finder>
    CacheTables(bool _no_cache, int _n, int t_cache_size, Finder* finder):CACHE_TABLE_SIZE(t_cache_size), no_cache(_no_cache), n(_n){
        if(!no_cache){ //TODO: double check
            // hashTableSize = min(n, (1 << parlay::log2_up((uint)CACHE_TABLE_SIZE)));
            hashTableSize = min(n, CACHE_TABLE_SIZE);
            TA = (distCacheT::eType *) malloc(sizeof(distCacheT::eType)* ((std::size_t)n*2*hashTableSize));
            distCacheT::eType emptyval = hashCluster().empty();
            cacheTbs = (distCacheT **) malloc(sizeof(distCacheT *)*2*n);
            parlay::parallel_for(0,n,[&](std::size_t i){
                cacheTbs[i] = new distCacheT(min(hashTableSize, min(T_SIZE_INIT, n)), TA + i*hashTableSize, hashCluster(), true);
            });
            parlay::parallel_for(n,2*n,[&](std::size_t i){
                cacheTbs[i] = new distCacheT(min(hashTableSize, n), TA + i*hashTableSize, hashCluster()); //clear together below
            });

            parlay::parallel_for((std::size_t)n*hashTableSize,(std::size_t)n*2*hashTableSize,[&](std::size_t i){
                TA[i] = emptyval;
            });
        } // end of if ! no cache
        rootIdx = finder->rootIdx.data();
        nodes = finder->nodes.data();

    }

    ~CacheTables(){
      if(!no_cache){
      parlay::parallel_for(0,2*n,[&](int i) {
        delete cacheTbs[i];
      });
      free(TA);
      free(cacheTbs);
    }
    }

    inline int cid(nodeT* node){ return node->cId;}
    inline int idx(nodeT* node){ return node->idx;}
    inline int idx(int cid){return idx(getNode(cid));}
    inline nodeT *getNode(int cid){return nodes+rootIdx[cid];}
    distCacheT *getTable(int idx){return cacheTbs[idx];}

    // return UNFOUND_TOKEN if not found
    // return distance if found
    inline double find(intT qid, intT rid, intT qIdx = -1, intT  rIdx = -1){
    CHECK_NO_CACHE(259)
    if(qIdx == -1){
      qIdx = idx(qid);
      rIdx = idx(rid);
    }

    typename distCacheT::eType result;
    bool reach_thresh;
    tie(result, reach_thresh) = cacheTbs[qIdx]->find_thresh(rid);
    if(!reach_thresh && result.idx == rIdx){
	    return result.dist;
    }
    
    tie(result, reach_thresh) = cacheTbs[rIdx]->find_thresh(qid);
    if(!reach_thresh && result.idx == qIdx){
	    return result.dist;
    }
    return UNFOUND_TOKEN;
  }

    inline double find(nodeT *inode, nodeT *jnode){
	  return find(cid(inode), cid(jnode), idx(inode), idx(jnode));
  }

    inline void insert_helper(intT qid, intT rid, double d, distCacheT *tb1, distCacheT *tb2){
        if(d == LARGER_THAN_UB){
            return;
        }
        CHECK_NO_CACHE(284)
        intT qIdx = idx(qid);
        intT rIdx = idx(rid);
        tb1->insert_one_update(hashClusterET(rid, rIdx, d));
        tb2->insert_one_update(hashClusterET(qid, qIdx, d));
  }

    inline void insert(intT qid, intT rid, double d){
        if(d == LARGER_THAN_UB){
            return;
        }
        CHECK_NO_CACHE(291)
        intT qIdx = idx(qid);
        intT rIdx = idx(rid);

        cacheTbs[qIdx]->insert_one_update(hashClusterET(rid, rIdx, d));
        cacheTbs[rIdx]->insert_one_update(hashClusterET(qid, qIdx, d));
  }

     // return true only when insert if sucussful
    // neex the  do Swap
    // if one is old and one is new, always check tb[new]->old
    // because the one stored in old might be outdated
    // must have enough space due to alloc new
    // calculate distance if return true
  inline bool insert_check(intT qid, intT rid, bool doSwap, bool iffull){
      CHECK_NO_CACHE(473)
      if(doSwap && qid > rid){
          swap(qid, rid);
      }
      intT qIdx = idx(qid);
      intT rIdx = idx(rid);
      bool inserted; bool reach_thresh;
      tie(inserted, reach_thresh) = cacheTbs[qIdx]->insert_thresh(hashClusterET(rid, rIdx, CHECK_TOKEN));
      if(!reach_thresh) return inserted;

      if(doSwap){
        tie(inserted, reach_thresh) = cacheTbs[rIdx]->insert_thresh(hashClusterET(qid, qIdx, CHECK_TOKEN));
        if(!reach_thresh) return inserted;
      }
      
      return iffull;
  }

};


}}}}