#pragma once


using namespace std;
namespace research_graph {
namespace in_memory {
namespace internal{
namespace utils {

#if defined(MCX16)
//ET should be 128 bits and 128-bit aligned
template <class ET> 
  inline bool CAS128(ET* a, ET b, ET c) {
  return __sync_bool_compare_and_swap_16((__int128*)a,*((__int128*)&b),*((__int128*)&c));
}
#endif

// The conditional should be removed by the compiler
// this should work with pointer types, or pairs of integers
template <class ET>
inline bool CAS(ET *ptr, ET oldv, ET newv) { 
  if (sizeof(ET) == 1) { 
    return __sync_bool_compare_and_swap_1((bool*) ptr, *((bool*) &oldv), *((bool*) &newv));
  } else if (sizeof(ET) == 8) {
    return __sync_bool_compare_and_swap_8((long*) ptr, *((long*) &oldv), *((long*) &newv));
  } else if (sizeof(ET) == 4) {
    return __sync_bool_compare_and_swap_4((int *) ptr, *((int *) &oldv), *((int *) &newv));
  } 
#if defined(MCX16)
  else if (sizeof(ET) == 16) {
    return utils::CAS128(ptr, oldv, newv);
  }
#endif
  else {
    std::cout << "common/utils.h CAS bad length " << sizeof(ET) << std::endl;
    abort();
  }
}

template <class ET>
inline bool CAS_GCC(ET *ptr, ET oldv, ET newv) {
  if (sizeof(ET) == 4) {
    return __sync_bool_compare_and_swap((int*)ptr, *((int*)&oldv), *((int*)&newv));
  } else if (sizeof(ET) == 8) {
    return __sync_bool_compare_and_swap((long*)ptr, *((long*)&oldv), *((long*)&newv));
  } 
#ifdef MCX16
  else if(sizeof(ET) == 16)
    return __sync_bool_compare_and_swap_16((__int128*)ptr,*((__int128*)&oldv),*((__int128*)&newv));
#endif
  else {
    std::cout << "common/utils.h CAS_GCC bad length" << sizeof(ET) << std::endl;
    abort();
  }
}

template <class ET>
inline bool writeMin(ET *a, ET b) {
  ET c; bool r=0;
  do c = *a; 
  while (c > b && !(r=CAS(a,c,b)));
  return r;
}

 template <class ET, class F>
  inline bool writeMin(ET *a, ET b, F f) {
  ET c; bool r=0;
  do c = *a; 
  while (f(b,c) && !(r=CAS_GCC(a,c,b)));
  return r;
}

 template <class ET, class F>
  inline bool writeMax(ET *a, ET b, F f) {
  ET c; bool r=0;
  do c = *a; 
  while ((!f(b,c)) && !(r=CAS_GCC(a,c,b)));
  return r;
}

}
}
}
}