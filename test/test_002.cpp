#include "pilar.h"

using namespace pilar;

int main() {

  // a = 5
  // b = 4 + a
  // print a
  // print b

  // .entry
  // push 5
  // store r0
  // load r0
  // print
  // push 4
  // +
  // store r1
  // load r1
  // print

  Instruction I0 = {Commands::PUSH, {"5"}};
  Instruction I1 = {Commands::STORE, {"r0"}};
  Instruction I2 = {Commands::LOAD, {"r0"}};
  Instruction I3 = {Commands::PUSH, {"4"}};
  Instruction I4 = {Commands::SUM, {}};
  Instruction I5 = {Commands::STORE, {"r1"}};
  Instruction I6 = {Commands::LOAD, {"r0"}};
  Instruction I7 = {Commands::PRINT, {}};
  Instruction I8 = {Commands::LOAD, {"r1"}};
  Instruction I9 = {Commands::PRINT, {}};

  Block entry = {&I0, &I1, &I2, &I3, &I4, &I5, &I6, &I7, &I8, &I9};

  Program P;
  P.Blocks[".entry"] = &entry;

  runProgram(P);
}
