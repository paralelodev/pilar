#include "dag.h"

using namespace dag;

int main() {
  IntLiteral i0(1);
  Assign a0("a", &i0);

  IntLiteral i1(0);
  Assign a1("b", &i1);

  Fetch f0("a");
  Fetch f1("b");
  BinaryOperation b0(Operators::EQUAL, &f0, &f1);
  IntLiteral i2(0);
  Print p0(&i2);
  Block bl0({});
  If if0(&b0, &p0, &bl0);

  Fetch f2("a");
  Fetch f3("b");
  BinaryOperation b1(Operators::GREATER, &f2, &f3);
  IntLiteral i3(1);
  Print p1(&i3);
  Block bl1({});
  If if1(&b1, &p1, &bl1);

  Fetch f4("a");
  Fetch f5("b");
  BinaryOperation b2(Operators::ADD, &f4, &f5);
  Print p2(&b2);

  Block DAG({&a0, &a1, &if0, &if1, &p2});

  DAG2Pilar(DAG);
}
