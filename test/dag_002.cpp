#include "dag.h"

using namespace dag;

int main() {
  Assign I0("a", IntLiteral(5));
  Assign I1("b", BinaryOperation(Operators::ADD, IntLiteral(4), Fetch("a")));
  Print I2(Fetch("a"));
  Print I3(Fetch("b"));

  Block DAG({&I0, &I1, &I2, &I3});

  DAG2Pilar(DAG);
}
