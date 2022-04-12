#pragma once

#include <atomic>
#include "utils/unionfind.h"
#include "utils/chain.h"

#include "parlay/sequence.h"
#include "parlay/parallel.h"
#include "parlay/primitives.h"

#include "finder.h"
#include "utils/dendro.h"
#include "dist.h"

using namespace std;

namespace research_graph {
namespace in_memory {
namespace internal {
namespace HACTree {

template<class TF>
inline void chain_find_nn(int chainNum, TF *finder, TreeChainInfo *info){
  parlay::parallel_for(0, chainNum, [&](int i){
      int cid = info->terminal_nodes[i];
      finder->edges[cid] = EDGE(-1,-1,numeric_limits<double>::max());
  });

  parlay::parallel_for(0, chainNum, [&](int i){
    int cid = info->terminal_nodes[i];
    pair<int, double> prev = info->getChainPrev(i);
    finder->getNN(cid, prev.second, prev.first);
  },1);
  parlay::parallel_for(0, chainNum, [&](int i){
      int cid = info->terminal_nodes[i];
      info->updateChain(cid, finder->edges[cid].second, finder->edges[cid].getW());
  });
}

template<class TF>
inline void link_terminal_nodes(UnionFind::ParUF<int> *uf, TF *finder, TreeChainInfo *info, int round, parlay::sequence<int>& flags){
  int chainNum = info->chainNum;
  EDGE *edges = finder->edges;
  
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
      finder->merge(cid, nn, newc, round, edges[cid].getW());
      flags[i] = newc;
    }else{
      flags[i] = NO_NEIGH;
    }
  });
  
