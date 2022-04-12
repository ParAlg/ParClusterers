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
  // double load=1;
  using index = size_t;
  using intT = size_t;
  bool is_full;

  static void clearA(eType* A, intT n, eType v) {
    auto f = [&](size_t i) { A[i] = v; };
    parlay::parallel_for(0, n, f, parlay::granularity(n));
  }

  index hashToRange(index h) { return static_cast<index>(static_cast<size_t>(h) % m); }
  index firstIndex(kType v) { return hashToRange(hashStruct.hash(v)); }
  index incrementIndex(index h) { return (h + 1 == m) ? 0 : h + 1; }


  // Constructor that takes an array for the hash table space.  The
  // passed size must be a power of 2 and will not be rounded.  Make
  // sure to not call del() if you are passing a pointer to the middle
  // of an array.
 Table(intT size, eType* _TA, HASH hashF, bool clear = false) :
    m(size), 
    empty(hashF.empty()),
    hashStruct(hashF), 
    TA(_TA),
    is_full(false)
      { 
        if(clear)clearA(TA,m,empty); 
      }

  // return <success, reached threshold>
  // //for equal keys, first one to arrive at location wins, linear probing
  // if replace CAS fail, will return fail to insert
  // if multiple updates, arbitrary one succeed
  //  can't use priority linear probing, because we need to stop probing when table is full.
  // TODO: CHECK
  tuple<bool, bool> insert(eType v) {
    // if(is_full) return make_tuple(0, true);  can't add this, might update or return found
    kType vkey = hashStruct.getKey(v);
    intT h = firstIndex(vkey);
    intT prob_ct = 0;
    while (true) {
      eType c = TA[h];
      if(c == empty && utils::CAS(&TA[h],c,v)){ 
        return make_tuple(1, false); 
      }else if(0 == hashStruct.cmp(vkey,hashStruct.getKey(c))) {
        if(!hashStruct.replaceQ(v, c)){
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
        is_full = true;
        return make_tuple(0, true); 
      }
    }
    return make_tuple(0, false); // should never get here
  }

  // Returns the value if an equal value is found in the table
  // <result, reached threshold>
  tuple<eType, bool> find(kType v) {
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


}
}
}
}