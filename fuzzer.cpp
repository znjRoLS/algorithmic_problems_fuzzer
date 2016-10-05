#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "common.h"
#include "binary_runner.h"
#include "config.h"

using namespace std;

int main(int argc, char **argv) { 

	if (argc != 2) {
		cout << "Please run program as fuzzer configfile" << endl;
		return 0;
	}

	//Config config(argv[1]);
	Config config;
	config.OpenFile(argv[1]);

	srand(time(0));
	for(int iter_num = 0; ; iter_num ++) {
		
		if (iter_num % 1000 == 0) cout << "Iteration " << iter_num << endl;

		string program_input = config.GetInput();

		bool first_bin = true;
		string first_output;

		for (string& binary_path: config.GetBinaries()) {

			Binary prog(binary_path);

			//cout << "Running: " << binary_path << endl;
			//cout <<	"With input: " << program_input.str() << endl;

			string output = prog.Run(program_input);
			
			if (first_bin) {
				first_output = output;
				first_bin = false;
			}
			else {
				if (first_output != output) {
					cout << "Input: " << endl;
					cout << program_input << endl;
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
