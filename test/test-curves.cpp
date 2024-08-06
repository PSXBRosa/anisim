#include <memory>
#include <functional>
#include "include/generic.hpp"
#include "include/shapes.hpp"
#include "include/printer.hpp"
#include "include/curves.hpp"

int main(){
  auto simulator = sim::agenda();
  auto var       = sim::point();
  auto probe     = sim::probe(simulator, "circle var", var);
  
  PARAMETRIC::BaseCurve<Point<Instant>> myCircle;
  {
    using namespace PARAMETRIC;
    auto center  = K<Instant>(sim::Point(4.0, 7.0));
    auto radius  = K<Instant>(3.0);
    myCircle = center + (radius * circle());
  }

  sim::____();
  sim::____display(0, "myCircle(0) = ", myCircle(0));
  sim::____display(0, "myCircle(1) = ", myCircle(1));
  sim::____();
  
  iterable<Instant> timestamps = iterable<sim::Instant>(10);
  std::generate(timestamps.begin(), timestamps.end(), [n = 1, a = 0.1]() mutable { return n++ * a; });

  iterable<Point<Instant>> vals = myCircle.evaluate(timestamps);
  simulator->notify(var, vals, timestamps);

  sim::____();
  simulator->flush();
  sim::____();
  return 0;

  /*
  +---------------------+--------------+---------------------------+
  | [n] myCircle(0) =   | [t] 0.000000 | [v]  (7.000000, 7.000000)
  | [n] myCircle(1) =   | [t] 0.000000 | [v]  (7.000000, 7.000000)
  +---------------------+--------------+---------------------------+
  +---------------------+--------------+---------------------------+
  | [n] circle var      | [t] 0.100000 | [v]  (6.427051, 8.763356)
  | [n] circle var      | [t] 0.200000 | [v]  (4.927051, 9.853170)
  | [n] circle var      | [t] 0.300000 | [v]  (3.072949, 9.853170)
  | [n] circle var      | [t] 0.400000 | [v]  (1.572949, 8.763356)
  | [n] circle var      | [t] 0.500000 | [v]  (1.000000, 7.000000)
  | [n] circle var      | [t] 0.600000 | [v]  (1.572949, 5.236644)
  | [n] circle var      | [t] 0.700000 | [v]  (3.072949, 4.146830)
  | [n] circle var      | [t] 0.800000 | [v]  (4.927051, 4.146830)
  | [n] circle var      | [t] 0.900000 | [v]  (6.427051, 5.236644)
  | [n] circle var      | [t] 1.000000 | [v]  (7.000000, 7.000000)
  +---------------------+--------------+---------------------------+
  */
}
