#pragma once
#include <iomanip>
#include <iostream>
#include "generic.hpp"
#include "shapes.hpp"

#define NAME_WIDTH  15
#define TIME_WIDTH   7
#define VALUE_WIDTH 20

namespace sim {
  /* begin printers */
  template<class C>
  std::ostream& operator<<(std::ostream& out, const Point<C> p){
    return out << " (" << p.x << ", " << p.y << ") ";
  }
  /* end printers */

  inline void ____() {
    std::cout << "+-"  << std::string(NAME_WIDTH + 4, '-')
        << "-+-" << std::string(TIME_WIDTH + 5, '-')
        << "-+-" << std::string(VALUE_WIDTH + 5, '-') << "-+" << std::endl;
  }

  template<class C>
  inline void ____display(Instant time, const std::string& name, C val) {
    std::cout << "| [n] "  << std::left << std::setw(NAME_WIDTH) << name;
    std::cout << " | [t] " << std::left << std::fixed << std::setw(TIME_WIDTH) << time;
    std::cout << " | [v] " << val <<  std::endl;
  }

  template<class C>
  inline ActionPtr probe(AgendaPtr a, const std::string& msg, VariablePtr<C> w) {
    auto act = action([a, msg, w](){____display(a->now(), msg, w->val());});
    w->link(act);
    return act;
  }
}
