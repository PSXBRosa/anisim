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
  std::ostream& operator<<(std::ostream& out, const Point p){
    return out << " (" << p.first << ", " << p.second << ") ";
  }
  /* end printers */

  inline void ____() {
    std::cout << "+-"  << std::string(NAME_WIDTH, '-')
        << "-+-" << std::string(TIME_WIDTH, '-')
        << "-+-" << std::string(VALUE_WIDTH, '-') << "-+" << std::endl;
  }

  template<class C>
  inline void ____display(Instant time, const std::string& name, C val) {
    std::cout.precision(2);
    std::cout << "| "  << std::left << std::setw(NAME_WIDTH) << name;
    std::cout << " | " << std::left << std::fixed << std::setw(TIME_WIDTH) << time;
    std::cout << " | " << val <<  std::endl;
  }

  template<class C>
  inline ActionPtr probe(AgendaPtr a, const std::string& msg, VariablePtr<C> w) {
    auto act = action([a, msg, w](){____display(a->now, msg, w->first);});
    link(w, act);
    return act;
  }
}
