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
                      unsigned int &ContinueCounter) = 0;
  virtual bool IsValid() = 0;
};

struct Block : Node {
  std::vector<Node *> Instructions;
  Block(std::vector<Node *> instructions) : Instructions(instructions) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) override;
  bool IsValid() override { return !Instructions.empty(); }
};

struct Assign : Node {
  std::string Identifier;
  Node *Value;
  Assign(std::string identifier, Node *value)
      : Identifier(identifier), Value(value) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) override;
  bool IsValid() override { return Value; }
};

struct IntLiteral : Node {
  int Value;
  IntLiteral(int value) : Value(value) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) override;
  bool IsValid() override { return true; }
};

struct BinaryOperation : Node {
  Operators Op;
  Node *LHS;
  Node *RHS;
  BinaryOperation(Operators op, Node *lhs, Node *rhs)
      : Op(op), LHS(lhs), RHS(rhs) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) override;
  bool IsValid() override { return LHS && RHS; }
};

struct If : Node {
  BinaryOperation *BO;
  Node *ThenPart;
  Node *ElsePart;
  If(BinaryOperation *bo, Node *ThenPart, Block *elsePart)
      : BO(bo), ThenPart(ThenPart), ElsePart(elsePart) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) override;
  bool IsValid() override { return BO && ThenPart && ElsePart; }
};

struct Print : Node {
  Node *Value;
  Print(Node *value) : Value(value) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) override;
  bool IsValid() override { return Value; }
};

struct Fetch : Node {
  std::string Identifier;
  Fetch(std::string identifier) : Identifier(identifier) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &VariableCounter, unsigned int &ThenCounter,
              unsigned int &ElseCounter,
              unsigned int &ContinueCounter) override;
  bool IsValid() override { return Identifier.size(); }
};

void DAG2Pilar(Block &DAG);

} // namespace dag
