// A "history independent" hash table that supports insertion, and searching
// It is described in the paper
//   Julian Shun and Guy E. Blelloch
//   Phase-concurrent hash tables for determinism
//   SPAA 2014: 96-107
// Insertions can happen in parallel
// Searches can happen in parallel
// each of the three types of operations have to happen in phase

#pragma once

#include "utils.h"
#include "parlay/sequence.h"
#include "parlay/primitives.h"
#include "parlay/utilities.h"


using namespace std;

extern int g_dim;

#ifndef A_HASH_LINKAGE_PROB
#define A_HASH_LINKAGE_PROB
#endif

#ifndef A_HASH_LINKAGE_PROBE_THRESH
#define A_HASH_LINKAGE_PROBE_THRESH (m)
#endif

namespace research_graph {
namespace in_memory {
namespace internal{
namespace HACTree{

template <class HASH>
class Table {
 public:
  typedef typename HASH::eType eType;
  typedef typename HASH::kType kType;
  size_t m;
  eType empty;
  HASH hashStruct;
  eType* TA;
  double load=1;
  using index = size_t;
  using intT = size_t;
  bool is_full;

  static void clearA(eType* A, intT n, eType v) {
    auto f = [&](size_t i) { parlay::assign_uninitialized(A[i], v); };
    parlay::parallel_for(0, n, f, parlay::granularity(n));
  }

  struct notEmptyF { 
    eType e; notEmptyF(eType _e) : e(_e) {} 
    int operator() (eType a) {return e != a;}};

  index hashToRange(index h) { return static_cast<index>(static_cast<size_t>(h) % m); }
  index firstIndex(kType v) { return hashToRange(hashStruct.hash(v)); }
  index incrementIndex(index h) { return (h + 1 == m) ? 0 : h + 1; }
  index decrementIndex(index h) { return (h == 0) ? m - 1 : h - 1; }
  bool lessIndex(index a, index b) {
    return (a < b) ? (2 * (b - a) < m) : (2 * (a - b) > m);
  }
  bool lessEqIndex(index a, index b) { return a == b || lessIndex(a, b); }


  // Constructor that takes an array for the hash table space.  The
  // passed size must be a power of 2 and will not be rounded.  Make
  // sure to not call del() if you are passing a pointer to the middle
  // of an array.
 Table(intT size, eType* _TA, HASH hashF, bool clear = false) :
    m(size), 
    // m(100 + static_cast<size_t>(load * size)),
    // mask(m-1),
    empty(hashF.empty()),
    hashStruct(hashF), 
    TA(_TA),
    is_full(false)
      { 
        if(clear)clearA(TA,m,empty); 
      }

  //for equal keys, first one to arrive at location wins, linear probing
  bool insert(eType v) {
    // if(is_full) return 0;
    kType vkey = hashStruct.getKey(v);
    intT h = firstIndex(vkey);
    intT prob_ct = 0;
    while (true) {
      eType c;
      c = TA[h];
      // intT cmp;
      if(c==empty && utils::CAS(&TA[h],c,v)) return 1; 
      else if(0 == hashStruct.cmp(vkey,hashStruct.getKey(c))) {
	if(!hashStruct.replaceQ(v,c))
	  return 0;
	else if (utils::CAS(&TA[h],c,v)) return 1;
      }
      // move to next bucket
      h = incrementIndex(h);
#ifdef A_HASH_LINKAGE_PROB
      prob_ct++;
      if(prob_ct > A_HASH_LINKAGE_PROBE_THRESH){
#ifdef A_HASH_LINKAGE_PROB_PRINT
        cout << "maximum prob in ndHash.h 1, m:" << m  << endl;
#endif
        // exit(1);
        is_full = true;
        return 0; 
      }
#endif
    }
    return 0; // should never get here
  }

  // give location in replaceQ
  // if replace CAS fail, will return fail to insert
  // if multiple updates, arbitrary one succeed
  bool insert_one_update(eType v) {
    // if(is_full) return 0; can't add this, might update
    kType vkey = hashStruct.getKey(v);
    intT h = firstIndex(vkey);
    intT prob_ct = 0;
    while (true) {
      eType c;
      c = TA[h];
      // intT cmp;
      if(c == empty && utils::CAS(&TA[h],c,v)){ 
        return 1; 
      }else if(0 == hashStruct.cmp(vkey,hashStruct.getKey(c))) {
        if(!hashStruct.replaceQ(v,TA, h, c)){
          return 0;
        }else if (utils::CAS(&TA[h],c,v)){ 
          return 1;
        }
        return 0;  // if multiple updates, arbitrary one succeed
      }
      // move to next bucket
      h = incrementIndex(h);
#ifdef A_HASH_LINKAGE_PROB
      prob_ct++;
      if(prob_ct > A_HASH_LINKAGE_PROBE_THRESH){
#ifdef A_HASH_LINKAGE_PROB_PRINT
        cout << "maximum prob in ndHash.h 2, m:" << m  << endl;
#endif
         // exit(1);
        is_full = true;
        return 0; 
      }
#endif
    }
    return 0; // should never get here
  }

