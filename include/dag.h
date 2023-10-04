#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace dag {
enum class Operators { ADD, EQUAL, GREATER };

using StringMap = std::map<std::string, std::string>;

struct Node {
  virtual void ToText(std::ostringstream &oss, StringMap &Dictionary,
                      unsigned int &VariableCounter, unsigned int &ThenCounter,
                      unsigned int &ElseCounter,
                      unsigned int &ContinueCounter) const = 0;
  virtual bool IsValid() const = 0;
};

struct Block : Node {
  std::vector<Node *> Instructions;
  Block(std::vector<Node *> instructions) : Instructions(instructions) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) const override;
  bool IsValid() const override { return !Instructions.empty(); }
};

struct Assign : Node {
  std::string Identifier;
  const Node &Value;
  Assign(std::string identifier, const Node &value)
      : Identifier(identifier), Value(value) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) const override;
  bool IsValid() const override { return true; }
};

struct IntLiteral : Node {
  int Value;
  IntLiteral(int value) : Value(value) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) const override;
  bool IsValid() const override { return true; }
};

struct BinaryOperation : Node {
  Operators Op;
  const Node &LHS;
  const Node &RHS;
  BinaryOperation(Operators op, const Node &lhs, const Node &rhs)
      : Op(op), LHS(lhs), RHS(rhs) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) const override;
  bool IsValid() const override { return true; }
};

struct If : Node {
  const BinaryOperation &BO;
  const Node &ThenPart;
  const Node &ElsePart;
  If(const BinaryOperation &bo, const Node &ThenPart, const Block &elsePart)
      : BO(bo), ThenPart(ThenPart), ElsePart(elsePart) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) const override;
  bool IsValid() const override { return true; }
};

struct Print : Node {
  const Node &Value;
  Print(const Node &value) : Value(value) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) const override;
  bool IsValid() const override { return true; }
};

struct Fetch : Node {
  std::string Identifier;
  Fetch(std::string identifier) : Identifier(identifier) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) const override;
  bool IsValid() const override { return Identifier.size(); }
};

void DAG2Pilar(Block &DAG);

} // namespace dag
