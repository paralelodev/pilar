#include "dag.h"

using namespace dag;

int main() {
  Assign I0("a", IntLiteral(1));
  Assign I1("b", IntLiteral(0));
  If I2(BinaryOperation(Operators::EQUAL, Fetch("a"), Fetch("b")),
        Print(IntLiteral(0)), Block({}));
  If I3(BinaryOperation(Operators::GREATER, Fetch("a"), Fetch("b")),
        Print(IntLiteral(1)), Block({}));
  Print I4(BinaryOperation(Operators::ADD, Fetch("a"), Fetch("b")));

  Block DAG({&I0, &I1, &I2, &I3, &I4});

  DAG2Pilar(DAG);
}
