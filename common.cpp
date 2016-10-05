#include "common.h"
#include "block.h"

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
    while( getline( inputstream, line ))  {
        lines.push_back(line);
    }

    return lines;
}

template<typename TO, typename FROM>
unique_ptr<TO> static_unique_pointer_cast (unique_ptr<FROM>&& old){
    return unique_ptr<TO>{static_cast<TO*>(old.release())};
    //conversion: unique_ptr<FROM>->FROM*->TO*->unique_ptr<TO>
}

template unique_ptr<Block> static_unique_pointer_cast<Block, BlockSimple> (unique_ptr<BlockSimple>&&);
template unique_ptr<Block> static_unique_pointer_cast<Block, BlockComposition> (unique_ptr<BlockComposition>&&);
template unique_ptr<Block> static_unique_pointer_cast<Block, BlockCondition> (unique_ptr<BlockCondition>&&);