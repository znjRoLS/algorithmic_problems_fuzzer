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

//TODO: parse text between vars properly ( "$x1$,$x2$" )

enum BlockType {
    ECHO, REPETE, CONDITION
};

//TODO: elegance, please
static unordered_map<char,int> BLOCK_CHARS = {
        {'$', 0},
        {'*', 1},
        {'@', 2},
};

static const char BLOCK_START = '{';
static const char BLOCK_END = '}';
static const char CONDITION_DELIMITER = '@';

class BlockComposition;

class Block {
public:

    virtual string GetGeneratedText() = 0;

    //TODO: naming
    static bool GetNextInputBlock(
            stringstream& input,
            unique_ptr<BlockComposition>& blockComposition,
            unordered_map<string,shared_ptr<Variable>>& vars
    );

    static bool GetOneInputBlock(
            stringstream& input,
            unique_ptr<Block>& block,
            unordered_map<string,shared_ptr<Variable>>& vars
    );

    static bool GetOneCaseBlock(
            stringstream& input,
            unique_ptr<BlockComposition>& block,
            int& value,
            unordered_map<string,shared_ptr<Variable>>& vars
    );

    static bool ParseInputBlock(
            string token,
            stringstream& input,
            BlockType blockType,
            unique_ptr<Block>& block,
            unordered_map<string,shared_ptr<Variable>>& vars
    );
};

class BlockSimple: public Block {
public:
    string GetGeneratedText() override;
    void SetEchoVar(shared_ptr<Variable> var);
private:
    shared_ptr<Variable> echoVar;
};

class BlockComposition: public Block {
public:
    string GetGeneratedText() override;
    void AddBlock(unique_ptr<Block>& block);
    void SetRepeteVar(shared_ptr<Variable> var);
    int getcompsize() { return composition.size(); }
private:
    //TODO: only int variables?
    shared_ptr<Variable> repeteVar;
    vector<unique_ptr<Block> >  composition;
};

class BlockCondition: public Block {
public:
    string GetGeneratedText() override;
    void SetConditionVar(shared_ptr<Variable> var);
    void AddCase(int val, unique_ptr<BlockComposition>& blockComposition);
private:
    //TODO: uskladi se, uniqeu, shared...
    shared_ptr<Variable> conditionVar;
    //TODO: make unordered_map<Variable...
    unordered_map<int, unique_ptr<BlockComposition>> cases;
    //TODO: add true and false cases
    //TODO: add izraze as case condition
};



#endif //FUZZER_BLOCK_H
