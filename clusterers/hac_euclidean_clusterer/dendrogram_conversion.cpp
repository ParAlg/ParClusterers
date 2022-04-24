#include <vector>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
// #include "utils/dendro.h"
#include "parlay/parallel.h"

using namespace std;
// g++ -std=c++17 -I../../external/gbbs/external/parlaylib/include dendrogram_conversion.cpp -o dendro
namespace research_graph {
namespace in_memory {

struct dendroLine{
    int id1;
    int id2;
    double height;
    int size;
    dendroLine(int _id1, int _id2, double _height, int _size):id1(_id1), id2(_id2), height(_height), size(_size){}
    dendroLine(){}

    void print(std::ofstream &file_obj){
        file_obj << id1 << " " << id2 << " " << std::setprecision(20) << height << " " << size << std::endl; 
    }

    void print(){
        std::cout << id1 << " " << id2 << " " << height << " " << size << std::endl; 
    }
};


vector<pair<int, double>> convertDendrogram(vector<dendroLine>& dendro, bool convert_weight=false){
    std::size_t n = dendro.size()+1;
    vector<pair<int, double>> parents = vector<pair<int, double>>(2*n-2);

    double min_nonzero_dist = std::numeric_limits<double>::max();

    parlay::parallel_for(0, n-1, [&](std::size_t i){
        int cur_id = n+i;
        int fst = dendro[i].id1;
        int snd = dendro[i].id2;
        double wgh = dendro[i].height;
        parents[fst] = make_pair(cur_id, wgh);
        parents[snd] = make_pair(cur_id, wgh);
        if(wgh != 0){
            min_nonzero_dist = min(min_nonzero_dist, wgh);
        }
    });

    if(convert_weight){
    parlay::parallel_for(0, 2*n-2, [&](std::size_t i){
        double dist = parents[i].second;
        if(dist==0) dist = min_nonzero_dist;
        double sim = 1/(1+dist);
        parents[i].second = sim;
    });
    }

    return parents;
}



std::vector<dendroLine> ReadDendrogram(const char* filename) {
  std::vector<dendroLine> points;
  std::ifstream file{filename};
  if (!file.is_open()) {
    cerr << "Unable to open file." <<endl;
    return std::vector<dendroLine>();
  }
  std::string line;
  std::vector<dendroLine> dendro;
  while (std::getline(file, line)){
    std::istringstream line_stream(line);
    int id1; int id2; double height; 
    line_stream >> id1;
    line_stream >> id2;
    line_stream >> height;
    dendro.push_back(dendroLine(id1, id2, height, 0 /*placeholder*/));
  }
  file.close();
  return dendro;
}


}}

int main(int argc, char **argv)
{
    string filename = argv[1];
    string out_filename = argv[2];
    auto dendro = research_graph::in_memory::ReadDendrogram(filename.c_str());
    vector<pair<int, double>> parent = research_graph::in_memory::convertDendrogram(dendro, true);
    ofstream file_obj;
    file_obj.open(out_filename); 
    int id = 0;
    for(pair<int, double> p : parent){
        file_obj << id << " " << p.first << " " << p.second << endl;
        id++;
    }
    cout << "done" << endl;
  
}