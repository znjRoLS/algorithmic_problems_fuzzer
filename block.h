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
#include <unordered_set>

using namespace std;

//TODO: parse text between vars properly ( "$x1$,$x2$" )

enum BlockType {
    ECHO, REPEAT, CONDITION
};

//TODO: elegance, please
static unordered_map<char,int> BLOCK_CHARS = {
        {'$', 0},
        {'*', 1},
        {'%', 2},
};

static const char BLOCK_START = '{';
static const char BLOCK_END = '}';
static const char CONDITION_DELIMITER = '@';
// TODO: make this happen
static const char NO_REGENERATION = '!';


class BlockComposition;

class Block {
public:
    virtual BlockType GetType() = 0;

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
            stringstream& textFill,
            unordered_map<string,shared_ptr<Variable>>& vars
    );

    static bool GetOneCaseBlock(
            stringstream& input,
            unique_ptr<BlockComposition>& block,
            int& value,
            bool& lessthan,
            unordered_map<string,shared_ptr<Variable>>& vars
    );

    static bool ParseInputBlock(
            string token,
            stringstream& input,
            BlockType blockType,
            unique_ptr<Block>& block,
            unordered_map<string,shared_ptr<Variable>>& vars
    );

    virtual void GenerateSelfVar() = 0;
};

class BlockSimple: public Block {
public:
    BlockType GetType() override;
    string GetGeneratedText() override;
    void GenerateSelfVar() override;
    void SetEchoVar(shared_ptr<Variable> var);
    shared_ptr<Variable> GetEchoVar();
    void SetRegenerate(bool regenerate);
private:
    shared_ptr<Variable> echoVar;
    bool _regenerate = true;
};

class BlockComposition: public Block {
public:
    BlockType GetType() override;
    string GetGeneratedText() override;
    void VariablesGeneration();
    void GenerateSelfVar() override;
    void AddBlock(unique_ptr<Block>& block);
    void AddTextFill(string textFill);
    void SetRepeatVar(shared_ptr<Variable> var);
    shared_ptr<Variable> GetRepeatVar();
    void SetRegenerate(bool regenerate);
private:
    //TODO: only int variables?
    shared_ptr<Variable> repeatVar;
    vector<unique_ptr<Block> >  composition;
    vector<string> textFills;
    bool _regenerate = true;
};

class BlockCondition: public Block {
public:
    BlockType GetType() override;
    string GetGeneratedText() override;
    void GenerateSelfVar() override;
    void SetConditionVar(shared_ptr<Variable> var);
    shared_ptr<Variable> GetConditionVar();
    void AddCase(int val, bool less_than, unique_ptr<BlockComposition>& blockComposition);
    void AddDefaultCase(unique_ptr<BlockComposition>& blockComposition);
private:
    //TODO: uskladi se, uniqeu, shared...
    shared_ptr<Variable> conditionVar;
    //TODO: make unordered_map<Variable...
    unordered_map<int, unique_ptr<BlockComposition>> cases;
    unordered_set<int> less_than;
    unique_ptr<BlockComposition> default_case;
    //TODO: add true and false cases
    //TODO: add izraze as case condition
};



#endif //FUZZER_BLOCK_H
