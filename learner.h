//
// Created by Bojan Roško on 5/20/18.
//

#ifndef FUZZER_LEARNER_H
#define FUZZER_LEARNER_H

#include "learn_param.h"

class Learner {

public:
    Learner(vector<LearnParam> learn_params);

    vector<vector<string>> GetNextGeneration();

    void GiveFeedback(vector<double> scores);
};


#endif //FUZZER_LEARNER_H
