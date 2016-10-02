//
// Created by Bojan Roško on 10/2/16.
//

#include "block.h"

bool block::GetNextInputBlock(stringstream& input, unique_ptr<BlockComposition*>& blockComposition) {
    while(input) {
        unique_ptr<block*> block;
        if (GetOneInputBlock(input, block)) {
            blockComposition->AddBlock(block);
        }
        else {
            return false;
        }
    }
    return true;
}

bool block::GetOneInputBlock(stringstream& input, unique_ptr<block*>& block) {
    char c;
    stringstream token;

    bool insideBlock = false;

    BlockType blockType;

    while(true) {
        if (!(input >> c)) {
            return false;
        }

        if (!insideBlock) {
            if (BLOCK_CHARS.find(c) != BLOCK_CHARS.end()) {
                insideBlock = true;
                blockType = BLOCK_CHARS[c];
            }
            continue;
        }

        if (BLOCK_CHARS.find(c) != BLOCK_CHARS.end() && BLOCK_CHARS[c] == blockType) {

            if (!ParseInputBlock(token, blockType, block)) {
                cout << "Error opening file " << filename << endl;
                exit(255);
            }
            break;
        }

        token << c;
    }
}

bool block::ParseInputBlock(stringstream& input, BlockType blockType, unique_ptr<block*>& block) {

}