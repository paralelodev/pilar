#include "pilar.h"
#include <iostream>

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
  return BI == Blocks.end() ? nullptr : &BI->second;
}

static int extractAndPopOne(Program &P) {
  if (P.Memory.size() < 1) {
    exitError(P, "stack memory does not have enough elements");
  }

  int a = P.Memory.top();
  P.Memory.pop();

  return a;
}

static std::pair<int, int> extractAndPopTwo(Program &P) {
  if (P.Memory.size() < 2) {
    exitError(P, "stack memory does not have enough elements");
  }

  int b = P.Memory.top();
  P.Memory.pop();
  int a = P.Memory.top();
  P.Memory.pop();

  return {a, b};
}

static void eraseScopeSymbols(SymbolMap &Dictionary, int scope) {
  std::vector<std::string> keysToDelete;
  for (auto [key, symbol] : Dictionary) {
    if (symbol.Scope == scope) {
      keysToDelete.push_back(key);
    }
  }

  for (std::string_view key : keysToDelete) {
    Dictionary.erase(key.data());
  }
}

static Block *runBlockAndProgress(Program &P, Block &B) {
  if (!B.empty() && B.back().Command != Commands::GOTO &&
      B.back().Command != Commands::CHOOSE) {
    exitError(P, "final instruction of block is not a branching instruction");
  }

  for (Instruction &I : B) {
    switch (I.Command) {
    case Commands::GOTO: {
      resetMemory(P.Memory);
      std::string_view targetBlock = I.Operands[0];
      if (P.CurrentScope == 0 && targetBlock != ".exit") {
        exitError(P, "blocks do not diverge");
      }
      eraseScopeSymbols(P.Dictionary, P.CurrentScope);
      P.CurrentScope--;
      return findBlock(P.Blocks, targetBlock);
      break;
    }
    case Commands::PUSH:
      P.Memory.push(atoi(I.Operands[0].data()));
      break;
    case Commands::PRINT:
      std::cout << extractAndPopOne(P) << '\n';
      break;
    case Commands::STORE:
      P.Dictionary[I.Operands[0]] = {extractAndPopOne(P), P.CurrentScope};
      break;
    case Commands::LOAD: {
      SymbolMap::iterator DI = P.Dictionary.find(I.Operands[0]);
      if (DI == P.Dictionary.end()) {
        exitError(P, "value to load does not exist in this scope");
      }
      P.Memory.push(DI->second.Value);
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
      int index = 1;
      if (a == 1) {
        P.CurrentScope++;
        index = 0;
      }

      return findBlock(P.Blocks, I.Operands[index]);
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
  Block *currentBlock = nullptr;
  Block *exitBlock = nullptr;

  for (auto &&[key, block] : P.Blocks) {
    if (key == ".entry") {
      currentBlock = &block;
    } else if (key == ".exit") {
      exitBlock = &block;
    } else if (block.empty()) {
      exitError(P, "block must have instructions");
    }
  }

  if (!currentBlock) {
    exitError(P, "program does not have an entry block");
  }

  if (currentBlock->empty()) {
    exitError(P, "entry block must have instructions");
  }

  if (!exitBlock) {
    exitError(P, "program does not have an exit block");
  }

  if (!exitBlock->empty()) {
    exitError(P, "exit block cannot have instructions");
  }

  do {
    currentBlock = runBlockAndProgress(P, *currentBlock);
  } while (currentBlock);
}
} // namespace pilar
