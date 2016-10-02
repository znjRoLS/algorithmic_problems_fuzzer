//
// Created by Bojan Roško on 10/2/16.
//

#include "variable.h"

#include <ctime>
#include <cstdlib>
#include <memory>
#include <string>

using namespace std;

//TODO: make template
inline void SetRandomVar(int x_min, int x_max, int &x) {
    x = rand() % (x_max - x_min + 1) + x_min;
}

inline void SetRandomVar(double x_min, double x_max, double &x) {
    x = (double)rand() / RAND_MAX;
    x = x_min + x * (x_max - x_min);
}


void VariableInt::GenerateValue() {
    if (isUpperLimitVar) {
        upperInt = stoi(upperVar->GetValue());
    }
    if (isLowerLimitVar) {
        lowerInt = stoi(lowerVar->GetValue());
    }

    SetRandomVar(lowerInt, upperInt, value);
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