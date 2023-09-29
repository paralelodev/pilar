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
using Program = std::map<std::string, Block *>;
using Dictionary = std::map<std::string, int>;
using Memory = std::stack<int>;

void runProgram(Program &P);

} // namespace pilar
