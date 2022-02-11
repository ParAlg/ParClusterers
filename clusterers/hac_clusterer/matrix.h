#pragma once


#include "absl/status/statusor.h"
#include "parcluster/api/config.pb.h"
#include "parcluster/api/in-memory-metric-clusterer-base.h"
#include "parcluster/api/status_macros.h"

#include "parlay/primitives.h"
#include "parlay/sequence.h"


namespace research_graph {
namespace in_memory {

namespace internal {

// symmetric matrix, diagonal is all diag_val = 0 by default
template<class T> 
struct SymMatrix{

    std::size_t n = 0;
    T* distmat = nullptr;
    std::size_t distmat_size = 0;
    T diag_val = 0;

    SymMatrix(){}
    SymMatrix(std::size_t _n, T _diag_val = 0){
        n = _n;
        diag_val = _diag_val;
        distmat_size = n*(n-1)/2;
        distmat = (T *)malloc(distmat_size * sizeof(T));
    }

    void init(std::size_t _n){
        n = _n;
        distmat_size = n*(n-1)/2;
        distmat = (T *)malloc(distmat_size * sizeof(T));
    }

    inline long getInd(std::size_t i, std::size_t j){
        if(i == j) return distmat_size;
        long r_ = static_cast<long>(i);
        long c_ = static_cast<long>(j);
        return (((2*n-r_-3) * r_) >> 1 )+ (c_)-1;
    }
    
    inline void setDiag(T v){
        diag_val = v;
    }

    inline T get(std::size_t r_, std::size_t c_){
        if(r_ == c_) return diag_val;
        if(r_ > c_) swap(r_,c_);
        return( distmat[getInd(r_, c_)] );
    }

    inline void update(std::size_t r_, std::size_t c_, T dist){
        if(r_ == c_) return;
        if(r_ > c_) swap(r_,c_);
        distmat[getInd(r_, c_)] = dist;
    }

    ~SymMatrix(){
        free(distmat);
    }
    void printMatrix(){
    // for (std::size_t i=0; i<distmat_size; i++){
    //     cout << distmat[i] << endl;
    // }
    for (std::size_t i=0; i<n; i++){
        for (std::size_t j=0; j<n; j++) {
            std::cout << get(i,j) << " ";
        }
        std::cout << std::endl;
    } 
    cout << "===" << endl;
    for (std::size_t i=0; i<n; i++){
        for (std::size_t j=i+1; j<n; j++) {
            std::cout << i << " " << j << " " <<getInd(i,j) << " " << get(i,j) << std::endl;
        }
    } 
    }
};

// compute the distance between two data points
// assume the two data points are in dense format
// assume the two data points have the same dimension
template <class T>
T distance(const DataPoint a, const DataPoint b) {
    T sum=0;
    for (std::size_t k=0; k<a.coordinates.size(); k++) {
        float tmp=a.coordinates[k]-b.coordinates[k];
        sum+=(tmp*tmp);
    }
    return sum;
}

//return a symmetric matrix representation of the euclidean distance
// between all pairs of datapoints.
// assume all points have the same dimension and are in dense format
template <class T>
SymMatrix<T> getDistanceMatrix(absl::Span<const DataPoint> datapoints) {
    std::size_t n = datapoints.size();
    SymMatrix<T> matrix = SymMatrix<T>(n);
    parlay::parallel_for(0, n, [&](size_t i){
        parlay::parallel_for(i+1, n,[&] (size_t j){
	        matrix.update(i, j, distance<T>(datapoints[i], datapoints[j]));
        });
    });
    matrix.setDiag(0);
    return matrix;
}

}  //namespace internal
}  // namespace in_memory
}  // namespace research_graph