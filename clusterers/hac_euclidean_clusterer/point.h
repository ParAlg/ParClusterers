#pragma once

#include <iostream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <limits>

using namespace std;


namespace research_graph {
namespace in_memory {

// from pbbs
template <int _dim> class point;

template <int _dim> class vect {
public:
  typedef double floatT;
  typedef vect vectT;
  typedef point<_dim> pointT;
  floatT x[_dim];
  static const int dim = _dim;
  vect() { for (int i=0; i<_dim; ++i) x[i] = 0; }
  vect(pointT p) { for (int i=0; i<_dim; ++i) x[i] = p.x[i]; }
  vect(floatT* v) { for (int i=0; i<_dim; ++i) x[i] = v[i]; }
  void print() {
    cout << std::setprecision(2);
    cout << ":(";
    for (int i=0; i<_dim-1; ++i) cout << x[i] << ",";
    cout << x[_dim-1] << "):";
  }
  vectT operator+(vectT op2) {
    floatT xx[_dim];
    for (int i=0; i<_dim; ++i) xx[i]=x[i] + op2.x[i];
    return vectT(xx);
  }
  vectT operator-(vectT op2) {
    floatT xx[_dim];
    for (int i=0; i<_dim; ++i) xx[i]=x[i] - op2.x[i];
    return vectT(xx);
  }
  pointT operator+(pointT op2);
  vectT operator*(floatT s) {
    floatT xx[_dim];
    for (int i=0; i<_dim; ++i) xx[i] = x[i]*s;
    return vectT(xx);
  }
  vectT operator/(floatT s) {
    floatT xx[_dim];
    for (int i=0; i<_dim; ++i) xx[i] = x[i]/s;
    return vectT(xx);
  }
  floatT& operator[] (int i) {return x[i];}
  float dot(vectT v) {
    floatT xx=0;
    for (int i=0; i<_dim; ++i) xx += x[i]*v[i];
    return xx;
  }
  vectT cross(vectT v) {
    cout << "vect cross not implemented, abort" << endl; abort();
  }
  floatT maxDim() {
    floatT xx = x[0];
    for (int i=1; i<_dim; ++i) xx = max(xx,x[i]);
    return xx;
  }
  floatT length() {
    floatT xx=0;
    for (int i=0; i<_dim; ++i) xx += x[i]*x[i];
    return sqrt(xx);
  }
};

template <int _dim> class point {
public:
  typedef double floatT;
  typedef vect<_dim> vectT;
  typedef point pointT;
  floatT x[_dim];
  static const int dim = _dim;
  static constexpr double empty = numeric_limits<double>::max();
  int dimension() {return _dim;}
  bool isEmpty() {return x[0]==empty;}
  point() { for (int i=0; i<_dim; ++i) x[i]=empty; }
  point(vectT v) { for (int i=0; i<_dim; ++i) x[i]=v[i]; }
  point(floatT* p) { for (int i=0; i<_dim; ++i) x[i]=p[i]; }
  point(pointT* p) { for (int i=0; i<_dim; ++i) x[i]=p->x[i]; }
//   void print() {
//     cout << std::setprecision(2);
//     cout << ":(";
//     for (int i=0; i<_dim-1; ++i) cout << x[i] << ",";
//     cout << x[_dim-1] << "):";
//   }
  pointT operator+(vectT op2) {
    floatT xx[_dim];
    for (int i=0; i<_dim; ++i) xx[i] = x[i]+op2.x[i];
    return pointT(xx);
  }
  vectT operator-(pointT op2) {
    floatT xx[_dim];
    for (int i=0; i<_dim; ++i) xx[i] = x[i]-op2.x[i];
    return pointT(xx);
  }
  floatT& operator[](int i) {return x[i];}
  void updateX(int i, floatT val) {x[i]=val;}
  floatT at(int i) {return x[i];}
  void minCoords(pointT b) {
    for (int i=0; i<_dim; ++i) x[i] = min(x[i], b.x[i]);}
  void minCoords(floatT* b) {
    for (int i=0; i<_dim; ++i) x[i] = min(x[i], b[i]);}
  void maxCoords(pointT b) {
    for (int i=0; i<_dim; ++i) x[i] = max(x[i], b.x[i]);}
  void maxCoords(floatT* b) {
    for (int i=0; i<_dim; ++i) x[i] = max(x[i], b[i]);}

   pointT mult(floatT c) {
      pointT r;
      for(int i=0; i<dim; ++i) r[i] = x[i]*c;
      return r;}

   pointT operator*(floatT c) {
      pointT r;
      for(int i=0; i<dim; ++i) r[i] = x[i]*c;
      return r;}
//   intT quadrant(pointT center) {
//     intT index = 0;
//     intT offset = 1;
//     for (int i=0; i<_dim; ++i) {
//       if (x[i] > center.x[i]) index += offset;
//       offset *= 2;
//     }
//     return index;
//   }
  floatT* coords() {return x;}  
  bool outOfBox(pointT center, floatT hsize) {
    for (int i=0; i<_dim; ++i) {
      if (x[i]-hsize > center.x[i] || x[i]+hsize < center.x[i])
        return true;
    }
    return false;
  }
  inline floatT pointDist(pointT p) {
    floatT xx=0;
    for (int i=0; i<_dim; ++i) xx += (x[i]-p.x[i])*(x[i]-p.x[i]);
    return sqrt(xx);
  }

  floatT dist(pointT p) {
    return pointDist(p);
  }
  
  floatT pointDistSq(point p) {
    floatT xx=0;
    for (int i=0; i<_dim; ++i) xx += (x[i]-p.x[i])*(x[i]-p.x[i]);
    return xx;
  }
};

template<int dim>
struct iPoint {
  typedef double floatT;
  typedef point<dim> pointT;
  int i;
  pointT p;
  iPoint(pointT pp, int ii): p(pp), i(ii) {}
  iPoint(pointT pp): p(pp), i(-1) {}
  iPoint(): i(-1) {}
  bool isEmpty() {return i<0;}
  floatT operator[](int i) {return p[i];}
  floatT pointDist(iPoint q) {return p.pointDist(q.p);}
  floatT pointDist(iPoint *q) {return p.pointDist(q->p);}
  floatT dist(iPoint q) {return p.pointDist(q.p);}
  floatT pointDist(pointT q) {return p.pointDist(q);}
  floatT dist(pointT q) {return p.pointDist(q);}
  floatT pointDistSq(iPoint q) {return p.pointDistSq(q.p);}
  floatT pointDistSq(iPoint *q) {return p.pointDistSq(q->p);}
  floatT pointDistSq(pointT q) {return p.pointDistSq(q);}
  int idx() {return i;}
  void idx(int ii) {i=ii;}
  pointT pt() {return p;}
  floatT* coordinate() {return p.x;}
  floatT coordinate(int i) {return p.x[i];}
  floatT* coords() {return p.x;}
  floatT coords(int i) {return p.x[i];}
  floatT x(int i) {return p.x[i];}
  floatT at(int i) {return p.x[i];}
  void x(int i, floatT val) {p.x[i]=val;}
  int getDim(){return dim;}
  void minCoords(iPoint q){p.minCoords(q.p);}
  void maxCoords(iPoint q){p.maxCoords(q.p);}
  void print(){p.print();}
};

template<int dim>
static std::ostream& operator<<(std::ostream& os, const point<dim> v) {
  for (int i=0; i<v.dim; ++i)
    os << std::setprecision(2) << v.x[i] << ", ";
  return os;
}

template<int dim>
static std::ostream& operator<<(std::ostream& os, const iPoint<dim> v) {
  for (int i=0; i<v.dim; ++i)
    os << std::setprecision(2) << v.x[i] << ", ";
  return os;
}


}
}