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

#include <iostream>
#include <fstream>
#include <string>

#include "parlay/parallel.h"
#include "parlay/primitives.h"

#include "geometry.h"

namespace research_graph {
namespace in_memory {
namespace IO {
  using namespace std;
  // modified from pbbsbench  https://github.com/cmuparlay/pbbsbench/blob/37df3e14c7f3d738500e06840874a1505944598d/common/geometryIO.h
  parlay::sequence<char> readStringFromFile(char const *fileName) {
    ifstream file (fileName, ios::in | ios::binary | ios::ate);
    if (!file.is_open()) {
      std::cout << "Unable to open file: " << fileName << std::endl;
      abort();
    }
    long end = file.tellg();
    file.seekg (0, ios::beg);
    long n = end - file.tellg();
    parlay::sequence<char> bytes(n, (char) 0);
    file.read (bytes.begin(), n);
    file.close();
    return bytes;
  }
  
  // parallel code for converting a string to word pointers
  // side effects string by setting to null after each word
  template <class Seq>
    parlay::sequence<char*> stringToWords(Seq &Str) {
    size_t n = Str.size();
    
    parlay::parallel_for(0, n, [&] (long i) {
	if (is_space(Str[i])) Str[i] = 0;}); 

    // mark start of words
    auto FL = parlay::tabulate(n, [&] (long i) -> bool {
	return (i==0) ? Str[0] : Str[i] && !Str[i-1];});
    
    // offset for each start of word
    auto Offsets = parlay::pack_index<long>(FL);

    // pointer to each start of word
    auto SA = parlay::tabulate(Offsets.size(), [&] (long j) -> char* {
	return Str.begin() + Offsets[j];});
    
    return SA;
  }

  template <class T>
  pointset<T> parsePoints(Seq W, int d) {
    size_t n = W.size()/d;
    pointset<T> points = pointset<T>(n,d);
    parlay::parallel_for(d * n, [&] (size_t i){
	    points.set(i, (T)atof(W[i]));});
    return points;
  }

  template <class T>
  pointset<T> readPointsFromFile(char const *fname, int d) {
    parlay::sequence<char> S = readStringFromFile(fname);
    parlay::sequence<char*> W = stringToWords(S);
    if (W.size() == 0) {
      cout << "readPointsFromFile empty file" << endl;
      abort();
    }
    if (W.size() % d != 0) {
      cout << "readPointsFromFile wrong file type or wrong dimension" << endl;
      abort();
    }
    return parsePoints<T>(W.cut(1,W.size()));
  }

}  // namespace IO
}  // namespace in_memory
}  // namespace research_graph