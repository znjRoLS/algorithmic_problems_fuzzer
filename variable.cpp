//
// Created by Bojan Roško on 10/2/16.
//

#include "variable.h"

#include <ctime>
#include <cstdlib>
#include <memory>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>

using namespace std;

bool isSeedSet = false;

inline void SetSeedIfNeeded(){
  if (!isSeedSet) {
    isSeedSet = true;
    srand(time(NULL));
  }
}

//TODO: make template
inline void SetRandomVar(int x_min, int x_max, int &x) {
  SetSeedIfNeeded();
  x = rand() % (x_max - x_min + 1) + x_min;
}

inline void SetRandomVar(double x_min, double x_max, double &x) {
  SetSeedIfNeeded();
  x = (double)rand() / RAND_MAX;
  x = x_min + x * (x_max - x_min);
}

inline void SetRandomVar(char x_min, char x_max, char &x) {
  SetSeedIfNeeded();
  x = rand() % (x_max - x_min + 1) + x_min;
}

inline void SetRandomPermutation(int x_min, int x_max, vector<int>& perm) {
  SetSeedIfNeeded();
  perm.clear();
  for(int i = x_min;i <= x_max; i ++) {
    perm.push_back(i);
  }
  auto engine = std::default_random_engine{};
  std::shuffle(std::begin(perm), std::end(perm), engine);
}


Variable::Variable():
    isUpperLimitVar(false), isLowerLimitVar(false), isUpperLimitInclusive(true), isLowerLimitInclusive(true) {}

void Variable::SetName(string n) {
  name = n;
}

string Variable::GetName() {
  return name;
}

void Variable::SetUpperLimit(shared_ptr<Variable> var) {
  upperVar = var;
  isUpperLimitVar = true;
}

void Variable::SetLowerLimit(shared_ptr<Variable> var) {
  lowerVar = var;
  isLowerLimitVar = true;
}

void Variable::SetUpperLimitInclusive(bool val) {
  isUpperLimitInclusive = val;
}

void Variable::SetLowerLimitInclusive(bool val) {
  isLowerLimitInclusive = val;
}

void VariableInt::GenerateValue() {
  if (isUpperLimitVar) {
    upperInt = stoi(upperVar->GetValue());
  }
  if (isLowerLimitVar) {
    lowerInt = stoi(lowerVar->GetValue());
  }

  int lowerLimit = lowerInt;
  if (!isLowerLimitInclusive) lowerLimit++;
  int upperLimit = upperInt;
  if (!isUpperLimitInclusive) upperLimit--;

  SetRandomVar(lowerLimit, upperLimit, value);
}

string VariableInt::GetValue() {
  return to_string(value);
}

void VariableInt::SetUpperLimit(int val) {
  upperInt = val;
  isUpperLimitVar = false;
}

void VariableInt::SetLowerLimit(int val) {
  lowerInt = val;
  isLowerLimitVar = false;
}

string VariableInt::GetType() {
  return "int";
}


void VariableDouble::GenerateValue() {
  if (isUpperLimitVar) {
    upperInt = stof(upperVar->GetValue());
  }
  if (isLowerLimitVar) {
    lowerInt = stof(lowerVar->GetValue());
  }

  SetRandomVar(lowerInt, upperInt, value);
}


string VariableDouble::GetValue() {
  return to_string(value);
};

void VariableDouble::SetUpperLimit(double val) {
  upperInt = val;
  isUpperLimitVar = false;
};

void VariableDouble::SetLowerLimit(double val) {
  lowerInt = val;
  isLowerLimitVar = false;
};


string VariableDouble::GetType() {
  return "double";
}

void VariableIntConstant::GenerateValue() {
}


void VariableIntConstant::SetValue(int val) {
  value = val;
}

string VariableIntConstant::GetValue() {
  return to_string(value);
};


string VariableIntConstant::GetType() {
  return "int";
}


void VariablePermutation::GenerateValue() {
  if (isUpperLimitVar) {
    upperInt = stoi(upperVar->GetValue());
  }
  if (isLowerLimitVar) {
    lowerInt = stoi(lowerVar->GetValue());
  }

  int lowerLimit = lowerInt;
  if (!isLowerLimitInclusive) lowerLimit++;
  int upperLimit = upperInt;
  if (!isUpperLimitInclusive) upperLimit--;

  SetRandomPermutation(lowerLimit, upperLimit, permutation);
}

string VariablePermutation::GetValue() {
  stringstream ss;
  for(int i = 0; i < permutation.size(); i ++) {
    if (i!= 0) ss << " ";
    ss << permutation[i];
  }

  return ss.str();
}

string VariablePermutation::GetType() {
  return "permutation";
}


void VariableChar::SetLower() {
  isUpper = false;
}

void VariableChar::SetUpper() {
  isUpper = true;
}

void VariableChar::GenerateValue() {
  SetRandomVar('a', 'z', value);
  if (isUpper) {
    value += 'A' - 'a';
  }
}

string VariableChar::GetValue() {
  return string(1, value);
}

string VariableChar::GetType() {
  return "char";
}