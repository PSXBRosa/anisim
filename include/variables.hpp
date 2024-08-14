#pragma once
#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <iostream>
#include <map>

#include "action.hpp"

template<class C>
using iterable = std::vector<C>;

namespace sim{
  // Variables
  class AbstractVariable {
    protected:
      std::vector<ActionWeakPtr> actions;

    public:
      AbstractVariable(VectAct actions) : actions(actions) {}
      AbstractVariable() = default;
      virtual ~AbstractVariable() = default;

      inline void link(ActionPtr t) {actions.push_back(t);}

      void force_update(){
        for(auto it = actions.begin(); it != actions.end(); /* Nothing */){
          auto action = it->lock();
          if(action){execute(action); it++;}
          else {it = actions.erase(it);}
        }
      }
  };

  template<class T>
  class Variable : public AbstractVariable {
    protected:
      T ival; // internal value

    public:
      Variable(T val, VectAct actions) : ival(val), AbstractVariable(actions) {}

      // Avoid using me, prefer set.
      void static_set(T newval){
        ival = newval;
      }

      void set(T newval) {
        if(ival == newval) return;
        static_set(newval);
        force_update();
      }

      T val() {return ival;}
  };

  // This class template provides access to the methods from C
  // while still being a Variable type
  template<class C>
  class ComplexVariable : public C, public Variable<C>{
    public:
      ComplexVariable() : Variable<C>({}, VectAct()) {C(ival);}
      ComplexVariable(C val) : Variable<C>(val, VectAct()){
        C(ival);
      }
  };

  /*
   * @warning: NOT A VARIABLE
   * @desc: this class provides a way to store variables in a queue
   */
  class VariableQueue {
    protected:
      std::vector<std::weak_ptr<AbstractVariable>> buff;
    public:
      void push(std::weak_ptr<AbstractVariable> varptr) {buff.push_back(varptr);}
      std::weak_ptr<AbstractVariable> pop() {auto out = buff.back(); buff.pop_back(); return out;}
      std::weak_ptr<AbstractVariable> at(size_t pos) {return buff[pos];}
  };

  /*
   * @desc: This class abstracts a group of variables and allows access into their individual actions.
   */
  using VariableGroup = ComplexVariable<VariableQueue>;

  class SmartVariableGroup {
    private:
      VariableGroup constrained;
      VariableGroup free;
    public:
      SmartVariableGroup(VariableGroup c, VariableGroup f) : constrained(c), free(f) {}
  };

  template<class T>
  using VariablePtr = std::shared_ptr<Variable<T>>;

  template<class T>
  inline VariablePtr<T> var(T initialValue) {return std::make_shared<Variable<T>>(initialValue, VectAct());};
  // end Variables

  std::map<std::string, sim::AbstractVariable>(varHandler);
}
