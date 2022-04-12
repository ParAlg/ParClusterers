#include <string>
#include <vector>

#include "clusterer.h"
#include "dist.h"
#include "IO/pointIO.h"
#include "utils/point.h"

#include "parlay/primitives.h"

// g++ -g -std=c++20 -ldl -pthread -DVERBOSE -I../../external/gbbs/external/parlaylib/include linkage.cpp -o linkage
// g++ -std=c++20 -ldl -pthread -I../../external/gbbs/external/parlaylib/include linkage.cpp -o linkage


using namespace std;
using namespace research_graph::in_memory::internal::HACTree;

template<int dim>
vector<dendroLine> run(char* filename){
    bool no_cache = true;
    auto P0 = pargeo::pointIO::readPointsFromFile<point<dim>>(filename);
    parlay::sequence<iPoint<dim>> P = makeIPoint<dim>(P0);
    // make float precision to match parClusterer results
    parlay::parallel_for(0,P.size(),[&](int i){
        for (int d = 0; d < 2; ++d){
            P[i][d] = double(float( P[i][d] ));
        }
    });
    return research_graph::in_memory::internal::runCompleteHAC<dim>(P, no_cache);
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
    double checksum = getCheckSum(dendro);
    cout << "Cost: " << std::setprecision(10) << checksum << endl;

    ofstream file_obj;
    file_obj.open(output); 
    for(size_t i=0;i<n-1;i++){
        dendro[i].print(file_obj);
    }
    file_obj.close();

}
