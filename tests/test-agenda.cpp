#include "include/generic.hpp"
#include "include/shapes.hpp"
#include "include/printer.hpp"

int main(){
  auto simulator = sim::agenda();
  auto var       = sim::var(0);
  auto print_var = sim::action([var, simulator](){sim::____display(simulator->now(), "var", var->val());});

  var->link(print_var);

  simulator->notify(var, 1, 2); 
  simulator->notify(var, 2, 4); 
  sim::____();
  simulator->flush();
  sim::____();
  std::cout << std::endl;

  /* Expected output:
  +-----------------+---------+----------------------+
  | var             | 2       | 1
  | var             | 4       | 2
  +-----------------+---------+----------------------+
  */

  simulator->notify(var, 1, 2); 
  simulator->notify(var, 2, 4); 
  sim::____();
  simulator->flush();
  sim::____();
  std::cout << std::endl;

  /* Expected output:
  +-----------------+---------+----------------------+
  | var             | 6       | 1
  | var             | 8       | 2
  +-----------------+---------+----------------------+
  */

  return 0;
}
