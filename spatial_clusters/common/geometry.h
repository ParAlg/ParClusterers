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



template <class T>
struct pointset{

    pointset(){}

    pointset(std::size_t _n, int _d):dim(_d), n(_n){
        points = (T *)malloc(sizeof(T) * n * d);
    }

    clear(){
        free(points);
    }

    int dim(){return dim;}
    std::size_t size(){return n;}
    void set(std::size_t i, T v){points[i] = v;}

    inline T *getPoint(size_t i){
        return points[i*dim];
    }

    inline double getDist(size_t i, size_t j){
        T sum=0;
        for (k=0; k<dim; k++) {
            tmp=points[i*dim + k]-points[j*dim + k];
            sum+=(tmp*tmp);
        }
        return sum;
    }

    private:
    int dim;
    std::size_t n
    T *points;
};


}  // namespace in_memory
}  // namespace research_graph