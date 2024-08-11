#pragma once
#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <iostream>
#include <map>

namespace sim{
  // Actions
  using Action = std::function<void()>;
  using ActionPtr = std::shared_ptr<Action>;
  using ActionWeakPtr = std::weak_ptr<Action>;
  inline void execute(ActionPtr t){(*t)();}
  inline ActionPtr action(Action t) {return std::make_shared<Action>(t);}
  // end Actions

  // Variables
  template<class T>
  using Variable = std::pair<T, std::vector<ActionWeakPtr>>; // TODO see if list isn't better

  template<class T>
  using VariablePtr = std::shared_ptr<Variable<T>>;

  template<class T>
  inline VariablePtr<T> newvar(T initialValue) {return std::make_shared<Variable<T>>(initialValue, std::vector<ActionWeakPtr>());};

  template<class T>
  inline void link(VariablePtr<T> v, ActionPtr t) {(v->second).push_back(t);}

  template<class T>
  void set(VariablePtr<T> var, T val) {
    if(var->first == val) return;
    var->first = val;
    auto& [ogVal, actions] = *var;
    for(auto it = actions.begin(); it != actions.end(); /* Nothing */){
      if(auto action = it->lock(); action){execute(action); it++;}
      else {it = actions.erase(it);}
    }
  }
  // end Variables

  // Agenda
  using Instant = int;
  struct Agenda{
    Instant now = 0;
    std::multimap<Instant, ActionPtr> events;
  };
  using AgendaPtr = std::shared_ptr<Agenda>;
  inline AgendaPtr agenda(){return std::make_shared<Agenda>(Agenda());}

  template<class T>
  inline void notify(AgendaPtr a, VariablePtr<T> w, T val, Instant delay) {
    (a->events).insert(
      {a->now + delay, action([w, val](){set(w, val);})}
    );
  }

  inline bool next(AgendaPtr a) {
    auto& events = a->events;
    if(events.size() == 0) return false;

    auto next_it = events.begin();
    auto [time, event] = *next_it;
    events.erase(next_it);
    a->now = time;

    execute(event);
    return true;
  }

  inline void flush(AgendaPtr a) {while(next(a));}
  inline void flush(AgendaPtr a, unsigned int nb_steps) {unsigned int n = 0; while((n++ < nb_steps) && next(a));}
  // end Agenda

  // Extensions
  template<class C>
  using Curve = std::function<C(Instant)>;

  ActionPtr triggerableAction(std::function<bool()>& condition, ActionPtr act){
    return action([condition, act](){if(condition()) execute(act);});
  }
  // end extensions
}
