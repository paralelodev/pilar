#include "pilar.h"
#include <iostream>
#include <string_view>

namespace pilar {
static void resetMemory(IntStack M) {
  while (!M.empty()) {
    M.pop();
  }
}

static void exitError(Program &P, std::string_view error) {
  std::cout << "Error: " << error << '\n';
  resetMemory(P.Memory);
  P.Dictionary.clear();
  exit(0);
}

static Block *findBlock(BlockMap &Blocks, std::string_view Label) {
  BlockMap::iterator BI = Blocks.find(Label.data());
  return BI == Blocks.end() ? nullptr : BI->second;
}

static int extractAndPopOne(Program &P) {
  if (P.Memory.size() < 1) {
    exitError(P, "stack memory does not have enough elements for a comparison");
  }

  int a = P.Memory.top();
  P.Memory.pop();

  return a;
}

static std::pair<int, int> extractAndPopTwo(Program &P) {
  if (P.Memory.size() < 2) {
    exitError(P, "stack memory does not have enough elements for a sum");
  }

  int b = P.Memory.top();
  P.Memory.pop();
  int a = P.Memory.top();
  P.Memory.pop();

  return {a, b};
}

static Block *runBlockAndProgress(Program &P, Block &B) {
  if (!B.empty() && B.back()->Command != Commands::GOTO &&
      B.back()->Command != Commands::CHOOSE) {
    exitError(P, "final instruction of block is not a branching instruction");
  }

  for (Instruction *I : B) {
    switch (I->Command) {
    case Commands::GOTO:
      resetMemory(P.Memory);
      return findBlock(P.Blocks, I->Operands[0]);
      break;
    case Commands::PUSH:
      P.Memory.push(stoi(I->Operands[0]));
      break;
    case Commands::PRINT:
      std::cout << P.Memory.top() << '\n';
      P.Memory.pop();
      break;
    case Commands::STORE:
      P.Dictionary[I->Operands[0]] = P.Memory.top();
      P.Memory.pop();
      break;
    case Commands::LOAD: {
      IntMap::iterator DI = P.Dictionary.find(I->Operands[0]);
      if (DI == P.Dictionary.end()) {
        exitError(P, "value to load does not exist");
      }
      auto [id, value] = *DI;
      P.Memory.push(value);
      break;
    }
    case Commands::SUM: {
      auto [a, b] = extractAndPopTwo(P);
      P.Memory.push(a + b);
      break;
    }
    case Commands::EQUAL: {
      auto [a, b] = extractAndPopTwo(P);
      P.Memory.push(a == b ? 1 : 0);
      break;
    }
    case Commands::GREATER: {
      auto [a, b] = extractAndPopTwo(P);
      P.Memory.push(a > b ? 1 : 0);
      break;
    }
    case Commands::CHOOSE: {
      int a = extractAndPopOne(P);
      resetMemory(P.Memory);
      return findBlock(P.Blocks, I->Operands[a == 1 ? 0 : 1]);
      break;
    }
    default:
      exitError(P, "unsupported command");
      break;
    }
  }
  return nullptr;
}

void runProgram(Program &P) {
  Block *currentBlock = findBlock(P.Blocks, ".entry");
  if (!currentBlock) {
    exitError(P, "program does not have an entry block");
  }

  if (!findBlock(P.Blocks, ".exit")) {
    exitError(P, "program does not have an exit block");
  }

  do {
    currentBlock = runBlockAndProgress(P, *currentBlock);
  } while (currentBlock);
}
} // namespace pilar
