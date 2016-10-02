//
// Created by Bojan Roško on 10/2/16.
//

#ifndef FUZZER_BLOCK_H
#define FUZZER_BLOCK_H

#include "variable.h"

#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

enum BlockType {
    ECHO, REPETE, CONDITION
};

//TODO: elegance, please
const static unordered_map<char,int> BLOCK_CHARS = {
        {'$', 0},
        {'*', 1},
        {'@', 2},
};

class block {
public:

    bool GetNextInputBlock(
            stringstream& input,
            unique_ptr<BlockComposition*>& blockComposition
    );

    static bool GetOneInputBlock(
            stringstream& input,
            unique_ptr<block*>& block
    );

    static bool ParseInputBlock(
            stringstream& input,
            BlockType blockType,
            unique_ptr<block*>& block
    );
};

class BlockSimple {

};

class BlockComposition {
public:
    void AddBlock(unique_ptr<block*> block) {
        composition.push_back(block);
    }
private:
    unique_ptr<Variable*> repeteVar;
    vector<unique_ptr<block*> >  composition;
};

class BlockCondition {
private:
    unique_ptr<Variable*> conditionVar;
};



#endif //FUZZER_BLOCK_H
