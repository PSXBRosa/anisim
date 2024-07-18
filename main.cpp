#include "generic.hpp"
#include "shapes.hpp"
#include "printer.hpp"

int main(){
  auto s1 = sim::Shape({1, 0});
  auto s2 = sim::Shape({0, 1});
  auto a  = sim::agenda();

  sim::____();
  sim::____display(1, "test", s1.center);
  sim::____();
  return 0;
}
