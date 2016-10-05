#include <iostream>
#include <vector>
#include <sstream>
#include <memory>

using namespace std;


void split(const string &s, char delim, vector<string> &elems);

vector<string> split(const string &s, char delim);

template<typename TO, typename FROM>
unique_ptr<TO> static_unique_pointer_cast (unique_ptr<FROM>&& old);

vector<string> getLines(string input);