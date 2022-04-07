
#include <fstream>
#include <iostream>

using namespace std;

// g++ -std=c++17 -mcx16 test16.cpp
template <class ET>
inline bool CAS_GCC(ET *ptr, ET oldv, ET newv) {
  if (sizeof(ET) == 4) {
    return __sync_bool_compare_and_swap((int*)ptr, *((int*)&oldv), *((int*)&newv));
  } else if (sizeof(ET) == 8) {
    return __sync_bool_compare_and_swap((long*)ptr, *((long*)&oldv), *((long*)&newv));
  } 
// #ifdef MCX16
  else if(sizeof(ET) == 16)
    return __sync_bool_compare_and_swap_16((__int128*)ptr,*((__int128*)&oldv),*((__int128*)&newv));
// #endif
  else {
    std::cout << "common/utils.h CAS_GCC bad length" << sizeof(ET) << std::endl;
    abort();
  }
}

 template <class ET, class F>
  inline bool writeMin(ET *a, ET b, F f) {
  ET c; bool r=0;
  do c = *a; 
  while (f(b,c) && !(r=CAS_GCC(a,c,b)));
  return r;
}

struct T {
    int a;
    int b;
    double c;
};

int main(int argc, char **argv)
{
    T e1{1,2,3};
    T e2{4,5,6};

    writeMin(&e2, e1, [&](T& i, T& j){return i.c < j.c;});
    cout << e2.a << endl;

    pair<double, long> a = make_pair(1.9, (long)10);
    pair<double, long> b = make_pair(1.2, (long)2);

    writeMin(&a, b, std::less<pair<double, long>>());
    cout << a.first << endl;

}