#include "pilar.h"

using namespace pilar;

int main() {

  // a = 1
  // b = 0
  // if b == a {
  //  print 0
  // }
  // if a > b {
  //  print 1
  // }
  // print a + b

  // .entry
  // push 1
  // store r0
  // push 0
  // store r1
  // load r0
  // load r1
  // ==
  // choose .true .continue
  //
  // .true
  // push 0
  // print
  // goto .continue
  //
  // .continue
  // load r0
  // load r1
  // >
  // choose .true1 .continue1
  //
  // .true1
  // push 1
  // print
  // goto .continue1
  //
  // .continue1
  // load r0
  // load r1
  // +
  // print

  Instruction I0 = {Commands::PUSH, {"1"}};
  Instruction I1 = {Commands::STORE, {"r0"}};
  Instruction I2 = {Commands::PUSH, {"0"}};
  Instruction I3 = {Commands::STORE, {"r1"}};
  Instruction I4 = {Commands::LOAD, {"r0"}};
  Instruction I5 = {Commands::LOAD, {"r1"}};
  Instruction I6 = {Commands::EQUAL, {}};
  Instruction I7 = {Commands::CHOOSE, {".true", ".continue"}};

  Instruction J0 = {Commands::PUSH, {"0"}};
  Instruction J1 = {Commands::PRINT, {}};
  Instruction J2 = {Commands::GOTO, {".continue"}};

  Instruction L0 = {Commands::LOAD, {"r0"}};
  Instruction L1 = {Commands::LOAD, {"r1"}};
  Instruction L2 = {Commands::GREATER, {}};
  Instruction L3 = {Commands::CHOOSE, {".true1", ".continue1"}};

  Instruction K0 = {Commands::PUSH, {"1"}};
  Instruction K1 = {Commands::PRINT, {}};
  Instruction K2 = {Commands::GOTO, {".continue1"}};

  Instruction N0 = {Commands::LOAD, {"r0"}};
  Instruction N1 = {Commands::LOAD, {"r1"}};
  Instruction N2 = {Commands::SUM, {}};
  Instruction N3 = {Commands::PRINT, {}};

  Block entry = {&I0, &I1, &I2, &I3, &I4, &I5, &I6, &I7};
  Block tru = {&J0, &J1, &J2};
  Block cont = {&L0, &L1, &L2, &L3};
  Block tru1 = {&K0, &K1, &K2};
  Block cont1 = {&N0, &N1, &N2, &N3};

  Program P;
  P.Blocks[".entry"] = &entry;
  P.Blocks[".true"] = &tru;
  P.Blocks[".true1"] = &tru1;
  P.Blocks[".continue"] = &cont;
  P.Blocks[".continue1"] = &cont1;

  runProgram(P);
}
