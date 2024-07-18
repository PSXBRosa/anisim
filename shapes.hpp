#pragma once
#include <utility>
#include "generic.hpp"

namespace sim {
  using Point = std::pair<float, float>
  using PointVarPtr = VariablePtr<Point>;

  ActionPtr translate(AgendaPtr a, const PointVarPtr p, const Curve<Point> c, const std::vector<Instant> keyinstants){
    notify(a, p, c(0), 0);
    for(const auto& t : keyinstants) notify(a, p, c(t), t);  
  }

  struct Shape{
      Point center;
      Shape(Point center): center(center) {};
  };
}