  // return <success, reached threshold>, same as insert_one_update
  tuple<bool, bool> insert_thresh(eType v) {
    // if(is_full) return make_tuple(0, true);  can't add this, might update or return found
    kType vkey = hashStruct.getKey(v);
    intT h = firstIndex(vkey);
    intT prob_ct = 0;
    while (1) {
      eType c;
      c = TA[h];
      // intT cmp;
      if(c == empty && utils::CAS(&TA[h],c,v)){ 
        return make_tuple(1, false); 
      }else if(0 == hashStruct.cmp(vkey,hashStruct.getKey(c))) {
        if(!hashStruct.replaceQ(v,TA, h, c)){
          return make_tuple(0, false);
        }else if (utils::CAS(&TA[h],c,v)){ 
          return make_tuple(1, false);
        }
        return make_tuple(0, false);  // if multiple updates, arbitrary one succeed
      }
      // move to next bucket
      h = incrementIndex(h);

      // probing
      prob_ct++;
      if(prob_ct > A_HASH_LINKAGE_PROBE_THRESH){
#ifdef A_HASH_LINKAGE_PROB_PRINT
        cout << "maximum prob in ndHash.h 5, m:" << m  << endl;
#endif
         // exit(1);
        is_full = true;
        return make_tuple(0, true); 
      }
    }
    return make_tuple(0, false); // should never get here
  }

  // Returns the value if an equal value is found in the table
  // otherwise returns the "empty" element.
  // due to prioritization, can quit early if v is greater than cell
  eType find(kType v) {
    intT h = firstIndex(v);
    eType c = TA[h]; 
    intT prob_ct = 0;
    while (1) {
      if (c == empty) return empty; 
      else if (!hashStruct.cmp(v,hashStruct.getKey(c)))
	return c;
      h = incrementIndex(h);
      c = TA[h];
#ifdef A_HASH_LINKAGE_PROB
      prob_ct++;
      if(prob_ct > A_HASH_LINKAGE_PROBE_THRESH){
        // cout << "maximum prob in ndHash.h 6, m:" << m  << endl;
         // exit(1);
        return empty; 
      }
#endif
    }
  }

  // <result, reached threshold>
  tuple<eType, bool> find_thresh(kType v) {
    intT h = firstIndex(v);
    eType c = TA[h]; 
    intT prob_ct = 0;
    while (1) {
      if (c == empty) return make_tuple(empty, false); 
      else if (!hashStruct.cmp(v,hashStruct.getKey(c)))
	return make_tuple(c, false); ;
      h = incrementIndex(h);
      c = TA[h];
      prob_ct++;
      if(prob_ct > A_HASH_LINKAGE_PROBE_THRESH){
        // cout << "maximum prob in ndHash.h 7, m:" << m  << endl;
         // exit(1);
        return make_tuple(empty, true); 
      }
    }
  }


  // returns the number of entries
   size_t count() {
    auto is_full = [&](size_t i) -> size_t { return (TA[i] == empty) ? 0 : 1; };
    return parlay::internal::reduce(parlay::delayed_seq<size_t>(m, is_full), parlay::addm<size_t>());
  }

    // returns all the current entries compacted into a sequence
    parlay::sequence<eType> entries() {
    return parlay::filter(parlay::make_slice(TA, TA+m),
                  [&] (eType v) { return v != empty; });
  }

  // prints the current entries along with the index they are stored at
  void print() {
    cout << "vals = ";
    for (intT i=0; i < m; i++) 
      if (TA[i] != empty)
	cout << i << ":" << TA[i] << ",";
    cout << endl;
  }
};

template <class intT>
struct hashInt {
  typedef intT eType;
  typedef intT kType;
  eType empty() {return -1;}
  kType getKey(eType v) {return v;}
  size_t hash(kType v) { return static_cast<size_t>(parlay::hash64(v)); }
  int cmp(kType v, kType b) {return (v > b) ? 1 : ((v == b) ? 0 : -1);}
  bool replaceQ(eType v, eType b) {return 0;}
};

//typedef Table<hashInt> IntTable;
//static IntTable makeIntTable(int m) {return IntTable(m,hashInt());}
template <class intT>
static Table<hashInt<intT>> makeIntTable(intT m, float load) {
  return Table<hashInt<intT>>(m,hashInt<intT>(),load);}

template <class KEYHASH, class DTYPE>
struct hashPair {
  KEYHASH keyHash;
  typedef typename KEYHASH::kType kType;
  typedef pair<kType,DTYPE>* eType;
  eType empty() {return NULL;}

  hashPair(KEYHASH _k) : keyHash(_k) {}

  kType getKey(eType v) { return v->first; }

  uint hash(kType s) { return keyHash.hash(s);}
  int cmp(kType s, kType s2) { return keyHash.cmp(s, s2);}

  bool replaceQ(eType s, eType s2) {
    return 0;}//s->second > s2->second;}
};

// static _seq<pair<char*,intT>*> removeDuplicates(_seq<pair<char*,intT>*> S) {
//   return removeDuplicates(S,hashPair<hashStr,intT>(hashStr()));}

struct hashSimplePair {
    typedef int intT;
    typedef unsigned int uintT;
  typedef pair<intT,intT> eType;
  typedef intT kType;
  eType empty() {return pair<intT,intT>(-1,-1);}
  kType getKey(eType v) { return v.first; }
 size_t hash(kType v) { return static_cast<size_t>(parlay::hash64(v)); }
  int cmp(intT v, intT b) {return (v > b) ? 1 : ((v == b) ? 0 : -1);}
  bool replaceQ(eType s, eType s2) {return 0;}//return s.second > s2.second;}
};


}
}
}
}