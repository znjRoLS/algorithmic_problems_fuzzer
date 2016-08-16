#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

//TODO: make template
inline void SetRandomVar(int x_min, int x_max, int &x) {
	x = rand() % (x_max - x_min + 1) + x_min;
}

string RunBinary(const string& binary_name, const string& input) {
	FILE *in;
  char buff[512];
 
	stringstream program_output;

  if(!(in = popen(binary_name.c_str(), "r"))){
      return "";
  }
 
  while(fgets(buff, sizeof(buff), in)!=NULL){
		program_output << buff;
//		cout << buff;
  }
  pclose(in);

	return program_output.str();	
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

	cout << RunBinary("pwd","");

	return 0;
}
