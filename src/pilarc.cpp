/** pilarc.cpp                            **/
/** Implementation for the Pilar frontend **/

#include "pilar.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

using namespace pilar;

static void exitError(std::string_view error) {
  std::cout << "Error: " << error << '\n';
  exit(0);
}

static bool isNumber(std::string_view Token) {
  char *p;
  strtol(Token.data(), &p, 10);
  return *p == 0;
}

// This is a VERY basic parser
// TODO: improve!
static Block *parseString(Program &P, std::string &Inst, Block *CurrentBlock) {
  std::istringstream StringStream(Inst);
  std::string Token, LastToken;
  Commands CurrentCommand = Commands::NONE;
  unsigned int TokenCount = 0;
  bool Completed = false;

  while (StringStream >> Token) {
    if (TokenCount == 0) {
      if (Token[0] == '.') {
        if (P.Blocks.find(Token) != P.Blocks.end()) {
          exitError("block is repeated");
        }
        P.Blocks[Token] = {};
        CurrentBlock = &P.Blocks[Token];
        Completed = true;
      } else if (Token == "push" || Token == "store" || Token == "load" ||
                 Token == "goto" || Token == "==" || Token == "choose" ||
                 Token == "print" || Token == "+" || Token == ">") {
        if (!CurrentBlock) {
          exitError("instruction does not belong to a block");
        }
        if (Token == "push") {
          CurrentCommand = Commands::PUSH;
        } else if (Token == "store") {
          CurrentCommand = Commands::STORE;
        } else if (Token == "load") {
          CurrentCommand = Commands::LOAD;
        } else if (Token == "goto") {
          CurrentCommand = Commands::GOTO;
        } else if (Token == "==") {
          CurrentCommand = Commands::EQUAL;
          CurrentBlock->push_back({CurrentCommand, {}});
          Completed = true;
        } else if (Token == "print") {
          CurrentCommand = Commands::PRINT;
          CurrentBlock->push_back({CurrentCommand, {}});
          Completed = true;
        } else if (Token == "+") {
          CurrentCommand = Commands::SUM;
          CurrentBlock->push_back({CurrentCommand, {}});
          Completed = true;
        } else if (Token == ">") {
          CurrentCommand = Commands::GREATER;
          CurrentBlock->push_back({CurrentCommand, {}});
          Completed = true;
        } else if (Token == "choose") {
          CurrentCommand = Commands::CHOOSE;
        }
      } else {
        exitError("Not supported");
      }
    } else if (TokenCount == 1) {
      if (CurrentCommand == Commands::NONE ||
          CurrentCommand == Commands::EQUAL ||
          CurrentCommand == Commands::PRINT ||
          CurrentCommand == Commands::SUM ||
          CurrentCommand == Commands::GREATER) {
        exitError("unexpected token");
      } else if (CurrentCommand == Commands::PUSH) {
        if (!isNumber(Token)) {
          exitError("push command accepts only integers");
        }
        CurrentBlock->push_back({CurrentCommand, {Token}});
        Completed = true;
      } else if (CurrentCommand == Commands::STORE ||
                 CurrentCommand == Commands::LOAD) {
        if (isNumber(Token) || Token[0] == '.') {
          exitError("command accepts only identifiers");
        }
        CurrentBlock->push_back({CurrentCommand, {Token}});
        Completed = true;
      } else if (CurrentCommand == Commands::GOTO ||
                 CurrentCommand == Commands::CHOOSE) {
        if (Token[0] != '.') {
          exitError("command accepts only block labels");
        }
        if (CurrentCommand == Commands::GOTO) {
          CurrentBlock->push_back({CurrentCommand, {Token}});
          Completed = true;
        }
      } else {
        exitError("Not supported");
      }
    } else {
      if (CurrentCommand == Commands::NONE ||
          CurrentCommand == Commands::EQUAL ||
          CurrentCommand == Commands::PRINT ||
          CurrentCommand == Commands::SUM ||
          CurrentCommand == Commands::GREATER) {
        exitError("unexpected token");
      } else if (CurrentCommand == Commands::PUSH ||
                 CurrentCommand == Commands::STORE ||
                 CurrentCommand == Commands::LOAD ||
                 CurrentCommand == Commands::GOTO) {
        exitError("command accepts only one argument");
      } else if (CurrentCommand == Commands::CHOOSE) {
        if (Token[0] != '.') {
          exitError("command accepts only block labels");
        }
        CurrentBlock->push_back({CurrentCommand, {LastToken, Token}});
        Completed = true;
      } else {
        exitError("Not supported");
      }
    }
    TokenCount++;
    LastToken = Token;
  }

  if (!Completed) {
    exitError("incomplete command");
    std::cout << Inst << '\n';
  }

  return CurrentBlock;
}

int main(int argc, char *argv[]) {
  // Reject on invalid number of parameters
  if (argc != 2) {
    exitError("wrong number of arguments");
  }

  std::string Filename = argv[1];
  std::fstream PilarFile;
  PilarFile.open(Filename, std::ios::in);

  // Reject on invalid file
  if (!PilarFile.is_open()) {
    exitError("could not open the file");
  }

  Program P;

  Block *CurrentBlock = nullptr;

  // Parse the file line by line
  std::string Inst;
  while (getline(PilarFile, Inst)) {
    // Skip on empty lines
    if (!Inst.empty()) {
      CurrentBlock = parseString(P, Inst, CurrentBlock);
    }
  }
  PilarFile.close();

  // Execute the parsed program
  runProgram(P);
}
