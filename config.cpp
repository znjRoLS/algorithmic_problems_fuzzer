#include "config.h"
#include "split.h"

#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>

using namespace std;


Config::Config(char *filename) {
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
        vector<string> vars = split(tokens[1], ',');

        //TODO@rols: check if exists
        if (tokens[0] == "int") {
            for(string &token: vars) {
                vars[token] =  new VariableInt();
            }
        }
    }
}



void Config::ParseInput(string input) {

	rootBlock = unique_ptr<BlockComposition*>(new BlockComposition());

	stringstream inputstream;
	inputstream << input;

	if (!GetNextInputBlock(inputstream, rootBlock)) {
		cout << "Error opening file " << filename << endl;
		exit(255);
	}
}

void Config::ParseConstraints(string input) {

}

vector<string> Config::GetBinaries() {
	return binaries;
}
