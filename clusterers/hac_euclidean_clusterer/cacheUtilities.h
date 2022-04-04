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


using namespace std;
namespace research_graph {
namespace in_memory {
namespace internal {
namespace HACTree {

struct CacheTables{
    typedef Node<dim> nodeT;

    static const int CHECK_TOKEN = -1;
    static const int UNFOUND_TOKEN = -1;

    nodeT* nodes;
    distCacheT **cacheTbs;
};


}}}}