//
// Created by Bojan Roško on 10/2/16.
//

#include "block.h"
#include "split.h"

#include <iostream>

bool Block::GetNextInputBlock(
        stringstream& input,
        unique_ptr<BlockComposition>& blockComposition,
        unordered_map<string,shared_ptr<Variable>>& vars) {
    while(input) {
        unique_ptr<Block> block;
        if (GetOneInputBlock(input, block, vars)) {
            blockComposition->AddBlock(block);
        }
        else {
            return false;
        }
    }
    return true;
}

bool Block::GetOneInputBlock(
        stringstream& input,
        unique_ptr<Block>& block,
        unordered_map<string,shared_ptr<Variable>>& vars) {
    char c;
    stringstream token;
    stringstream blockInput;

    bool insideBlock = false;
    bool insideToken = false;

    BlockType blockType;

    while(true) {
        if (!(input >> c)) {
            return false;
        }

        if (insideBlock) {
            if (c == BLOCK_END) {
                if (!ParseInputBlock(token.str(), blockInput, blockType, block, vars)) {
//                    cout << "Error opening file " << endl;
//                    exit(255);
                    return false;
                }
                break;
            }
            blockInput << c;

            continue;
        }

        if (insideToken) {
            if (BLOCK_CHARS.find(c) != BLOCK_CHARS.end() && BLOCK_CHARS[c] == blockType) {

                insideToken = false;
                input >> c;
                if (c == BLOCK_START) {
                    insideBlock = true;
                }
                else {
                    if (!ParseInputBlock(token.str(), blockInput, blockType, block, vars)) {
//                      cout << "Error opening file " << endl;
//                      exit(255);
                        return false;
                    }
                    break;
                }
            }
            else {
                token << c;
            }
            continue;
        }

        if (BLOCK_CHARS.find(c) != BLOCK_CHARS.end()) {
            insideToken = true;
            blockType = (BlockType)BLOCK_CHARS[c];
        }
    }
    return true;
}


bool Block::GetOneCaseBlock(
        stringstream& input,
        unique_ptr<BlockComposition>& block,
        int& value,
        unordered_map<string,shared_ptr<Variable>>& vars) {
    char c;
    stringstream token;
    stringstream blockInput;

    bool insideBlock = false;
    bool insideToken = false;

    BlockType blockType;

    while(true) {
        if (!(input >> c)) {
            return false;
        }

        if (insideBlock) {
            if (c == CONDITION_DELIMITER) {
                if (!GetNextInputBlock(blockInput, block, vars)) {
//                    cout << "Error opening file " << endl;
//                    exit(255);
                    return false;
                }
                value = stoi(token.str());
                break;
            }
            blockInput << c;

            continue;
        }

        if (insideToken) {
            if (c == CONDITION_DELIMITER) {
                insideToken = false;
                insideBlock = true;
            }
            else {
                token << c;
            }
            continue;
        }

        if (c == CONDITION_DELIMITER) {
            insideToken = true;
        }
    }
    return true;
}

bool Block::ParseInputBlock(
        string token,
        stringstream& input,
        BlockType blockType,
        unique_ptr<Block>& block,
        unordered_map<string,shared_ptr<Variable>>& vars) {

    if (vars.find(token) == vars.end()) {
//            cout << "Error parsing config" << endl;
//            exit(255);
        return false;
    }

    if (blockType == BlockType::ECHO) {
        unique_ptr<BlockSimple> blockSimple(new BlockSimple());
        blockSimple->SetEchoVar(vars[token]);
        block = static_unique_pointer_cast<Block,BlockSimple>(move(blockSimple));

        return true;
    }

    if (blockType == BlockType::REPETE) {
        unique_ptr<BlockComposition> blockComposition(new BlockComposition());
        blockComposition->SetRepeteVar(vars[token]);
        if (!Block::GetNextInputBlock(input, blockComposition, vars)) {
            return false;
        }
        block = static_unique_pointer_cast<Block,BlockComposition>(move(blockComposition));

        return true;
    }

    if (blockType == BlockType::CONDITION) {
        unique_ptr<BlockCondition> blockCondition(new BlockCondition());
        blockCondition->SetConditionVar(vars[token]);
        while(input) {
            int caseVal;
            unique_ptr<BlockComposition> blockComposition;
            if (!GetOneCaseBlock(input, blockComposition, caseVal, vars)) {
                return false;
            }
            blockCondition->AddCase(caseVal, blockComposition);
        }
        return true;
    }

    return false;
}

void BlockSimple::SetEchoVar(shared_ptr<Variable> var) {
    echoVar = var;
}

void BlockComposition::SetRepeteVar(shared_ptr<Variable> var) {
    repeteVar = var;
}

void BlockComposition::AddBlock(unique_ptr<Block>& block) {
    composition.push_back(move(block));
}

void BlockCondition::SetConditionVar(shared_ptr<Variable> var) {
    conditionVar = var;
}

void BlockCondition::AddCase(int val, unique_ptr<BlockComposition>& blockComposition) {
    cases[val] = move(blockComposition);
}


string BlockSimple::GetGeneratedText() {
    echoVar->GenerateValue();

    return echoVar->GetValue();
}

string BlockComposition::GetGeneratedText() {

    repeteVar->GenerateValue();

    int repeteNum = stoi(repeteVar->GetValue());

    stringstream outputText;

    //TODO: fix this, output text and spaces
    for (int repeteI = 0; repeteI < repeteNum; repeteI ++) {
        if (repeteI > 0) {
            outputText << " ";
        }
        for (auto& block: composition) {
            outputText << block->GetGeneratedText();
            if (repeteI + 1 < repeteNum) {
                outputText << " ";
            }
        }
    }

    return outputText.str();
}

string BlockCondition::GetGeneratedText() {
    conditionVar->GenerateValue();

    int conditionVal = stoi(conditionVar->GetValue());

    if (cases.find(conditionVal) == cases.end()) {
        cout << "err";
        exit(255);
    }

    return cases[conditionVal]->GetGeneratedText();
}