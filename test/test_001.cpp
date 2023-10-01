#include "pilar.h"

using namespace pilar;

int main() {

  // .entry
  // push 1
  // store r0
  // load r0
  // print
  // push 4
  // print
  // goto .exit
  //
  // .exit

  Instruction I0 = {Commands::PUSH, {"1"}};
  Instruction I1 = {Commands::STORE, {"r0"}};
  Instruction I2 = {Commands::LOAD, {"r0"}};
  Instruction I3 = {Commands::PRINT, {}};
  Instruction I4 = {Commands::PUSH, {"4"}};
  Instruction I5 = {Commands::PRINT, {}};
  Instruction I6 = {Commands::GOTO, {".exit"}};
  Block entry = {I0, I1, I2, I3, I4, I5, I6};
  Block exit = {};

  Program P;
  P.Blocks[".entry"] = entry;
  P.Blocks[".exit"] = exit;

  runProgram(P);
}
