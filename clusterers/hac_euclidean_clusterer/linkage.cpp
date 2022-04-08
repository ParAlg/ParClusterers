#include <string>
#include <vector>

#include "clusterer.h"
#include "dist.h"
#include "IO/pointIO.h"

#include "parlay/primitives.h"

// g++ -g -std=c++17 -ldl -pthread -DVERBOSE -I../../external/gbbs/external/parlaylib/include linkage.cpp -o linkage
// g++ -std=c++17 -ldl -pthread -I../../external/gbbs/external/parlaylib/include linkage.cpp -o linkage


using namespace std;
using namespace research_graph::in_memory::internal::HACTree;

template<int dim>
vector<dendroLine> run(char* filename){
    auto P = pargeo::pointIO::readPointsFromFile<point<dim>>(filename);
    int n = P.size();
    UnionFind::ParUF<int> *uf = new UnionFind::ParUF<int>(n, true);
    using distT = distAverageSq<dim>;
    using F = RangeQueryCenterF<dim, iPoint<dim>, distT>;
    using TF = NNFinder<dim, distT, F>;
    distT *dist = new distT();
    TF *finder = new TF(n, P.data(), uf, dist, true); //a no cache finder

    vector<dendroLine> dendro = chain_linkage<dim, TF>(finder);

    delete finder;
    delete uf;
    delete dist;
    
    return dendro;
}

int main(int argc, char *argv[]) {
    char* filename = argv[1];
    std::string output = argv[2];
    int dim = atoi(argv[3]);

    cout << "num workers: " << parlay::num_workers() << endl;
    
    vector<dendroLine> dendro;
    if(dim ==2){
        dendro = run<2>(filename);
    }else if(dim == 10){
        dendro = run<10>(filename);
    }else{
        cerr << "dim not supported" << endl;
    }
    
    int n = dendro.size()+1;

    double checksum = parlay::reduce(parlay::delayed_seq<double>(n-1, [&](size_t i){return dendro[i].height;}));

    cout << "Cost: " << std::setprecision(10) << checksum << endl;
    ofstream file_obj;
    file_obj.open(output); 
    for(size_t i=0;i<n-1;i++){
        dendro[i].print(file_obj);
    }
    file_obj.close();

}
