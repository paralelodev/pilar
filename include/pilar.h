#include <map>
#include <stack>
#include <string>
#include <vector>

namespace pilar {
enum class Commands { GOTO, CHOOSE, PUSH, PRINT };

struct Instruction {
  Commands Command;
  int Constant;
  std::string LBranch;
  std::string RBranch;
};

struct Symbol {};

using Block = std::vector<Instruction>;
using Program = std::map<std::string, Block>;
using Dictionary = std::map<std::string, Symbol>;
using Memory = std::stack<int>;

void runProgram(Program &P);

} // namespace pilar
