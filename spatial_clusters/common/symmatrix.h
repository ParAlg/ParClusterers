// Copyright 2020 The Google Research Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#pragma once

namespace research_graph {
namespace in_memory {


template<class T> 
struct SymMatrix{

    std::size_t n = 0;
    T* distmat = nullptr;
    std::size_t distmat_size = 0;

    SymM(){}
    SymM(std::size_t _n){
        n = _n;
        distmat_size = n*(n-1)/2;
        distmat = (T *)malloc(distmat_size * sizeof(T));
    }

    void init(std::size_t _n){
        n = _n;
        distmat_size = n*(n-1)/2;
        distmat = (T *)malloc(distmat_size * sizeof(T));
    }

    inline unsigned long getInd(std::size_t i, std::size_t j){
        if(i == j) return distmat_size;
        long r_ = static_cast<long>(i);
        long c_ = static_cast<long>(j);
        return (((2*n-r_-3) * r_) >> 1 )+ (c_)-1;
    }
    
    inline T get(std::size_t r_, std::size_t c_){
        if(r_ == c_) return 0;
        if(r_ > c_) swap(r_,c_);
        return( distmat[getInd(r_, c_)] );
    }

    inline void update(std::size_t r_, std::size_t c_, T dist){
        if(r_ == c_) return;
        if(r_ > c_) swap(r_,c_);
        distmat[getInd(r_, c_)] = dist;
    }

    ~SymM(){
        free(distmat);
    }

    void printMatrix(){
        for (std::size_t i=0; i<distmat_size; i++){
            cout << distmat[i] << endl;
        }
        for (std::size_t i=0; i<n; i++){
            for (std::size_t j=i+1; j<n; j++) {
                cout << i << " " << j << " " <<getInd(i,j) << " " << get(i,j) << endl;
            }
        } 
    }
};

}  // namespace in_memory
}  // namespace research_graph