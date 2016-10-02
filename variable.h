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
    void SetUpperLimit(shared_ptr<Variable> var)
    { upperVar = var; isUpperLimitVar = true; }
    void SetLowerLimit(shared_ptr<Variable> var)
    { lowerVar = var; isLowerLimitVar = true;	}
    virtual string GetValue() = 0;
    virtual void GenerateValue() = 0;
protected:
    bool isUpperLimitVar;
    bool isLowerLimitVar;

    shared_ptr<Variable> lowerVar;
    shared_ptr<Variable> upperVar;
};


class VariableInt: public Variable {
public:
    string GetValue() override;
    void GenerateValue() override;

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

    void SetUpperLimit(double val);
    void SetLowerLimit(double val);

private:
    double upperInt;
    double lowerInt;
    double value;
};



#endif //FUZZER_VARIABLE_H
