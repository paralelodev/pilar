#include "dag.h"
#include <iostream>

namespace dag {

static std::string getAlias(std::string_view Identifier, StringMap &Dictionary,
                            unsigned int &Counter, bool isStore) {
  StringMap::iterator SI = Dictionary.find(Identifier.data());
  if (SI != Dictionary.end()) {
    return SI->second;
  }

  if (!isStore) {
    std::cerr << "Error: identifier does not exist\n";
    exit(0);
  }

  std::string Alias = "r" + std::to_string(Counter);
  Counter++;
  return Dictionary[Identifier.data()] = Alias;
}

void Assign::ToText(std::ostringstream &oss, StringMap &Dictionary,
                    unsigned int &Counter) {
  Value->ToText(oss, Dictionary, Counter);
  oss << "store " << getAlias(Identifier, Dictionary, Counter, true) << '\n';
};

void Fetch::ToText(std::ostringstream &oss, StringMap &Dictionary,
                   unsigned int &Counter) {
  oss << "load " << getAlias(Identifier, Dictionary, Counter, false) << '\n';
};

void DAG2Pilar(Block &DAG) {
  unsigned int Counter = 0;
  StringMap Dictionary;
  std::ostringstream PilarOutput;
  PilarOutput << ".entry" << '\n';
  for (Node *node : DAG.Instructions) {
    node->ToText(PilarOutput, Dictionary, Counter);
  }
  PilarOutput << "goto .exit" << '\n' << '\n' << ".exit" << '\n' << '\n';
  std::cout << PilarOutput.str();
}
} // namespace dag
