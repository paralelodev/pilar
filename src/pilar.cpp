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

  return BI->second;
}

static Block *runBlockAndProgress(Program &P, Memory &M, Dictionary &D,
                                  Block &B) {
  for (Instruction *I : B) {
    switch (I->Command) {
    case Commands::GOTO:
      return findBlock(P, I->Operands[0]);
      break;
    case Commands::PUSH:
      M.push(stoi(I->Operands[0]));
      break;
    case Commands::PRINT:
      std::cout << M.top() << '\n';
      M.pop();
      break;
    case Commands::STORE:
      D[I->Operands[0]] = M.top();
      M.pop();
      break;
    case Commands::LOAD: {
      Dictionary::iterator DI = D.find(I->Operands[0]);
      if (DI == D.end()) {
        exitError("value to load does not exist");
      }
      auto [id, value] = *DI;
      M.push(value);
      break;
    }
    case Commands::SUM: {
      if (M.size() < 2) {
        exitError("stack memory does not have enough elements for a sum");
      }

      int sum = 0;
      for (int i = 0; i < 2; i++) {
        sum += M.top();
        M.pop();
      }
      M.push(sum);
      break;
    }
    default:
      exitError("unsupported command");
      break;
    }
  }
  return nullptr;
}

void runProgram(Program &P) {
  Memory M;
  Dictionary D;
  std::string entry(".entry");
  Block *currentBlock = findBlock(P, entry);
  if (!currentBlock) {
    exitError("program does not have an entry point");
  }

  do {
    currentBlock = runBlockAndProgress(P, M, D, *currentBlock);
  } while (currentBlock);
}
} // namespace pilar
