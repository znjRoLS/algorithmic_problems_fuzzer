#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include "binary_runner.h"

using namespace std;

//TODO: make template
inline void SetRandomVar(int x_min, int x_max, int &x) {
	x = rand() % (x_max - x_min + 1) + x_min;
}

int main() { 

	int t = 1;

	const int n_min = 1;
	const int n_max = 100;
	int n;
	
	const int c_min = 1;
	const int c_max = 100;
	int c;

	srand(time(0));

	SetRandomVar(n_min, n_max, n);
	SetRandomVar(c_min, c_max, c);

	stringstream program_input;

	program_input << t << endl;
	
	program_input << n << " " << c << endl;

	for (int i = 0; i < c; i ++) {
		int b,p,q,v;

		SetRandomVar(0,1,b);
		SetRandomVar(0,n-1,p);
		SetRandomVar(p,n-1,q);
		SetRandomVar(0,1000000,v);

		program_input << b << " " << p << " " << q;
		if (b == 0) program_input << " " << v;
		program_input << endl;
	}

	string dir = "../spoj_cpp/";
	//string dir = "bin/";
	//string dir = "";
	string cmd = "horrible";

	Binary prog(dir, cmd);

	cout << "Running: " << dir << cmd << endl;
	cout <<	"With input: " << program_input.str() << endl;

	string output = prog.Run(program_input.str());
	cout << "Output: " << output << endl;

	return 0;
}
