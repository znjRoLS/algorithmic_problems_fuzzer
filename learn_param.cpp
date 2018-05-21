//
// Created by Bojan Roško on 5/12/18.
//

#include "learn_param.h"

LearnParamAllPermutations::LearnParamAllPermutations(vector<LearnParam> params):
_params(params),
_param_vals(params.size()),
_indexes(params.size(), 0),
_valid(false) {
    for (int i = 0 ; i < _params.size(); i ++) {
        LearnParam& curr_param = _params[i];
        for (int iter = 0; iter < curr_param.numpoints; iter ++) {
            if (curr_param.scale == LINEAR) {
                double val = (curr_param.maxval - curr_param.minval) * iter / (curr_param.numpoints-1) + curr_param.minval;
                _param_vals[i].push_back(val);
            } else if (curr_param.scale == LOG) {
                double logmax = log(curr_param.maxval);
                double logmin = log(curr_param.minval);
                double val = (logmax - logmin) * iter / (curr_param.numpoints-1) + logmin;
                _param_vals[i].push_back(val);
            }
        }
    }


}

LearnParamAllPermutations::~LearnParamAllPermutations() {
}

bool LearnParamAllPermutations::IsValid() {
    return _valid;
}

void LearnParamAllPermutations::Reset() {
    _valid = true;
    for (int i = 0 ; i < _indexes.size(); i ++) _indexes[i] = 0;
}

void LearnParamAllPermutations::Next() {
    _indexes[0]++;
    int currind = 0;
    while (currind+1 < _indexes.size() && _indexes[currind] == _param_vals[currind].size()) {
        _indexes[currind] = 0;
        currind++;
        _indexes[currind] ++;
    }
    if (_indexes[currind] == _param_vals[currind].size()) {
        _valid = false;
    }
}

string LearnParamAllPermutations::ToString() {
    stringstream ss;
    for (int i = 0 ; i < _params.size(); i++) {
        ss << " " << _params[i].name << "=" << _param_vals[i][_indexes[i]];
    }
    return ss.str();
}

vector<string> LearnParamAllPermutations::GetArgs() {
    vector<string> res;
    for (int i = 0 ; i < _params.size(); i++) {
        string arg = _params[i].name + "=" + std::to_string(_param_vals[i][_indexes[i]]);
        res.push_back(arg);
    }
    return res;
}