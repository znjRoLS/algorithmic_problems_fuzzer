#include "config.h"
#include "common.h"

#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <functional>

using namespace std;

unordered_map<string,function<unique_ptr<Variable>()>> variable_constructors = {
    {"int", []()->unique_ptr<Variable> {
      return unique_ptr<Variable>(new VariableInt());
    }},
    {"permutation", []()->unique_ptr<Variable> {
      return unique_ptr<Variable>(new VariablePermutation());
    }},
    {"char", []()->unique_ptr<Variable> {
      return unique_ptr<Variable>(new VariableChar());
    }},
};

Config::Config():
    rootBlock(new BlockComposition()) {

}

void Config::OpenFile(char *filename) {

  ifstream config_file(filename);

  if (!config_file.is_open()) {
    cout << "Error opening file " << filename << endl;
    exit(255);
  }

  Config::Parse(config_file);
}

void Config::Parse(ifstream& config_file) {
  string line;

  while (1) {
    if (!getline(config_file,line)) {
      break;
    }

    if (line == END_LINE) {
      break;
    }

    if (line == INPUT_START_LINE) {
      stringstream input_config;
      bool first = true;
      while (1) {
        getline(config_file,line);
        if (line == INPUT_END_LINE) {
          //config_file >> line;
          Config::ParseInput(input_config.str());
          break;
        }
        if (!first) input_config << endl;
        first = false;
        input_config << line;
      }
      continue;
    }
    if (line == VAR_START_LINE) {
      stringstream input_config;
      bool first = true;
      while (1) {
        getline(config_file, line);
        if (line == VAR_END_LINE) {
          //config_file >> line;
          Config::ParseVar(input_config.str());
          break;
        }
        if (!first) input_config << endl;
        first = false;
        input_config << line;
      }
      continue;
    }

    if (line == CONSTRAINTS_START_LINE) {
      stringstream input_config;
      bool first = true;
      while (1) {
        getline(config_file, line);
        if (line == CONSTRAINTS_END_LINE) {
          //config_file >> line;
          Config::ParseConstraints(input_config.str());
          break;
        }
        if (!first) input_config << endl;
        first = false;
        input_config << line;
      }
      continue;
    }


    //if (config_file.eof()) break;

    vector<string> tokens = split(line, '=');
    if (tokens.size() != 2) {
      //cout << "Error while parsing config! line: (skipping)" << line << endl;
      //exit(255);
      continue;
    }

    confParams[tokens[0]] = tokens[1];
  }

  string bin_prefix = confParams["binary_prefix"];
  string binaries_string = confParams["binaries"];

  vector<string> binaries_vector = split(binaries_string, ',');

  for(string& binary: binaries_vector) {
    binaries.push_back(bin_prefix + binary);
  }

}

void Config::ParseVar(string input) {
  //TODO add if already parsed this section.

  //cout << "ParseVar" << endl;
  //cout << input << endl;
  stringstream inputstream;
  inputstream << input;

  while (!inputstream.eof()) {
    string line;
    getline(inputstream,line);

    vector<string> tokens = split(line, ':');
    if (tokens.size() != 2) {
      //cout << "meeeh, just passing" << endl;
      break;
    }
    vector<string> varsVector = split(tokens[1], ',');

    //TODO@rols: check if exists
    //TODO@rols: hashmap for types?
    if (variable_constructors.find(tokens[0]) != variable_constructors.end()) {
      for(string &token: varsVector) {
        vars[token] = variable_constructors[tokens[0]]();
      }
    } else {
      cerr << "Variable type not defined! " << tokens[0] << endl;
    }
//    if (tokens[0] == "int") {
//      for(string &token: varsVector) {
//        vars[token] =  unique_ptr<Variable>(new VariableInt());
//        vars[token]->SetName(token);
//      }
//    } else if (tokens[0] == "permutation") {
//      for(string &token: varsVector) {
//        vars[token] = unique_ptr<Variable>(new VariablePermutation());
//        vars[token]->SetName(token);
//      }
//    } else if (tokens[0] == "char") {
//      for (string &token: varsVector) {
//
//      }
//    }
  }
}



