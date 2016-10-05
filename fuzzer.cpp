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

		//SetRandomVar(n_min, n_max, n);
		//SetRandomVar(c_min, c_max, c);

		stringstream program_input;

		program_input << t << endl;
		
		program_input << n << " " << c << endl;

		for (int i = 0; i < c; i ++) {
			int b,p,q,v;

			//SetRandomVar(0,1,b);
			//SetRandomVar(1,n,p);
			//SetRandomVar(p,n,q);
			//SetRandomVar(0,10000000,v);

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
