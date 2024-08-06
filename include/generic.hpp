#pragma once
#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <iostream>
#include <map>

template<class C>
using iterable = std::vector<C>;

namespace sim{
  // Actions
  using Action = std::function<void()>;
  using ActionPtr = std::shared_ptr<Action>;
  using ActionWeakPtr = std::weak_ptr<Action>;
  using VectAct = std::vector<ActionWeakPtr>;
  inline void execute(ActionPtr t){(*t)();}
  inline ActionPtr action(Action t) {return std::make_shared<Action>(t);}
  // end Actions

  // Variables
  template<class T>
  class Variable{
    protected:
      T ival; // internal value
      std::vector<ActionWeakPtr> actions;

    public:
      // Avoid using me, prefer set.
      void static_set(T newval){
        ival = newval;
      }

      // Avoid using me, prefer set.
      void force_update(){
        for(auto it = actions.begin(); it != actions.end(); /* Nothing */){
          auto action = it->lock();
          if(action){execute(action); it++;}
          else {it = actions.erase(it);}
        }
      }

      void set(T newval) {
        if(ival == newval) return;
        static_set(newval);
        force_update();
      };

      inline void link(ActionPtr t) {actions.push_back(t);}
      inline T& val() const {return ival;}
      Variable(T val, VectAct actions) : ival(val), actions(actions) {}
  };

  // This class template provides access to the methods from C
  // while still being a Variable type
  template<class C>
  class ComplexVariable : public C, public Variable<C>{
    public:
      ComplexVariable() : Variable<C>({}, VectAct()) {C(ival);};
      ComplexVariable(C val) : Variable<C>(val, VectAct()){
        C(ival);
      };
  };

  template<class T>
  using VariablePtr = std::shared_ptr<Variable<T>>;

  template<class T>
  inline VariablePtr<T> var(T initialValue) {return std::make_shared<Variable<T>>(initialValue, VectAct());};
  // end Variables

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

  // Extensions
  ActionPtr triggerableAction(std::function<bool()>& condition, ActionPtr act){
    return action([condition, act](){if(condition()) execute(act);});
  }
  // end extensions
}