  if(!finder->no_cache){
  auto merged = parlay::filter(flags.cut(0, chainNum), [&](int i){return i!=NO_NEIGH;});

#ifdef TIMING2
	 if(LINKAGE_DOPRINT(round)){ UTIL::PrintSubtimer(":::merge", t1.next());  cout << "merged.n: " << merged.n << endl;}
#endif
  // insert to new hashtables and delete old hashtables
  parlay::parallel_for(0, merged.size(), [&](int i){
    int newc = merged[i];
    finder->updateDist(newc, round);
  });
#ifdef TIMING2
	if(LINKAGE_DOPRINT(round)){ UTIL::PrintSubtimer(":::update dist", t1.next());}
#endif
  } //  end !no_cache

}

// TODO: proceed in stages to save work
template<int dim, class TF>
vector<dendroLine> chain_linkage(TF *finder){
  // cout << "hash table size " << finder->hashTableSize << endl;
  
  UnionFind::ParUF<int> *uf = finder->uf;
  int n = finder->n;
  int chainNum = n;
  TreeChainInfo *info = new TreeChainInfo(n, finder->eps);
  parlay::sequence<int> flags = parlay::sequence<int>(chainNum);// used in linkterminalnodes

#ifdef VERBOSE
 ofstream file_obj;
 file_obj.open("debug/1k_comp.txt"); 
#endif

  int round = 0;
  // bool print = false;
  while(finder->C > 1 ){
    round ++;
#ifdef VERBOSE
    // if(LINKAGE_DOPRINT(round)){//
    // print = true;
    std::cout << endl;
    std::cout << "Round " << round << endl;
    std::cout << "Comp Num " <<  finder->C << endl;
    std::cout << "Chain # " <<  info->chainNum << endl;//}else{print = false;}
  // if(round >= 10)  exit(1);
#endif
  // if(round >= 2 && info->chainNum == 0) zero_chain_debug(finder, round, info); 
  if(round >= n){
      std::cerr << "too many rounds" << std::endl;
      exit(1);
  }
  if(round == 1){
    finder->initChain(info);
  }else{
    chain_find_nn<TF>(chainNum, finder, info);
  }

#ifdef VERBOSE
   for(int i = 0; i < finder->C; ++i){
     file_obj << round << " " << finder->activeClusters[i] << endl;
   }
   file_obj << round << "========" << endl;
    for(int i = 0; i < chainNum; ++i){
      int cid = info->terminal_nodes[i];
      file_obj << round << " " << cid << " " << finder->edges[cid].second << endl;//<< " " << finder->edges[cid].getW() 
    }

   file_obj << round << "========" << endl;
#endif

    link_terminal_nodes<TF>(uf, finder, info, round, flags);
    // get ready for next round
    finder->updateActiveClusters(round);
    finder->distComputer->update(round, finder);
    info->next(finder, round);
    chainNum = info->chainNum;
//   t1.next();
  }
  // UTIL::PrintFunctionItem("CLINK", "rounds", round);
  delete info;
  // finder->distComputer->postProcess(finder);

  return formatDendrogram<dim>(finder->nodes, n, 0);
}
}//end of namespace HACTree

  template<int dim>
  vector<HACTree::dendroLine> runCompleteHAC(parlay::sequence<HACTree::iPoint<dim>>& P, bool no_cache, int cache_size=32, double eps = 0){
    std::size_t n = P.size();
    UnionFind::ParUF<int> *uf = new UnionFind::ParUF<int>(n, true);
    using distT = HACTree::distComplete<dim>;
    using F = HACTree::RangeQueryCountF<dim, HACTree::iPoint<dim>, distT>;
    using TF = HACTree::NNFinder<dim, distT, F>;
    distT *dist = new distT(uf, P.data());
    TF *finder = new TF(n, P.data(), uf, dist, no_cache, cache_size, eps); //a no cache finder
    vector<HACTree::dendroLine> dendro = chain_linkage<dim, TF>(finder);
    delete finder; delete dist; delete uf;
    return dendro;
  }

  template<int dim>
  vector<HACTree::dendroLine> runAVGHAC(parlay::sequence<HACTree::iPoint<dim>>& P, bool no_cache, int cache_size=32, double eps = 0){
    std::size_t n = P.size();
    UnionFind::ParUF<int> *uf = new UnionFind::ParUF<int>(n, true);
    using distT = HACTree::distAverage<dim>;
    using F = HACTree::RangeQueryCenterF<dim, HACTree::iPoint<dim>, distT>;
    using TF = HACTree::NNFinder<dim, distT, F>;
    distT *dist = new distT(P.data(), n);
    TF *finder = new TF(n, P.data(), uf, dist, no_cache, cache_size, eps); //a no cache finder
    vector<HACTree::dendroLine> dendro = chain_linkage<dim, TF>(finder);
    delete finder; delete dist; delete uf;
    return dendro;
  }

  template<int dim>
  vector<HACTree::dendroLine> runAVGSQHAC(parlay::sequence<HACTree::iPoint<dim>>& P, bool no_cache, int cache_size=32, double eps = 0){
    std::size_t n = P.size();
    UnionFind::ParUF<int> *uf = new UnionFind::ParUF<int>(n, true);
    using distT = HACTree::distAverageSq<dim>;
    using F = HACTree::RangeQueryCenterF<dim, HACTree::iPoint<dim>, distT>;
    using TF = HACTree::NNFinder<dim, distT, F>;
    distT *dist = new distT();
    TF *finder = new TF(n, P.data(), uf, dist, no_cache, cache_size, eps); //a no cache finder
    vector<HACTree::dendroLine> dendro = chain_linkage<dim, TF>(finder);
    delete finder; delete dist; delete uf;
    return dendro;
  }

  template<int dim>
  vector<HACTree::dendroLine> runWARDHAC(parlay::sequence<HACTree::iPoint<dim>>& P, bool no_cache, int cache_size=32, double eps = 0){
    std::size_t n = P.size();
    UnionFind::ParUF<int> *uf = new UnionFind::ParUF<int>(n, true);
    using distT = HACTree::distWard<dim>;
    using F = HACTree::RangeQueryCenterF<dim, HACTree::iPoint<dim>, distT>;
    using TF = HACTree::NNFinder<dim, distT, F>;
    distT *dist = new distT();
    TF *finder = new TF(n, P.data(), uf, dist, no_cache, cache_size, eps); //a no cache finder
    vector<HACTree::dendroLine> dendro = chain_linkage<dim, TF>(finder);
    delete finder; delete dist; delete uf;
    return dendro;
  }

}}}