//
// Created by Bojan Roško on 5/9/18.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <pthread.h>
#include <map>

#include "common.h"
#include "binary_runner.h"
#include "config.h"
#include "learn_param.h"

using namespace std;

int main(int argc, char **argv) {

    if (argc != 2) {
        cout << "Please run program as learn configfile" << endl;
        return 0;
    }

    //Config config(argv[1]);
    Config config;
    config.OpenFile(argv[1]);

    //srand(time(0));

    int max_interations = stoi(config.GetParam("max_iterations", "1000"));
    string generator_binary = config.GetParam("generator_binary", "");
    bool generate_binary = generator_binary != "";
    int iterations_step = stoi(config.GetParam("iterations_step", "10"));
    string binary_path = config.GetBinaries()[0];
    int iterations_per_learn = stoi(config.GetParam("iterations_per_learn", "1"));

    vector<LearnParam> learn_parameters = config.GetLearnParameters();
    LearnParamAllPermutations learner(learn_parameters);

    learner.Reset();

    int global_iter_num = 0;
    multimap<double,string> global_results;
    while (learner.IsValid()) {

        Binary prog(binary_path);

        cout << "Learner params: " << learner.ToString() << endl;

        int num = 0;
        double max_score = 0;
        double min_score = 0;
        double sum = 0;

        double thresh = 200;

        for (int iter_num = 0; iter_num < iterations_per_learn; iter_num++) {

            string program_input;

            if (generate_binary) {
                Binary prog(config.GetParam("binary_prefix", "") + generator_binary);
                program_input = prog.Run("");
            } else {
                program_input = config.GetInput();
            }

//            cout << "Running: " << binary_path << endl;
//            cout <<	"With input: " << program_input << endl;

            char **args = learner.GetArgs();
            char **real_args = new char*[learn_parameters.size() + 2];
            real_args[0] = new char[100];
            memcpy(real_args[0], binary_path.c_str(), binary_path.size());
            real_args[0][binary_path.size()] = '\0';
            for (int i = 0 ; i < learn_parameters.size(); i ++) real_args[i+1] = args[i];
            real_args[learn_parameters.size()+1] = nullptr;

            string output = prog.Run(program_input, real_args);

//            cout << "OUT " << output << endl;
            double score = stod(output);

            if (score > thresh) {
                num ++;
                if (num == 1) {
                    min_score = max_score = sum = score;
                } else {
                    min_score = min (min_score, score);
                    max_score = max (max_score, score);
                    sum += score;
                }
            }

            cout << "iter " << iter_num << "\tcurr\t" << score << "\tmin\t" << min_score \
                << "\tmax\t" << max_score << "\tavg\t" << sum/num << endl;

            //delete [] real_args[learn_parameters.size() + 1];
            delete [] real_args[0];
            delete [] real_args;

//            double score = stod(output);

            if (++global_iter_num == max_interations) return 0;
        }

        global_results.insert({sum/num, learner.ToString()});

        learner.Next();
    }

    for (auto par : global_results) {
        cout << par.first << " : " << par.second << endl;
    }


    return 0;
}
