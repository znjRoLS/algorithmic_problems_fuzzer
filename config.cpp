#include "config.h"
#include "common.h"

#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>

using namespace std;

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

	unordered_map<string,string> conf;

	while (1) {
		config_file >> line;
		
		if (line == INPUT_START_LINE) {
			stringstream input_config;
			while (1) {
				config_file >> line;
				if (line == INPUT_END_LINE) {
					config_file >> line;
					Config::ParseInput(input_config.str());
					break;
				}
				input_config << line;
			}
		}
		if (line == VAR_START_LINE) {
			stringstream input_config;
			while (1) {
				config_file >> line;
				if (line == VAR_END_LINE) {
					config_file >> line;
					Config::ParseVar(input_config.str());
					break;
				}
				input_config << line;
			}
		}

		if (line == CONSTRAINTS_START_LINE) {
			stringstream input_config;
			while (1) {
				config_file >> line;
				if (line == CONSTRAINTS_END_LINE) {
					config_file >> line;
					Config::ParseConstraints(input_config.str());
					break;
				}
				input_config << line;
			}
		}


		if (config_file.eof()) break;

		vector<string> tokens = split(line, '=');
		if (tokens.size() != 2) {
			cout << "Error while parsing config!" << endl;
			exit(255);
		}

		conf[tokens[0]] = tokens[1];
	}

	string bin_prefix = conf["binary_prefix"];
	string binaries_string = conf["binaries"];

	vector<string> binaries_vector = split(binaries_string, ',');
	
	for(string& binary: binaries_vector) {
		binaries.push_back(bin_prefix + binary);
	}

}

void Config::ParseVar(string input) {
	//TODO add if already parsed this section.

    stringstream inputstream;
    inputstream << input;

    while (!inputstream.eof()) {
        string line;
        inputstream >> line;

        vector<string> tokens = split(line, ':');
        vector<string> varsVector = split(tokens[1], ',');

        //TODO@rols: check if exists
        if (tokens[0] == "int") {
            for(string &token: varsVector) {
                vars[token] =  unique_ptr<Variable>(new VariableInt());
            }
        }
    }
}



void Config::ParseInput(string input) {

	//rootBlock = unique_ptr<BlockComposition>(new BlockComposition());
    shared_ptr<VariableIntConstant> unityVar = make_shared<VariableIntConstant>();
    unityVar->SetValue(1);
    rootBlock->SetRepeteVar(static_pointer_cast<Variable>(unityVar));

	stringstream inputstream;
	inputstream << input;

    cout << input << " hehe" << endl;

	if (!Block::GetNextInputBlock(inputstream, rootBlock, vars)) {
		cout << "Error opening file " << endl;
		exit(255);
	}
}

void Config::ParseConstraints(string input) {

	vector<string> lines = getLines(input);

    //TODO:: handle '=' properly (shouldn't be found now)
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
            cout << "parse errorr!";
            exit(255);
        }
        shared_ptr<Variable> firstVar = vars[firstParam];


        if (vars.find(secondParam) != vars.end()) {

            firstVar->SetUpperLimit(vars[secondParam]);
            continue;
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
        }
        if (foundToken == ">=") {
            tryFirstCast->SetLowerLimit(second);
        }
        if (foundToken == "<") {
            tryFirstCast->SetUpperLimit(--second);
        }
        if (foundToken == ">") {
            tryFirstCast->SetLowerLimit(++second);
        }
    }

}

vector<string> Config::GetBinaries() {
	return binaries;
}

string Config::GetInput() {
    return rootBlock->GetGeneratedText();
}
