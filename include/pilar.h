/** pilar.h                      **/
/** Header for the Pilar library **/

#include <map>
#include <stack>
#include <string>
#include <vector>

namespace pilar {
enum class Commands {
  GOTO,
  CHOOSE,
  PUSH,
  PRINT,
  STORE,
  LOAD,
  SUM,
  EQUAL,
  GREATER,
  NONE
  // TODO: add here more commands for *, /, -, etc
};

// An instruction is a command followed by a variable number of operands
struct Instruction {
  Commands Command;
  std::vector<std::string> Operands;
};

// A symbol has a value and the scope where the symbol was created.
struct Symbol {
  int Value;
  int Scope;
};

// Instructions are grouped into blocks
using Block = std::vector<Instruction>;

// Blocks are assigned labels
using BlockMap = std::map<std::string, Block>;

// Symbols are assinged identifiers
using SymbolMap = std::map<std::string, Symbol>;

// Computing happens over the elements of a stack
using IntStack = std::stack<int>;

// A program consists of 3 data structures:
// - A Block CFG for program control
// - A dictionary to store values into symbols
// - A stack for the values that have to be processed
struct Program {
  int CurrentScope = 0;
  BlockMap Blocks;
  SymbolMap Dictionary;
  IntStack Memory;
};

void runProgram(Program &P);

} // namespace pilar
