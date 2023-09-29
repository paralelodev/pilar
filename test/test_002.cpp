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
  Instruction I3 = {Commands::PRINT, {}};
  Instruction I4 = {Commands::PUSH, {"4"}};
  Instruction I5 = {Commands::SUM, {}};
  Instruction I6 = {Commands::STORE, {"r1"}};
  Instruction I7 = {Commands::LOAD, {"r1"}};
  Instruction I8 = {Commands::PRINT, {}};

  Block entry = {&I0, &I1, &I2, &I3, &I4, &I5, &I6, &I7, &I8};

  Program P;
  P[".entry"] = &entry;

  runProgram(P);
}
