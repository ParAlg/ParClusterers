#pragma once

#include <limits>
#include <fstream>
#include <string>
#include <vector>

#include "point.h"
#include "node.h"
#include "unionfind.h"
#include "utils.h"
#include "linkage_types.h"
#include "hashtable_parallel.h"
#include "hashtable_serial.h"

#include "parlay/utilities.h"

using namespace std;
namespace research_graph {
namespace in_memory {
namespace internal {
namespace HACTree {

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
    // hashClusterAveET(intT ii, cInfoET jj):first(ii), second(jj){}
    hashClusterET(int ii, int jj, double dd):first(ii), idx(jj), dist(dd){}
    void print(){
      cout << first << " " << idx << " " << dist << endl;
    }

    bool operator== (const hashClusterET& y) const
   { return first == y.first && idx == y.idx && dist == y.dist;// abs(dist - y.dist) < 1e-20; //todo: CHANGE? should only be used for detect empty
   }

       bool operator!= (const hashClusterET& y) const
   {  return first != y.first || idx != y.idx || dist != y.dist;// abs(dist - y.dist) > 1e-20;
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
      bool replaceQ(eType s, eType* TA, std::size_t h, eType s2) {
        if(s.dist == CHECK_TOKEN){ // insert check
          if(s.idx == s2.idx){ return false;} // already exist, no need to compute
          else{return true;}// wrong index, need to compute
        }
        // a real insert, replace
        return true;
      }
  };

template<class nodeT>
struct CacheTables{
    typedef int intT;
    typedef Table<hashCluster> distCacheT;

    static const int MAX_CACHE_TABLE_SIZE = 128;

    bool no_cache;
    int n;
    nodeT* nodes;
    distCacheT **cacheTbs; // distance to clusters, store two copies of each distance
    int hashTableSize=0;
    distCacheT::eType *TA;

    CacheTables(bool _no_cache, int _n, int t_cache_size):no_cache(_no_cache), n(_n), MAX_CACHE_TABLE_SIZE(t_cache_size){
        if(!no_cache){ //TODO: double check
            hashTableSize = min(n, 1 << utils::log2Up((uintT)(LINKAGE_LOADFACTOR*(uintT)MAX_CACHE_TABLE_SIZE)));
            TA = (distCacheT::eType *) malloc(sizeof(distCacheT::eType)* ((long)n*2*hashTableSize));
            distCacheT::eType emptyval = LDS::hashClusterAve().empty();
            cacheTbs = (distCacheT **) malloc(sizeof(distCacheT *), 2*n);
            parlay::parallel_for(0,n,[&](std::size_t i){
                cacheTbs[i] = new distCacheT(min(MAX_CACHE_TABLE_SIZE, min(MAX_CACHE_TABLE_SIZE_INIT, C)), TA + i*hashTableSize, LDS::hashClusterAve(), LINKAGE_LOADFACTOR, true);
            });
            parlay::parallel_for(n,2*n,[&](std::size_t i){
                cacheTbs[i] = new distCacheT(min(MAX_CACHE_TABLE_SIZE, C), TA + i*hashTableSize, LDS::hashClusterAve(), LINKAGE_LOADFACTOR);
            });

            parlay::parallel_for((std::size_t)n*hashTableSize,(std::size_t)n*2*hashTableSize,[&](std::size_t i){
                TA[i] = emptyval;
            });
        } // end of if ! no cache
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

    inline intT cid(nodeT* node){ return node->cId;}
    inline intT idx(nodeT* node){ return node->idx;}

    distCacheT *getTable(int idx){return cacheTables[idx];}

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
        tb1->insert_one_update(hashClusterAveET(rid, rIdx, d));
        tb2->insert_one_update(hashClusterAveET(qid, qIdx, d));
  }

    inline void insert(intT qid, intT rid, double d){
        if(d == LARGER_THAN_UB){
            return;
        }
        CHECK_NO_CACHE(291)
        intT qIdx = idx(qid);
        intT rIdx = idx(rid);

        cacheTbs[qIdx]->insert_one_update(hashClusterAveET(rid, rIdx, d));
        cacheTbs[rIdx]->insert_one_update(hashClusterAveET(qid, qIdx, d));
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
      tie(inserted, reach_thresh) = cacheTbs[qIdx]->insert_thresh(hashClusterAveET(rid, rIdx, CHECK_TOKEN));
      if(!reach_thresh) return inserted;

      if(doSwap){
        tie(inserted, reach_thresh) = cacheTbs[rIdx]->insert_thresh(hashClusterAveET(qid, qIdx, CHECK_TOKEN));
        if(!reach_thresh) return inserted;
      }
      
      return iffull;
  }

};


}}}}