//
// Created by Bojan Roško on 10/2/16.
//

#ifndef FUZZER_VARIABLE_H
#define FUZZER_VARIABLE_H

#include <memory>
#include <string>

using namespace std;

class Variable {
public:
  Variable();
  void SetUpperLimit(shared_ptr<Variable> var);
  void SetLowerLimit(shared_ptr<Variable> var);
  void SetUpperLimitInclusive(bool);
  void SetLowerLimitInclusive(bool);
  virtual string GetValue() = 0;
  virtual void GenerateValue() = 0;
  void SetName(string);
  string GetName();

protected:
  bool isUpperLimitVar;
  bool isLowerLimitVar;
  bool isUpperLimitSet;
  bool isLowerLimitSet;
  bool isUpperLimitInclusive = false;
  bool isLowerLimitInclusive = false;

  shared_ptr<Variable> lowerVar;
  shared_ptr<Variable> upperVar;
private:
  string name;
};

class VariableIntConstant: public Variable {
public:
  string GetValue() override;
  void GenerateValue() override;

  void SetValue(int val);

private:
  int value;
};

class VariableInt: public Variable {
public:
  string GetValue() override;
  void GenerateValue() override;

  using Variable::SetLowerLimit;
  using Variable::SetUpperLimit;
  void SetUpperLimit(int val);
  void SetLowerLimit(int val);

private:
  int upperInt;
  int lowerInt;
  int value;
};


class VariableDouble: public Variable {
public:
  string GetValue() override;
  void GenerateValue() override;

  using Variable::SetLowerLimit;
  using Variable::SetUpperLimit;
  void SetUpperLimit(double val);
  void SetLowerLimit(double val);

private:
  double upperInt;
  double lowerInt;
  double value;
};



#endif //FUZZER_VARIABLE_H
