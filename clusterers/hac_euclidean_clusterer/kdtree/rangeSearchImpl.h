#pragma once

#include "parlay/parallel.h"
#include "parlay/utilities.h"
#include "kdtree.h"

using namespace std;
namespace research_graph {
namespace in_memory {
namespace internal{
namespace HACTree {

    template <int dim, typename objT, typename nodeT, typename F>
    void rangeTraverse(nodeT *Q, objT center, double r, F* f) {
    int relation = Q->boxBallCompare(center, r, Q->pMin, Q->pMax);
    if(relation == Q->boxExclude) return;
    if (f->isComplete(Q)) return;

    if (Q->isLeaf()) {
        for(int i=0; i<Q->size(); ++i) {
            if (Q->itemInBall(center, r, Q->items[i])) {
                f->checkComplete(Q->items[i]);
            }
        }
    } else {
        if(f->Par(Q)){
            parlay::par_do([&](){
                rangeTraverse<dim, objT, nodeT, F>(Q->left, center, r, f);
            },[&](){    
                rangeTraverse<dim, objT, nodeT, F>(Q->right, center, r, f);
            });
        }else{
            rangeTraverse<dim, objT, nodeT, F>(Q->left, center, r, f);
            rangeTraverse<dim, objT, nodeT, F>(Q->right, center, r, f);
        }
    }
  }

}}}}