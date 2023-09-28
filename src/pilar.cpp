#include "pilar.h"
#include <iostream>
#include <string_view>

namespace pilar {
static void exitError(std::string_view error) {
  std::cout << "Error: " << error << '\n';
  exit(0);
}

static Block *findBlock(Program &P, std::string_view Label) {
  Program::iterator BI = P.find(Label.data());
  std::string error("program has no block with label ");
  error.append(Label);
  if (BI == P.end()) {
    exitError(error);
  }

  return &(BI->second);
}

static Block *runBlockAndProgress(Program &P, Memory &M, Block &B) {
  for (Instruction &I : B) {
    switch (I.Command) {
    case Commands::GOTO:
      return findBlock(P, I.LBranch);
      break;
    case Commands::PUSH:
      M.push(I.Constant);
      break;
    case Commands::PRINT:
      std::cout << M.top() << '\n';
      M.pop();
      break;
    default:
      exitError("unsupported command");
      break;
    }
  }
  return nullptr;
}

void runProgram(Program &P) {
  Memory M;
  std::string entry(".entry");
  Block *currentBlock = findBlock(P, entry);
  if (!currentBlock) {
    exitError("program does not have an entry point");
  }

  do {
    currentBlock = runBlockAndProgress(P, M, *currentBlock);
  } while (currentBlock);
}
} // namespace pilar
