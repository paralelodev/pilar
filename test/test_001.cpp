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

  Instruction I0 = {.Command = Commands::PUSH, .Constant = 1};
  Instruction I1 = {.Command = Commands::PRINT};
  Instruction I2 = {.Command = Commands::GOTO, .LBranch = ".end"};
  Instruction I3 = {.Command = Commands::PUSH, .Constant = 4};
  Instruction I4 = {.Command = Commands::PRINT};
  Block entry = {I0, I1, I2};
  Block end = {I3, I4};
  Program P = {{".entry", entry}, {".end", end}};

  runProgram(P);
}
