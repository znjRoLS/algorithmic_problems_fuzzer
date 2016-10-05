//
// Created by Bojan Roško on 10/3/16.
//

#include "gtest/gtest.h"

#include "../block.h"

#include <memory>

using namespace std;



//TODO: write tests for parsing failures
TEST(BlockSimple, BlockTests) {
    unordered_map<string, shared_ptr<Variable>> vars;

    shared_ptr<VariableIntConstant> var = make_shared<VariableIntConstant>();
    var->SetValue(3);
    vars["x"] = static_pointer_cast<Variable>(var);

    unique_ptr<BlockComposition> rootBlock (new BlockComposition());
    shared_ptr<VariableIntConstant> unityVar = make_shared<VariableIntConstant>();
    unityVar->SetValue(1);
    rootBlock->SetRepeteVar(static_pointer_cast<Variable>(unityVar));

    stringstream input;
    input << "$x$";

    Block::GetNextInputBlock(input, rootBlock, vars);

    var->GenerateValue();

    string temp = rootBlock->GetGeneratedText();

    EXPECT_EQ(temp, "3");
}

TEST(BlockSimpleMore, BlockTests) {
    unordered_map<string, shared_ptr<Variable>> vars;

    shared_ptr<VariableIntConstant> var = make_shared<VariableIntConstant>();
    var->SetValue(3);
    vars["x"] = static_pointer_cast<Variable>(var);

    unique_ptr<BlockComposition> rootBlock (new BlockComposition());
    shared_ptr<VariableIntConstant> unityVar = make_shared<VariableIntConstant>();
    unityVar->SetValue(1);
    rootBlock->SetRepeteVar(static_pointer_cast<Variable>(unityVar));

    stringstream input;
    input << "$x$$x$";

    Block::GetNextInputBlock(input, rootBlock, vars);

    var->GenerateValue();

    string temp = rootBlock->GetGeneratedText();

    EXPECT_EQ(temp, "3 3");
}

TEST(BlockRepeat, BlockTests) {
    unordered_map<string, shared_ptr<Variable>> vars;

    shared_ptr<VariableIntConstant> var = make_shared<VariableIntConstant>();
    var->SetValue(3);
    vars["t"] = static_pointer_cast<Variable>(var);

    shared_ptr<VariableInt> var2 = make_shared<VariableInt>();
    var2->SetLowerLimit(10);
    var2->SetUpperLimit(100);
    vars["x"] = static_pointer_cast<Variable>(var2);

    unique_ptr<BlockComposition> rootBlock (new BlockComposition());
    shared_ptr<VariableIntConstant> unityVar = make_shared<VariableIntConstant>();
    unityVar->SetValue(1);
    rootBlock->SetRepeteVar(static_pointer_cast<Variable>(unityVar));

    stringstream input;
    input << "*t*{$x$}";

    Block::GetNextInputBlock(input, rootBlock, vars);

    var->GenerateValue();

    string temp = rootBlock->GetGeneratedText();

    stringstream tempstream;
    tempstream << temp;

    int test;
    tempstream >> test;
    EXPECT_GE(test, 10);
    EXPECT_LE(test, 100);

    tempstream >> test;
    EXPECT_GE(test, 10);
    EXPECT_LE(test, 100);

    tempstream >> test;
    EXPECT_GE(test, 10);
    EXPECT_LE(test, 100);
}

TEST(BlockCondition, BlockTests) {
    unordered_map<string, shared_ptr<Variable>> vars;

    shared_ptr<VariableIntConstant> var = make_shared<VariableIntConstant>();
    var->SetValue(1);
    vars["t"] = static_pointer_cast<Variable>(var);

    shared_ptr<VariableIntConstant> varx = make_shared<VariableIntConstant>();
    varx->SetValue(11);
    vars["x"] = static_pointer_cast<Variable>(varx);

    shared_ptr<VariableIntConstant> vary = make_shared<VariableIntConstant>();
    vary->SetValue(22);
    vars["y"] = static_pointer_cast<Variable>(vary);

    unique_ptr<BlockComposition> rootBlock (new BlockComposition());
    shared_ptr<VariableIntConstant> unityVar = make_shared<VariableIntConstant>();
    unityVar->SetValue(1);
    rootBlock->SetRepeteVar(static_pointer_cast<Variable>(unityVar));

    stringstream input;
    input << "%t%{@1@$x$@ @2@$y$@}";

    Block::GetNextInputBlock(input, rootBlock, vars);

    var->GenerateValue();
    string temp = rootBlock->GetGeneratedText();
    EXPECT_EQ(temp, "11");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}