void Config::ParseInput(string input) {

  //cout << "ParseInput" << endl;
  //cout << input << endl;
  //rootBlock = unique_ptr<BlockComposition>(new BlockComposition());
  shared_ptr<VariableIntConstant> unityVar = make_shared<VariableIntConstant>();
  unityVar->SetValue(1);
  rootBlock->SetRepeatVar(static_pointer_cast<Variable>(unityVar));

  stringstream inputstream;
  inputstream << input;

  if (!Block::GetNextInputBlock(inputstream, rootBlock, vars)) {
    cout << "Error opening file " << endl;
    exit(255);
  }
}

void Config::ParseConstraints(string input) {

  //cout << "ParseConstraints" << endl;
  //cout << input << endl;
  vector<string> lines = getLines(input);

  static const vector<string> operatorTokens = {"<=", ">=", "=", "<", ">"};

  for(string& line : lines) {
    size_t found;
    string foundToken;

    for (auto& operatorToken : operatorTokens) {
      found = line.find(operatorToken);
      if (found!=std::string::npos) {
        foundToken = operatorToken;
        break;
      }
    }

    string firstParam = line.substr(0,found);
    string secondParam = line.substr(found + foundToken.size());
    if (vars.find(firstParam) == vars.end()) {
      cout << "parse errorr! " << firstParam << endl;
      exit(255);
    }
    shared_ptr<Variable> firstVar = vars[firstParam];

    if (firstVar->GetType() == "char") {
      if (foundToken == "<")
        dynamic_pointer_cast<VariableChar>(firstVar)->SetUpperLimit(secondParam[0]-1);
      if (foundToken == ">")
        dynamic_pointer_cast<VariableChar>(firstVar)->SetLowerLimit(secondParam[0]+1);
        if (foundToken == "<=")
            dynamic_pointer_cast<VariableChar>(firstVar)->SetUpperLimit(secondParam[0]);
        if (foundToken == ">=")
            dynamic_pointer_cast<VariableChar>(firstVar)->SetLowerLimit(secondParam[0]);

      continue;
    } else if (firstVar->GetType() == "int") {
      if (vars.find(secondParam) != vars.end()) {

        if (foundToken == "<=") {
          firstVar->SetUpperLimit(vars[secondParam]);
          firstVar->SetUpperLimitInclusive(true);
        }
        if (foundToken == ">=") {
          firstVar->SetLowerLimit(vars[secondParam]);
          firstVar->SetLowerLimitInclusive(true);
        }
        if (foundToken == "<") {
          firstVar->SetUpperLimit(vars[secondParam]);
          firstVar->SetUpperLimitInclusive(false);
        }
        if (foundToken == ">") {
          firstVar->SetLowerLimit(vars[secondParam]);
          firstVar->SetLowerLimitInclusive(false);
        }
        if (foundToken == "=") {
          firstVar->SetUpperLimit(vars[secondParam]);
          firstVar->SetUpperLimitInclusive(true);
          firstVar->SetLowerLimit(vars[secondParam]);
          firstVar->SetLowerLimitInclusive(true);
        }
        continue;
      }
    }

    shared_ptr<VariableInt> tryFirstCast = dynamic_pointer_cast<VariableInt>(firstVar);
    if (!tryFirstCast) {
      cout << "var not an int!";
      exit(255);
    }
    //TODO: really? just int?
    int second = stoi(secondParam);

    if (foundToken == "<=") {
      tryFirstCast->SetUpperLimit(second);
      tryFirstCast->SetUpperLimitInclusive(true);
    }
    if (foundToken == ">=") {
      tryFirstCast->SetLowerLimit(second);
      tryFirstCast->SetLowerLimitInclusive(true);
    }
    if (foundToken == "<") {
      tryFirstCast->SetUpperLimit(second);
      tryFirstCast->SetUpperLimitInclusive(false);
    }
    if (foundToken == ">") {
      tryFirstCast->SetLowerLimit(second);
      tryFirstCast->SetLowerLimitInclusive(false);
    }
    if (foundToken == "=") {
      tryFirstCast->SetUpperLimit(second);
      tryFirstCast->SetUpperLimitInclusive(true);
      tryFirstCast->SetLowerLimit(second);
      tryFirstCast->SetLowerLimitInclusive(true);
    }
  }

}

vector<string> Config::GetBinaries() {
  return binaries;
}

string Config::GetInput() {
  return rootBlock->GetGeneratedText();
}

string Config::GetParam(string name, string defaultVal) {
  if (confParams.find(name) == confParams.end()) return defaultVal;
  return confParams[name];
}
