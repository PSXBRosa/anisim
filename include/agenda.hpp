#pragma once
#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <iostream>
#include <map>

#include "variables.hpp"
#include "action.hpp"

namespace sim {
  // Agenda
  using Instant = double;
  class Agenda{
    private:
      Instant time = 0;
      std::multimap<Instant, ActionPtr> events;

    public:
      inline Instant now() const {return time;}

      inline void push(ActionPtr a, Instant delay){
        events.insert({time + delay, a});
      }

      template<class T>
      inline void notify(VariablePtr<T> w, T val, Instant delay) {
        push(action([w, val](){w->set(val);}), delay);
      }

      template<class T>
      inline void notify(VariablePtr<T> w, iterable<T> vals, iterable<Instant> delays){
        size_t len = vals.size();
        for(size_t i = 0; i < len; i++){notify(w, vals[i], delays[i]);}
      }

      inline bool next(){
        if(events.size() == 0) return false;

        auto next_it = events.begin();
        auto ev_time = next_it->first;
        auto event   = next_it->second;

        events.erase(next_it);

        time = ev_time;
        execute(event);

        return true;
      }

      inline void flush() {while(next());}
      inline void flush(unsigned int nb_steps) {unsigned int n = 0; while((n++ < nb_steps) && next());}
  };

  using AgendaPtr = std::shared_ptr<Agenda>;
  inline AgendaPtr agenda(){return std::make_shared<Agenda>(Agenda());}
  // end Agenda
}
