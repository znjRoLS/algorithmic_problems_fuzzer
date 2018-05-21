//
// Created by Bojan Roško on 5/12/18.
//

#ifndef FUZZER_LEARN_PARAM_H
#define FUZZER_LEARN_PARAM_H

#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using std::string;
using std::vector;
using std::stringstream;

enum PARAM_SCALE {LINEAR, LOG};

struct LearnParam {
    double minval, maxval;
    PARAM_SCALE scale;
    int numpoints;
    string name;
};

class LearnParamAllPermutations {
    vector<LearnParam> _params;
    vector<vector<double>> _param_vals;
    vector<int> _indexes;
    bool _valid;
    char ** _args;

public:
    LearnParamAllPermutations(vector<LearnParam> params);
    ~LearnParamAllPermutations();

    bool IsValid();

    void Reset();

    void Next();

    string ToString();

    char ** GetArgs();
};


#endif //FUZZER_LEARN_PARAM_H
