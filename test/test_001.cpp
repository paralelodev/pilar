#include "pilar.h"

using namespace pilar;

int main() {

  // .entry
  // push 1
  // print
  // goto .end
  //
  // .end
  // push 4
  // print

  Instruction I0 = {Commands::PUSH, {"1"}};
  Instruction I1 = {Commands::PRINT, {}};
  Instruction I2 = {Commands::GOTO, {".end"}};
  Instruction I3 = {Commands::PUSH, {"4"}};
  Instruction I4 = {Commands::PRINT, {}};
  Block entry = {I0, I1, I2};
  Block end = {I3, I4};
  Program P = {{".entry", entry}, {".end", end}};

  runProgram(P);
}
