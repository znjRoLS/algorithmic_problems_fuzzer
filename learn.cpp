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

#define CR 0.7122
#define F 0.6301

struct agent {
    vector<LearnParam> params;
    vector<double> vals;
    double score;

    string ToString() {
        stringstream ss;
        for (string s : GetArgs()) {
            ss << s << " ";
        }
        return ss.str();
    }

    vector<string> GetArgs() {
        vector<string> res;
        for (int i = 0 ; i < params.size(); i ++) {
            res.push_back(params[i].name + "=" + to_string(vals[i]));
        }
        return res;
    }
};

double getdiff(agent& lhs, agent& rhs) {
    double sc = 0;
    for (int i = 0; i < lhs.vals.size(); i ++) {
        sc += fabs(lhs.vals[i] - rhs.vals[i]);
    }
    return sc;
}

double getmaxdiff(vector<agent>& agents) {
    double sc = 0;
    for (int i = 1; i < agents.size(); i ++) {
        sc = max(sc, getdiff(agents[0], agents[i]));
    }
    return sc;
}

double getalldiff(vector<agent>& agents) {
    double sc = 0;
    for (int i = 0; i < agents.size(); i ++) {
        for (int j = i + 1; j < agents.size() ; j ++) {
            sc += getdiff(agents[j], agents[i]);
        }
    }
    return sc;
}

agent get_new(vector<agent>& agents, int x) {
    agent new_agent = agents[x];
    int a=x, b=x, c=x;
    while(a == x) {
        a = rand() % agents.size();
    }
    while (b == a || b == x) {
        b=rand() % agents.size();
    }
    while(c == b || c == a || c == x) {
        c = rand() % agents.size();
    }

    int R = rand() % agents[x].params.size();

    for (int i = 0 ; i < agents[x].params.size(); i ++) {
        double r = (double)rand()/RAND_MAX;
        if (r < CR || i == R) {
            new_agent.vals[i] = agents[a].vals[i] + F * (agents[b].vals[i] - agents[c].vals[i]);
        }
    }
    return new_agent;
}

