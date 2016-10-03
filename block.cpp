//
// Created by Bojan Roško on 10/2/16.
//

#include "block.h"

#include <iostream>

bool Block::GetNextInputBlock(stringstream& input, unique_ptr<BlockComposition>& blockComposition) {
    while(input) {
        unique_ptr<Block> block;
        if (GetOneInputBlock(input, block)) {
            blockComposition->AddBlock(block);
        }
        else {
            return false;
        }
    }
    return true;
}

bool Block::GetOneInputBlock(stringstream& input, unique_ptr<Block>& block) {
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
                blockType = (BlockType)BLOCK_CHARS[c];
            }
            continue;
        }

        if (BLOCK_CHARS.find(c) != BLOCK_CHARS.end() && BLOCK_CHARS[c] == blockType) {

            if (!ParseInputBlock(token, blockType, block)) {
                cout << "Error opening file " << endl;
                exit(255);
            }
            break;
        }
        token << c;
    }
    return true;
}

bool Block::ParseInputBlock(stringstream& input, BlockType blockType, unique_ptr<Block>& block) {
    return true;
}

void BlockComposition::AddBlock(unique_ptr<Block>& block) {
    composition.push_back(move(block));
}