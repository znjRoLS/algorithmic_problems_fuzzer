#include <iostream>
#include <sstream>

using namespace std;

class Binary {
	public:
		Binary(string dir, string cmd);
		string Run(string input);
	private:
		string dir, cmd;

		void PutInput(string input, int);
		string GetOutput(int);
};
