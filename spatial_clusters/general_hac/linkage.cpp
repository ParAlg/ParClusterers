#include <string>
#include <vector>

#include "linkage.h"
#include "../common/common.h"

// g++ -g -std=c++17 -ldl -pthread -I../../external/gbbs/external/parlaylib/include linkage.cpp -o linkage

using namespace std;


int main(int argc, char *argv[]) {
    char* filename = argv[1];
    std::size_t n = atoi(argv[2]);
    std::string output = argv[3];

    cout << "num workers: " << parlay::num_workers() << endl;

    research_graph::in_memory::SymMatrix<double> W = research_graph::in_memory::IO::readSymMatrixFromFile<double>(filename, n); 
    using distT = research_graph::in_memory::distComplete<double>;
    vector<research_graph::in_memory::dendroLine> dendro = research_graph::in_memory::chain_linkage_matrix<double, distT>(&W);

    ofstream file_obj;
    file_obj.open(output); 
    for(size_t i=0;i<n-1;i++){
        dendro[i].print(file_obj);
    }
    file_obj.close();

}