int main(int argc, char **argv) {

    srand(time(0));

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
    vector<agent> agents;
    vector<double> vals(learn_parameters.size(), 0);
    agents.push_back({learn_parameters, vals});
    for (int i = 0 ; i < learn_parameters.size(); i ++) {
        LearnParam& param = learn_parameters[i];
        double step_val = (param.maxval - param.minval) / param.numpoints;
        for (int step = 0; step < param.numpoints; step ++) {
            double val = param.minval + step * step_val;
            if (fabs(val) < 1e-7) continue;
            agent a;
            a.params = learn_parameters;
            a.vals = vals;
            a.vals[i] = val;
            agents.push_back(a);
        }
    }

    int global_iter_num = 0;

//    /////////////////
//
//    map<int, int> blah;
//    map<int,int> avg_blah;
//
//    for (int i = 0 ; i < 30 ; i ++) {
//        vector<string> inputs;
//        for (int iter_num = 0; iter_num < iterations_per_learn; iter_num++) {
//
//            string program_input;
//
//            if (generate_binary) {
//                Binary prog(config.GetParam("binary_prefix", "") + generator_binary);
//                program_input = prog.Run("");
//            } else {
//                program_input = config.GetInput();
//            }
//
//            inputs.push_back(program_input);
//        }
//
//        Binary prog(binary_path);
//
//
//        int num = 0;
//        double max_score = 0;
//        double min_score = 0;
//        double sum = 0;
//        double thresh = 20;
//
//        for (int iter_num = 0; iter_num < iterations_per_learn; iter_num++) {
//            string program_input = inputs[iter_num];
//
////            cout << "Running: " << binary_path << endl;
////            cout <<	"With input: " << program_input << endl;
//
//            string output = prog.Run(program_input, agents[0].GetArgs());
//
////            cout << "OUT " << output << endl;
//            double score = stod(output);
//
//            if (score > thresh) {
//                num++;
//                if (num == 1) {
//                    min_score = max_score = sum = score;
//                } else {
//                    min_score = min(min_score, score);
//                    max_score = max(max_score, score);
//                    sum += score;
//                }
//
//                blah[(int)score] ++;
//            }
//
//
//            cout << "iter " << iter_num << "\tcurr\t" << score << "\tmin\t" << min_score \
// << "\tmax\t" << max_score << "\tavg\t" << sum / num << endl;
//
//
//            if (++global_iter_num == max_interations) return 0;
//        }
//
//        cout << "i " << i << " avg " << sum / num << endl << endl;
//        avg_blah[(int)(sum/num)] ++;
//
//    }
//
//    // Compute mean (average of elements)
//    int sum = 0;
//    int n = 0;
//    for (auto par : blah) {
//        sum += par.first * par.second;
//        n += par.second;
//    }
//    double mean = (double)sum /
//                  (double)n;
//
//
//    // Compute sum squared
//    // differences with mean.
//    double sqDiff = 0;
//    for (auto par : blah)
//        sqDiff += (par.first - mean) *
//                  (par.first - mean) * par.second;
//     sqDiff / n;
//
//     cout << "mean " << mean << " | var " << sqDiff / n << endl;
//     for (auto par : blah) {
//         cout << par.first << "\t" << par.second << endl;
//     }
//
//
//    // Compute mean (average of elements)
//    sum = 0;
//    n = 0;
//    for (auto par : avg_blah) {
//        sum += par.first * par.second;
//        n += par.second;
//    }
//    mean = (double)sum /
//                  (double)n;
//
//
//    // Compute sum squared
//    // differences with mean.
//    sqDiff = 0;
//    for (auto par : avg_blah)
//        sqDiff += (par.first - mean) *
//                  (par.first - mean) * par.second;
//    sqDiff / n;
//
//    cout << "avgmean " << mean << " | avgvar " << sqDiff / n << endl;
//    for (auto par : avg_blah) {
//        cout << par.first << "\t" << par.second << endl;
//    }
//
//
//    exit(0);
//
//    ///////////


    vector<string> inputs;
    for (int iter_num = 0; iter_num < iterations_per_learn; iter_num++) {

        string program_input;

        if (generate_binary) {
            Binary prog(config.GetParam("binary_prefix", "") + generator_binary);
            program_input = prog.Run("");
        } else {
            program_input = config.GetInput();
        }

        inputs.push_back(program_input);
    }

    vector<double> agent_scores(agents.size(), 0);

    for (int i = 0 ; i < agents.size(); i ++) {

        Binary prog(binary_path);

        cout << "agent " << i << " params: " << agents[i].ToString() << endl;

        int num = 0;
        double max_score = 0;
        double min_score = 0;
        double sum = 0;

        double thresh = 20;



        for (int iter_num = 0; iter_num < iterations_per_learn; iter_num++) {
            string program_input = inputs[iter_num];

//            cout << "Running: " << binary_path << endl;
//            cout <<	"With input: " << program_input << endl;

            string output = prog.Run(program_input, agents[i].GetArgs());

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


            if (++global_iter_num == max_interations) return 0;
        }

        agent_scores[i] = sum/num;
    }

    int curr_agent = 0;
    while(getalldiff(agents) > 1) {

        cout << "---- alll" << endl;
        for (int i = 0 ; i < agents.size(); i ++) {
            cout << "agent all " << i << " score " << agent_scores[i] << " params: " << agents[i].ToString() << endl;
        }
        cout << getalldiff(agents) << endl;
        cout << "---------" << endl;

        agent potential_new = get_new(agents, curr_agent);


        Binary prog(binary_path);

        cout << "agent potential new " << curr_agent << " params: " << potential_new.ToString() << endl;

        int num = 0;
        double max_score = 0;
        double min_score = 0;
        double sum = 0;
        double thresh = 20;

        for (int iter_num = 0; iter_num < iterations_per_learn; iter_num++) {
            string program_input = inputs[iter_num];

            string output = prog.Run(program_input, potential_new.GetArgs());

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


            if (++global_iter_num == max_interations) return 0;
        }

        if (sum/num > agent_scores[curr_agent]) {
            agent_scores[curr_agent] = sum/num;
            agents[curr_agent] = potential_new;
        }

        curr_agent++;
        if (curr_agent == agents.size()) {
            curr_agent = 0;
            inputs.clear();
            for (int iter_num = 0; iter_num < iterations_per_learn; iter_num++) {

                string program_input;

                if (generate_binary) {
                    Binary prog(config.GetParam("binary_prefix", "") + generator_binary);
                    program_input = prog.Run("");
                } else {
                    program_input = config.GetInput();
                }

                inputs.push_back(program_input);
            }
        }
    }

    for (int i = 0; i < agents.size(); i ++) {
        cout << agents[i].ToString() << " : " << agent_scores[i] << endl;
    }


    return 0;
}
