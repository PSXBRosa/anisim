#pragma once
#include "generic.hpp"
#include "shapes.hpp"
#include "curves.hpp"

namespace sim{

  template<class C>
  auto accelerate(AgendaPtr a, VariablePtr<DynamicPoint<C>> var, C amount, Instant delay = 1){
    auto upd = action([a, var](){var->force_update(); var->setDynamics(var->now(), var->val().});
    auto act = action([n = 0, a, var, amount, delay]() mutable {
      a->notify(var, (n++) * amount, delay);
    });
    var->link(act);
    execute(act);
    return act;
  }

  template<class C>
  auto impulse(AgendaPtr a, VariablePtr<DynamicPoint<C>> var, C amount, Instant delay = 1){
    auto act = action([a, var, amount, delay](){
      a->notify(var, amount, delay);
    });
    var->link(act);
    execute(act);
    return act;
  }

  template<class C, class D>
  auto follow(AgendaPtr a, VariablePtr<C> ref, VariablePtr<D> fol, PARAMETRIC::Curve<C, D> cur, Instant delay = 0){
    auto act = action([a, ref, fol, cur, delay](){a->notify(fol, cur(ref->val()), delay);});
    ref->link(act);
    execute(act);
    return act;
  }

  template<class C>
  auto follow(AgendaPtr a, VariablePtr<C> ref, VariablePtr<C> fol, Instant delay = 0){
    return follow(a, ref, fol, PARAMETRIC::I<C, C>(), delay);
  }

}


