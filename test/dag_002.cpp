#include "dag.h"

using namespace dag;

int main() {
  IntLiteral i0(5);
  Assign a0("a", &i0);

  IntLiteral i1(4);
  Fetch f0("a");
  BinaryOperation b0(Operators::ADD, &i1, &f0);
  Assign a1("b", &b0);

  Fetch f1("a");
  Print p0(&f1);

  Fetch f2("b");
  Print p1(&f2);

  Block DAG({&a0, &a1, &p0, &p1});

  DAG2Pilar(DAG);
}
