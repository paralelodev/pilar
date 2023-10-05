/** dag.cpp                            **/
/** Implementation for the DAG library **/

/** TODO: improve                      **/

#include "dag.h"
#include <iostream>

namespace dag {

static std::string getAlias(std::string_view Identifier, StringMap &Dictionary,
                            unsigned int &VariableCounter, bool isStore) {
  StringMap::iterator SI = Dictionary.find(Identifier.data());
  if (SI != Dictionary.end()) {
    return SI->second;
  }

  if (!isStore) {
    std::cerr << "Error: identifier does not exist\n";
    exit(0);
  }

  std::string Alias = "r" + std::to_string(VariableCounter);
  VariableCounter++;
  return Dictionary[Identifier.data()] = Alias;
}

void IntLiteral::ToText(std::ostringstream &oss, StringMap &Dictionary,
                        unsigned int &VariableCounter,
                        unsigned int &ThenCounter, unsigned int &ElseCounter,
                        unsigned int &ContinueCounter) const {
  oss << "push " << Value << '\n';
}

void Assign::ToText(std::ostringstream &oss, StringMap &Dictionary,
                    unsigned int &VariableCounter, unsigned int &ThenCounter,
                    unsigned int &ElseCounter,
                    unsigned int &ContinueCounter) const {
  Value.ToText(oss, Dictionary, VariableCounter, ThenCounter, ElseCounter,
               ContinueCounter);
  oss << "store " << getAlias(Identifier, Dictionary, VariableCounter, true)
      << '\n';
};

void Fetch::ToText(std::ostringstream &oss, StringMap &Dictionary,
                   unsigned int &VariableCounter, unsigned int &ThenCounter,
                   unsigned int &ElseCounter,
                   unsigned int &ContinueCounter) const {
  oss << "load " << getAlias(Identifier, Dictionary, VariableCounter, false)
      << '\n';
};

void Print::ToText(std::ostringstream &oss, StringMap &Dictionary,
                   unsigned int &VariableCounter, unsigned int &ThenCounter,
                   unsigned int &ElseCounter,
                   unsigned int &ContinueCounter) const {
  Value.ToText(oss, Dictionary, VariableCounter, ThenCounter, ElseCounter,
               ContinueCounter);
  oss << "print" << '\n';
}

void Block::ToText(std::ostringstream &oss, StringMap &Dictionary,
                   unsigned int &VariableCounter, unsigned int &ThenCounter,
                   unsigned int &ElseCounter,
                   unsigned int &ContinueCounter) const {
  for (Node *node : Instructions) {
    node->ToText(oss, Dictionary, VariableCounter, ThenCounter, ElseCounter,
                 ContinueCounter);
  }
}

void BinaryOperation::ToText(std::ostringstream &oss, StringMap &Dictionary,
                             unsigned int &VariableCounter,
                             unsigned int &ThenCounter,
                             unsigned int &ElseCounter,
                             unsigned int &ContinueCounter) const {
  LHS.ToText(oss, Dictionary, VariableCounter, ThenCounter, ElseCounter,
             ContinueCounter);
  RHS.ToText(oss, Dictionary, VariableCounter, ThenCounter, ElseCounter,
             ContinueCounter);
  switch (Op) {
  case Operators::ADD:
    oss << "+" << '\n';
    break;
  case Operators::EQUAL:
    oss << "==" << '\n';
    break;
  case Operators::GREATER:
    oss << ">" << '\n';
    break;

  default:

    break;
  }
}

void If::ToText(std::ostringstream &oss, StringMap &Dictionary,
                unsigned int &VariableCounter, unsigned int &ThenCounter,
                unsigned int &ElseCounter,
                unsigned int &ContinueCounter) const {
  BO.ToText(oss, Dictionary, VariableCounter, ThenCounter, ElseCounter,
            ContinueCounter);

  if (ThenPart.IsValid() && ElsePart.IsValid()) {
    oss << "choose .then" << ThenCounter << " .else" << ElseCounter << '\n';
  } else if (ThenPart.IsValid()) {
    oss << "choose .then" << ThenCounter << " .continue" << ContinueCounter
        << '\n';
  } else {
    std::cerr << "Error: then section is missing" << '\n';
    exit(0);
  }

  if (ThenPart.IsValid()) {
    oss << "\n.then" << ThenCounter << '\n';
    ThenPart.ToText(oss, Dictionary, VariableCounter, ThenCounter, ElseCounter,
                    ContinueCounter);
    oss << "goto "
        << ".continue" << ContinueCounter << '\n';
    ThenCounter++;
  }

  if (ElsePart.IsValid()) {
    oss << "\n.else" << ElseCounter << '\n';
    ;
    ElsePart.ToText(oss, Dictionary, VariableCounter, ThenCounter, ElseCounter,
                    ContinueCounter);
    oss << "goto "
        << ".continue" << ContinueCounter;
    ElseCounter++;
  }

  oss << "\n.continue" << ContinueCounter << '\n';

  ContinueCounter++;
}

void DAG2Pilar(Block &DAG) {
  unsigned int VariableCounter = 0;
  unsigned int ThenCounter = 0;
  unsigned int ElseCounter = 0;
  unsigned int ContinueCounter = 0;
  StringMap Dictionary;
  std::ostringstream PilarOutput;

  PilarOutput << ".entry" << '\n';
  
  for (Node *node : DAG.Instructions) {
    node->ToText(PilarOutput, Dictionary, VariableCounter, ThenCounter,
                 ElseCounter, ContinueCounter);
  }
  
  PilarOutput << "goto .exit" << '\n' << '\n' << ".exit" << '\n';
  
  std::cout << PilarOutput.str();
}
} // namespace dag
