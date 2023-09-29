#include "pilar.h"

using namespace pilar;

int main() {

  // .entry
  // push 1
  // store r0
  // load r0
  // print
  // goto .end
  //
  // .end
  // push 4
  // print

  Instruction I0 = {Commands::PUSH, {"1"}};
  Instruction I1 = {Commands::STORE, {"r0"}};
  Instruction I2 = {Commands::LOAD, {"r1"}};
  Instruction I3 = {Commands::PRINT, {}};
  Instruction I4 = {Commands::GOTO, {".end"}};
  Instruction I5 = {Commands::PUSH, {"4"}};
  Instruction I6 = {Commands::PRINT, {}};
  Block entry = {I0, I1, I2, I3, I4};
  Block end = {I5, I6};
  Program P = {{".entry", entry}, {".end", end}};

  runProgram(P);
}
