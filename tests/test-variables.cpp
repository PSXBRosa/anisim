#include "generic.hpp"
#include "shapes.hpp"
#include "printer.hpp"

int main(){
  auto v = sim::var(10);
  auto a = sim::agenda();

  auto act_1 = sim::action([](){sim::____display(1, "action 1", true);});
  auto act_2 = sim::action([](){sim::____display(1, "action 2", true);});
  auto act_3 = sim::action([](){sim::____display(1, "action 3", true);});

  {
    auto act_4 = sim::action([](){sim::____display(1, "action 4", true);});
    auto act_5 = sim::action([](){sim::____display(1, "action 5", true);});

    v->link(act_1);
    v->link(act_2);
    v->link(act_3);
    v->link(act_4);
    v->link(act_5);

    sim::____();
    v->set(11);
    sim::____();
    std::cout << std::endl;

    sim::____();
    v->set(11);
    sim::____();
    std::cout << std::endl;
  }

  sim::____();
  v->set(12);
  sim::____();
  std::cout << std::endl;

  return 0;
}
