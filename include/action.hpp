#pragma once
#include <functional>
#include <memory>
#include <vector>

namespace sim{
  using Action = std::function<void()>;
  using ActionPtr = std::shared_ptr<Action>;
  using ActionWeakPtr = std::weak_ptr<Action>;
  using VectAct = std::vector<ActionWeakPtr>;
  inline void execute(ActionPtr t){(*t)();}
  inline ActionPtr action(Action t) {return std::make_shared<Action>(t);}
}
