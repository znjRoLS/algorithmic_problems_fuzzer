#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "split.h"
#include "binary_runner.h"

using namespace std;

//TODO: make template
inline void SetRandomVar(int x_min, int x_max, int &x) {
	x = rand() % (x_max - x_min + 1) + x_min;
}

class Config {
	public:
		Config(char *filename);
		vector<string> GetBinaries();
	private:
		void Parse(ifstream& config_file);
		void ParseInput(string input);
		
		vector<string> binaries;

};

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

void Config::ParseInput(string input) {
	
}

vector<string> Config::GetBinaries() {
	return binaries;
}

int main(int argc, char **argv) { 

	if (argc != 2) {
		cout << "Please run program as fuzzer configfile" << endl;
		return 0;
	}

	Config config(argv[1]);

	int t = 1;

	const int n_min = 1;
	const int n_max = 1000;
	int n;
	
	const int c_min = 1;
	const int c_max = 100;
	int c;

	srand(time(0));
	for(int iter_num = 0; ; iter_num ++) {
		
		if (iter_num % 1000 == 0) cout << "Iteration " << iter_num << endl;

		SetRandomVar(n_min, n_max, n);
		SetRandomVar(c_min, c_max, c);

		stringstream program_input;

		program_input << t << endl;
		
		program_input << n << " " << c << endl;

		for (int i = 0; i < c; i ++) {
			int b,p,q,v;

			SetRandomVar(0,1,b);
			SetRandomVar(1,n,p);
			SetRandomVar(p,n,q);
			SetRandomVar(0,10000000,v);

			program_input << b << " " << p << " " << q;
			if (b == 0) program_input << " " << v;
			program_input << endl;
		}

		//string dir = "../spoj_cpp/";
		//string dir = "bin/";
		//string dir = "";
		//string cmd = "horrible";

		bool first_bin = true;
		string first_output;

		for (string& binary_path: config.GetBinaries()) {

			Binary prog(binary_path);

			//cout << "Running: " << binary_path << endl;
			//cout <<	"With input: " << program_input.str() << endl;

			string output = prog.Run(program_input.str());
			
			if (first_bin) {
				first_output = output;
				first_bin = false;
			}
			else {
				if (first_output != output) {
					cout << "Input: " << endl;
					cout << program_input.str() << endl;
					cout << "Iteration: " << iter_num << endl;
					cout << "Output 1: " << endl;
					cout << first_output << endl;
					cout << "Output 2: " << endl;
					cout << output << endl;
					return 0;
				}
			}


			//cout << "Output: " << output << endl;
		}

		

	}
	return 0;
}
