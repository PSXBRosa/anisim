#pragma once
#define _USE_MATH_DEFINES
#include <algorithm>
#include <functional>
#include <variant>
#include <cmath>
#include "generic.hpp"
#include "shapes.hpp"

using namespace sim;

namespace PARAMETRIC {
  // CURVES
  /*
    parametric curves are normalized:
      max   is  1
      min   is -1
      start at  0
      end   at  1
  */

  // class definitions
  // -----------------------------------------------------------------------------------
  template<class I, class O>
  class BaseCurve{
    private:
      std::function<O(I)> foo;

      iterable<O> evaluate(const iterable<I>& linspace){
        iterable<O> out;
        std::transform(
          linspace.begin(), linspace.end(),
          std::back_inserter(out), foo
        );
        return out;
      }
    public:
      BaseCurve(){}
      BaseCurve(std::function<O(I)> foo) : foo(foo) {}
      O operator()(I t) const {return foo(t);}
      iterable<O> operator()(const iterable<I>& linspace) const {return evaluate(linspace);}
  };

  template<class I, class O = I> struct Curve : public BaseCurve<I, O> {Curve(std::function<O(I)> foo) : BaseCurve<I, O>(foo) {}};
  // ------------------------------------------------------------------------------------
  // end class definitions

  template<class A, class B, class T>
  auto operator+(const BaseCurve<T, A>& lhs, const BaseCurve<T, B>& rhs){
    return BaseCurve<T, decltype(lhs(0) + rhs(0))>([lhs, rhs](T t) -> decltype(lhs(0) + rhs(0)) {return lhs(t) + rhs(t);});
  }

  template<class A, class B, class T>
  auto operator*(BaseCurve<T, A> lhs, const BaseCurve<T, B>& rhs){
    return BaseCurve<T, decltype(lhs(0) * rhs(0))>([lhs, rhs](T t) -> decltype(lhs(0) * rhs(0)) {return lhs(t) * rhs(t);});
  }

  // create a constant curve
  template<class T, class A>
  inline Curve<T, A> K(const A c){return Curve<T, A>([c](T t) -> A {return c;});}

  // create an identity curve
  template<class T>
  inline Curve<T, T> I(){return Curve<T, T>([](T t) -> T {return t;});}

  // create a circle
  template<class A = Instant>
  inline Curve<A, Point<A>> circle(){
    return Curve<A, Point<A>>( [](A t){return Point<A>(std::cos(t * (2 * M_PI)), std::sin(t * (2 * M_PI)));} );
  }
  //

}

