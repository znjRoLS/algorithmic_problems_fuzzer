#include "split.h"

#include <sstream>

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

vector<string> getLines(string input) {
    stringstream inputstream;
    inputstream << input;

    vector<string> lines;

    string line;
    while(true) {
        inputstream >> line;
        if (!inputstream)
            break;

        lines.push_back(line);
    }

    return lines;
}