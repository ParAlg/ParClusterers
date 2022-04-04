#pragma once

#include <atomic>
#include "unionfind.h"
#include "linkage_types.h"
#include "matrix.h"

#include "parlay/sequence.h"
#include "parlay/parallel.h"
#include "parlay/primitives.h"

#include "finder.h"

using namespace std;

namespace research_graph {
namespace in_memory {
namespace internal {


template<int dim, class TF>
inline void chain_find_nn(int chainNum, TF *finder, TreeChainInfo *info){
  parlay::parallel_for(0, chainNum, [&](int i){
      int cid = info->terminal_nodes[i];
      finder->edges[cid] = LDS::EDGE(-1,-1,numeric_limits<double>::max());
  });

  parlay::parallel_for(0, chainNum, [&](int i){
    int cid = info->terminal_nodes[i];
    pair<int, double> prev = info->getChainPrev(i);
    finder->getNN(cid, prev.second, prev.first);
  },1);
  parlay::parallel_for(0, chainNum, [&](int i){
      int cid = info->terminal_nodes[i];
      info->updateChain(cid, finder->edges[cid].second, finder->edges[cid].getW());
#ifdef DEBUG
  UTIL::PrintFunctionItem("NN", "cid", cid);
  UTIL::PrintFunctionItem("NN", "nn", finder->edges[cid].second);
  UTIL::PrintFunctionItem("NN", "w", finder->edges[cid].getW());
#endif
  });
}

template<int dim, class TF>
inline void link_terminal_nodes(UnionFind::ParUF<int> *uf, TF *finder, TreeChainInfo *info, int round, int *flags){
  int chainNum = info->chainNum;
  LDS::EDGE *edges = finder->edges.data();
  
#ifdef TIMING2
  timer t1; t1.start();
#endif

  parlay::parallel_for(0, chainNum, [&](int i){
    int cid = info->terminal_nodes[i];
    // int cid = uf->find(edges[i].first); 
    int nn = uf->find(edges[cid].second);
    int nn_of_nn =info->getNN(nn);
    // if( nn_of_nn> 0) nn_of_nn = uf->find(nn_of_nn); do not need because all clusters on chain did not merge in the last round

    // avoid merging twice
    // update chain info and merge trees
    if ((!(info->isTerminal(nn) && cid <= nn)) && (nn != cid && nn_of_nn == cid)){// if they are RNN
      int newc = uf->link(cid, nn, edges[cid].getW());
      info->invalidate(nn, NO_NEIGH);
      info->invalidate(cid, NO_NEIGH);
      info->invalidateRev(newc);
      finder->merge(cid, nn, newc, round, edges[cid].getW());
      flags[i] = newc;
    }else{
      flags[i] = NO_NEIGH;
    }
  });
  
  if(!finder->no_cache){
  auto merged = parlay::filter(make_slice(flags).cut(0, chainNum), [&](std::size_t i){return i!=NO_NEIGH;});

#ifdef TIMING2
	 if(LINKAGE_DOPRINT(round)){ UTIL::PrintSubtimer(":::merge", t1.next());  cout << "merged.n: " << merged.n << endl;}
#endif
  // insert to new hashtables and delete old hashtables
  parlay::parallel_for(0, merged.size(), [&](std::size_t i){
    int newc = merged[i];
    finder->updateDist(newc);
  });
#ifdef TIMING2
	if(LINKAGE_DOPRINT(round)){ UTIL::PrintSubtimer(":::update dist", t1.next());}
#endif
  } //  end !no_cache

}

// TODO: proceed in stages to save work
template<int dim, class TF>
inline void chain_linkage(TF *finder){
  cout << "hash table size " << finder->hashTableSize << endl;
  
  UnionFind::ParUF<int> *uf = finder->uf;
  int n = finder->n;
  int chainNum = n;
  TreeChainInfo *info = new TreeChainInfo(n, finder->eps);
  parlay::sequence<int> flagsSeq = parlay::sequence<int>(chainNum);
  int *flags = flagsSeq.data();// used in linkterminalnodes

#ifdef VERBOSE
	UTIL::PrintSubtimer("initialize", t1.next());
#endif
//  ofstream file_obj;
//  file_obj.open("debug/avg_UCI1_32_1th.txt"); //"+ to_string(round) + "

  int round = 0;
  bool print = false;
  while(finder->C > 1 ){
    round ++;
#ifdef VERBOSE
    if(LINKAGE_DOPRINT(round)){//
    print = true;
    UTIL::PrintBreak();
    UTIL::PrintFunctionItem("CLINK", "Round", round);
    UTIL::PrintFunctionItem("CLINK", "Comp Num", finder->C);
    UTIL::PrintFunctionItem("Chain", "#", info->chainNum);}else{print = false;}
#endif
  if(round >= n)  exit(1);
  if(round >= 2 && info->chainNum == 0) zero_chain_debug(finder, round, info); 

  if(round == 1){
    finder->initChain(info);
#ifdef VERBOSE
	if(print) UTIL::PrintSubtimer("init-chains", t1.next());
#endif
  }else{
    chain_find_nn(chainNum, finder, info);
    // findAllNNBruteForce(chainNum, finder, info);
#ifdef VERBOSE
	if(print) UTIL::PrintSubtimer("find-nn", t1.next());
#endif
  }
    link_terminal_nodes<dim, TF>(uf, finder, info, round, flags);
#ifdef VERBOSE
	if(print) UTIL::PrintSubtimer("link-update", t1.next());
#endif
      // get ready for next round
    finder->updateActiveClusters(round);
    finder->distComputer->update(round, finder);
    info->next(finder);
    chainNum = info->chainNum;
#ifdef VERBOSE
	if(print) UTIL::PrintSubtimer("update-clusters", t1.next());
#endif
//   t1.next();
  }
  UTIL::PrintFunctionItem("CLINK", "rounds", round);
  delete info;
  finder->distComputer->postProcess(finder);
}


}}}