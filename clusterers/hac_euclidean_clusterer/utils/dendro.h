#pragma once

#include <iostream>
#include <fstream>
#include "parlay/parallel.h"
#include "parlay/primitives.h"
#include "parlay/delayed_sequence.h"
#include "node.h"

using namespace std;
namespace research_graph {
namespace in_memory {
namespace internal {
namespace HACTree {

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

template<int dim>
vector<dendroLine> formatDendrogram(parlay::sequence<Node<dim>> &nodes, std::size_t n, double eps){
    auto sorted_nodes = parlay::sort(parlay::make_slice(nodes).cut(n,2*n-1), nodeComparator<dim>(eps));

    auto map = parlay::sequence<std::size_t>(n);
    parlay::parallel_for(0, n-1, [&](std::size_t i){
        map[sorted_nodes[i].getIdx() - n] = i+n;
    });
    vector<dendroLine> dendrogram = vector<dendroLine>(n-1);//(dendroLine *)malloc(sizeof(dendroLine)*(n-1));
    parlay::parallel_for(0, n-1, [&](std::size_t i){
        std::size_t left = sorted_nodes[i].left->getIdx();
        std::size_t right = sorted_nodes[i].right->getIdx();

        left = left < n ? left : map[left-n];
        right = right < n ? right : map[right-n];

        if(left > right) swap(left, right);


        dendrogram[i] = dendroLine(left, right, sorted_nodes[i].getHeight(),sorted_nodes[i].size());
    });
    return dendrogram;
}

double getCheckSum(vector<dendroLine>& dendro){
    return parlay::reduce(parlay::delayed_seq<double>(dendro.size(), [&](size_t i){return dendro[i].height;}));
}

}}}}