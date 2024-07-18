#pragma once
#include <iostream>
#include "generic.hpp"

#define NAME_WIDTH 15
#define TIME_WIDTH  7

namespace sim {
  inline void ____() {
    std::cout << "+-"  << std::string(NAME_WIDTH, '-')
        << "-+-" << std::string(TIME_WIDTH, '-')
        << "-+-----+" << std::endl;
  }

  template<class C>
  inline void ____display(Instant time, const std::string& name, C val) {
    std::cout.precision(2);
    std::cout << "| " << std::left << std::setw(NAME_WIDTH) << name
        << " | " << std::right << std::fixed << std::setw(TIME_WIDTH) << time;
    std::cout << val << std::endl;
  }

  inline ActionPtr probe(AgendaPtr a, const std::string& msg, ValuePtr w) {
    auto act = action([a, msg, w](){____display(a->now, msg, w->first);});
    link(w, act);
    return act;
  }
}
