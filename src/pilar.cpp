/** pilar.cpp                            **/
/** Implementation for the Pilar library **/

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

// This is the core function where instructions are interpreted.
// It executes one by one the instructions of a block until a branching
// instruction is found.
// It returns a pointer to the next block in the CFG, for further processing.
static Block *runBlockAndProgress(Program &P, Block &B) {
  // Reject on final instruction not being a branching one
  if (!B.empty() && B.back().Command != Commands::GOTO &&
      B.back().Command != Commands::CHOOSE) {
    exitError(P, "final instruction of block is not a branching instruction");
  }

  // Interpret each instruction of the block
  for (Instruction &I : B) {
    switch (I.Command) {
    case Commands::GOTO: { // inconditional branching
      resetMemory(P.Memory);

      // Reject on inconditional branching at scope 0
      std::string_view targetBlock = I.Operands[0];
      if (P.CurrentScope == 0 && targetBlock != ".exit") {
        exitError(P, "blocks do not diverge");
      }

      // Before moving to a previous scope, the symbols of the current scope
      // must be first erased from the symbol table
      eraseScopeSymbols(P.Dictionary, P.CurrentScope);
      P.CurrentScope--;

      // Transfer control to the target block
      return findBlock(P.Blocks, targetBlock);
      break;
    }
    case Commands::CHOOSE: { // conditional branching
      // choose the operand index according to the value of the top of the stack
      int a = extractAndPopOne(P);
      resetMemory(P.Memory);
      int index = 1; // defaults to the .else index
      if (a == 1) {
        P.CurrentScope++;
        index = 0; // go to the .then index
      }

      // Transfer control to the selected block
      return findBlock(P.Blocks, I.Operands[index]);
      break;
    }
    case Commands::PUSH: // pushes a constant value to the top of the stack
      P.Memory.push(atoi(I.Operands[0].data()));
      break;
    case Commands::PRINT: // prints the top of the stack and pops it
      std::cout << extractAndPopOne(P) << '\n';
      break;
    case Commands::STORE: // stores the top of the stack into the dictionary and
                          // pops it
      P.Dictionary[I.Operands[0]] = {extractAndPopOne(P), P.CurrentScope};
      break;
    case Commands::LOAD: { // retrieves a value from the dictionary and pushes
                           // it into the top of the stack
      SymbolMap::iterator DI = P.Dictionary.find(I.Operands[0]);

      // Reject on inexistent symbol
      if (DI == P.Dictionary.end()) {
        exitError(P, "value to load does not exist in this scope");
      }

      P.Memory.push(DI->second.Value);
      break;
    }
    case Commands::SUM: { // binary operators take two stack elements, operate
                          // on them, pop them and push the result on the top
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
    default:
      exitError(P, "unsupported command");
      break;
    }
  }

  // No instructions left to process
  return nullptr;
}

void runProgram(Program &P) {
  Block *currentBlock = nullptr;
  Block *exitBlock = nullptr;

  // Validate the CFG

  // Check for .entry, .exit and empty blocks
  for (auto &&[key, block] : P.Blocks) {
    if (key == ".entry") {
      currentBlock = &block;
    } else if (key == ".exit") {
      exitBlock = &block;
    } else if (block.empty()) { // Reject on other empty blocks
      exitError(P, "block must have instructions");
    }
  }

  // Reject on absence of an .entry block
  if (!currentBlock) {
    exitError(P, "program does not have an entry block");
  }

  // Reject on an empty .entry block
  if (currentBlock->empty()) {
    exitError(P, "entry block must have instructions");
  }

  // Reject on absence of an .exit block
  if (!exitBlock) {
    exitError(P, "program does not have an exit block");
  }

  // Reject on an .exit block with instructions
  if (!exitBlock->empty()) {
    exitError(P, "exit block cannot have instructions");
  }

  // Finally, run the program by following the CFG, starting from the .entry
  // block
  do {
    currentBlock = runBlockAndProgress(P, *currentBlock);
  } while (currentBlock);
}
} // namespace pilar
