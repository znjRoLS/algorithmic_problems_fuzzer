#include <iostream>
#include <sstream>

using namespace std;

class Binary {
	public:
		Binary(string cmd);
		string Run(string input, char *args[] = nullptr);
//		string Run(string input, int timelimit);
	private:
		string dir, cmd;

		void PutInput(string input, int);
		string GetOutput(int);
};
