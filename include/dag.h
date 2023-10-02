#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace dag {
enum class Operators { ADD };

using StringMap = std::map<std::string, std::string>;

struct Node {
  virtual void ToText(std::ostringstream &oss, StringMap &Dictionary,
                      unsigned int &Counter) = 0;
};

struct Block : Node {
  std::vector<Node *> Instructions;
  Block(std::vector<Node *> instructions) : Instructions(instructions) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &Counter) override{};
};

struct Assign : Node {
  std::string Identifier;
  Node *Value;
  Assign(std::string identifier, Node *value)
      : Identifier(identifier), Value(value) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &Counter);
};

struct IntLiteral : Node {
  int Value;
  IntLiteral(int value) : Value(value) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &Counter) override {
    oss << "push " << Value << '\n';
  }
};

struct BinaryOperation : Node {
  Operators Op;
  Node *LHS;
  Node *RHS;
  BinaryOperation(Operators op, Node *lhs, Node *rhs)
      : Op(op), LHS(lhs), RHS(rhs) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &Counter) override {
    RHS->ToText(oss, Dictionary, Counter);
    LHS->ToText(oss, Dictionary, Counter);
    switch (Op) {
    case Operators::ADD:
      oss << "+" << '\n';
      break;
    }
  }
};

struct Print : Node {
  Node *Value;
  Print(Node *value) : Value(value) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &Counter) override {
    Value->ToText(oss, Dictionary, Counter);
    oss << "print " << '\n';
  }
};

struct Fetch : Node {
  std::string Identifier;
  Fetch(std::string identifier) : Identifier(identifier) {}
  void ToText(std::ostringstream &oss, StringMap &Dictionary,
              unsigned int &Counter) override;
};

void DAG2Pilar(Block &DAG);

} // namespace dag
