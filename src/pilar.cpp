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

static int extractAndPopOne(Memory &M) {
  if (M.size() < 1) {
    exitError("stack memory does not have enough elements for a comparison");
  }

  int a = M.top();
  M.pop();

  return a;
}

static std::pair<int, int> extractAndPopTwo(Memory &M) {
  if (M.size() < 2) {
    exitError("stack memory does not have enough elements for a sum");
  }

  int b = M.top();
  M.pop();
  int a = M.top();
  M.pop();

  return {a, b};
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
      auto [a, b] = extractAndPopTwo(M);
      M.push(a + b);
      break;
    }
    case Commands::EQUAL: {
      auto [a, b] = extractAndPopTwo(M);
      M.push(a == b ? 1 : 0);
      break;
    }
    case Commands::GREATER: {
      auto [a, b] = extractAndPopTwo(M);
      M.push(a > b ? 1 : 0);
      break;
    }
    case Commands::CHOOSE:
      return findBlock(P, I->Operands[extractAndPopOne(M) == 1 ? 0 : 1]);
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
