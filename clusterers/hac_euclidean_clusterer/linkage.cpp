#include <string>
#include <vector>

#include "clusterer.h"
#include "dist.h"
#include "IO/pointIO.h"

// g++ -g -std=c++17 -ldl -pthread -I../../external/gbbs/external/parlaylib/include linkage.cpp -o linkage

using namespace std;
using namespace research_graph::in_memory::internal::HACTree;

int main(int argc, char *argv[]) {
    char* filename = argv[1];
    std::string output = argv[2];

    cout << "num workers: " << parlay::num_workers() << endl;

    auto P = pargeo::pointIO::readPointsFromFile<point<2>>(filename);
    int n = P.size();
    UnionFind::ParUF<int> *uf = new UnionFind::ParUF<int>(n, true);

    using distT = distAverageSq<2>;
    using F = RangeQueryCenterF<2, iPoint<2>, distT>;
    using TF = NNFinder<2, distT, F>;
    distT *dist = new distT();
    TF *finder = new TF(n, P.data(), uf, dist, true); //a no cache finder
    vector<dendroLine> dendro = chain_linkage<2, TF>(finder);

    ofstream file_obj;
    file_obj.open(output); 
    for(size_t i=0;i<n-1;i++){
        dendro[i].print(file_obj);
    }
    file_obj.close();

}
