//
// Created by Bojan Roško on 10/2/16.
//

#include "gtest/gtest.h"

#include "../variable.h"

using namespace std;


TEST(VariableIntBasic, VariableTests) {
    VariableInt v;
    v.SetUpperLimit(5);
    v.SetLowerLimit(1);
    v.GenerateValue();
    int val = stoi(v.GetValue());
    EXPECT_LE(1,val);
    EXPECT_LE(val,5);
}

TEST(VariableIntVarLimits, VariableTests) {

    shared_ptr<VariableInt> v = make_shared<VariableInt>();
    shared_ptr<VariableInt> v1 = make_shared<VariableInt>();
    shared_ptr<VariableInt> v2 = make_shared<VariableInt>();
    v1->SetLowerLimit(1);
    v1->SetUpperLimit(10);
    v2->SetLowerLimit(100);
    v2->SetUpperLimit(1000);

    v->SetLowerLimit(static_pointer_cast<Variable>(v1));
    v->SetUpperLimit(static_pointer_cast<Variable>(v2));

    v1->GenerateValue();
    v2->GenerateValue();
    v->GenerateValue();
    int val = stoi(v->GetValue());

    EXPECT_LE(1,val);
    EXPECT_LE(val,1000);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}