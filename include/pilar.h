#include <map>
#include <stack>
#include <string_view>
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
  GREATER
};

struct Instruction {
  Commands Command;
  std::vector<std::string_view> Operands;
};

struct Symbol {
  int Value;
  int Scope;
};

using Block = std::vector<Instruction *>;
using BlockMap = std::map<std::string_view, Block *>;
using SymbolMap = std::map<std::string_view, Symbol>;
using IntStack = std::stack<int>;

struct Program {
  int CurrentScope = 0;
  BlockMap Blocks;
  SymbolMap Dictionary;
  IntStack Memory;
};

void runProgram(Program &P);

} // namespace pilar
