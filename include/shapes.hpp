#pragma once
#include <utility>
#include <iostream>
#include "generic.hpp"

namespace sim {
  // For implementing basic operations between classes
  // when more than one of this class templated in inherited
  // the increase_by/multiply_by must be function templates
  template<class I, class O>
  struct BaseOperators{
    O& operator+=(const I& rhs){
      O& reference = static_cast<O&>(*this);
      reference.increase_by(rhs);
      return reference;
    }

    O& operator *=(const I& rhs){
      O& reference = static_cast<O&>(*this);
      reference.multyply_by(rhs);
      return reference;
    }
  };

  template<class I, class O> class Operators           : public BaseOperators<I, O> {};
  template<class O>          class Operators<int,   O> : public BaseOperators<int, O>, BaseOperators<float, O> {};
  template<class O>          class Operators<float, O> : public BaseOperators<int, O>, BaseOperators<float, O> {};

  /* Point */
  template<class C>
  class Point : BaseOperators<Point<C>, Point<C>>, BaseOperators<int, Point<C>>{
    protected:
      C x_, y_;
    public:
      Point(): x_(), y_() {}
      Point(const Point<C>& p): x_(p.x_), y_(p.y_) {}
      Point(C x, C y) : x_(x), y_(y) {}

      const C& x() const {return x_;}
      const C& y() const {return y_;}

      bool operator==(const Point<C>& other){return x_ == other.x() && y_ == other.y();}

      void increase_by(const Point<C>& rPoint){x_ += rPoint.x(); y_ += rPoint.y();}
      void increase_by(const float rhs){x_ += rhs; y_ += rhs;}
      void increase_by(const int rhs){x_ += rhs; y_ += rhs;}

      void multiply_by(const Point<C> rPoint){x_ *= rPoint.x(); y_ *= rPoint.y();}
      void multiply_by(const float rhs){x_ *= rhs; y_ *= rhs;}
      void multiply_by(const int rhs){x_ *= rhs; y_ *= rhs;}
  };

  template<class C>
  class DynamicPoint
  : public Point<C>,
    BaseOperators<Point<C>,        DynamicPoint<C>>,
    BaseOperators<DynamicPoint<C>, DynamicPoint<C>>,
    BaseOperators<int, DynamicPoint<C>>
  {
    protected:
      Point<C> v_, a_;
    public:
      DynamicPoint(): v_(), a_() {}
      DynamicPoint(Point<C>& p): Point<C>(p), v_(), a_() {}
      DynamicPoint(Point<C>& p, Point<C>& v, Point<C>& a): Point<C>(p), v_(v), a_(a) {}
      DynamicPoint(DynamicPoint<C>& p): Point<C>(p.x(), p.y()), v_(p.v()), a_(p.a()) {}
      DynamicPoint(C x, C y, Point<C> v, Point<C> a) : Point<C>(x, y), v_(v), a_(a) {}
      Point<C> v(){return v_;}
      Point<C> a(){return a_;}
  };

  // Operators
  template<class C>
  Point<C> operator+(Point<C> lPoint, const Point<C>& rPoint){
    lPoint += rPoint;
    return lPoint;
  }

  template<class C>
  Point<C> operator*(Point<C> point, const C& val){
    point *= val;
    return point;
  }

  template<class C>
  Point<C> operator*(const C& val, Point<C> point){
    point *= val;
    return point;
  }
  /* end Point */

  // Shorthands
  template<class C>
  using PointVar = ComplexVariable<Point<C>>;

  template<class C>
  using DynamicPointVar = ComplexVariable<DynamicPoint<C>>;

  template<class T = Instant>
  std::shared_ptr<PointVar<T>> point() {return std::make_shared<PointVar<T>>();};

  template<class T = Instant>
  std::shared_ptr<DynamicPointVar<T>> dynamicpoint() {return std::make_shared<DynamicPointVar<T>>();};
}
