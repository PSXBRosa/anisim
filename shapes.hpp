#pragma once
#include <utility>
#include <iostream>
#include "generic.hpp"

namespace sim {
  /* Point */
  using Point = std::pair<float, float>;
  using PointVarPtr = VariablePtr<Point>;
  /* end Point */

  ActionPtr translate(AgendaPtr a, const PointVarPtr p, const Curve<Point> c, const std::vector<Instant> keyinstants){
    auto act = action([a, p, c, keyinstants](){
      notify(a, p, c(0), 0);
      for(const auto& t : keyinstants) notify(a, p, c(t), t);
    });
    return act;
  }

  struct Shape{
      Point center;
      Shape(Point center): center(center) {};
  };
}
