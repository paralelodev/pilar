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
  GREATER
};

struct Instruction {
  Commands Command;
  std::vector<std::string> Operands;
};

using Block = std::vector<Instruction *>;
using BlockMap = std::map<std::string, Block *>;
using IntMap = std::map<std::string, int>;
using IntStack = std::stack<int>;

struct Program {
  BlockMap Blocks;
  IntMap Dictionary;
  IntStack Memory;
};

void runProgram(Program &P);

} // namespace pilar
