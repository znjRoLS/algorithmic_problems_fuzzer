//
// Created by Bojan Roško on 10/5/16.
//

#include "gtest/gtest.h"

#include "../config.h"

using namespace std;


class TestConfig {
public:
    static void ParseVar(Config& config, string input)
    { config.ParseVar(input); }
    static void ParseInput(Config& config, string input)
    { config.ParseInput(input); }
    static void ParseConstraints(Config& config, string input)
    { config.ParseConstraints(input); }
};

TEST(ConfigParseConstraints, ConfigTests) {
    Config config;

    TestConfig::ParseVar(config, "int:t,x,v");

    TestConfig::ParseInput(config, "$t$ $x$ $v$");

    TestConfig::ParseConstraints(config, "t>0\nt<=10\nx>5\nx<=6\nv<6\nv>=5");

    for (int i = 0 ; i < 20; i ++) {

        string input = config.GetInput();
        stringstream inputstream;
        inputstream << input;
        int tempt,tempx,tempv;
        inputstream >> tempt >> tempx >> tempv;
        EXPECT_GT(tempt, 0);
        EXPECT_LE(tempt, 10);
        EXPECT_EQ(tempx, 6);
        EXPECT_EQ(tempv, 5);
    }
}

TEST(ConfigParseConditional, ConfigTests) {
    Config config;

    TestConfig::ParseVar(config, "int:t,x,v");

    //TODO: currently must be hardcoded like this, "$x$ $x$" generates two values, figure this out
    TestConfig::ParseInput(config, "$t$-%t%{ @1@2@ @2@3@ } $x$ $v$");

    TestConfig::ParseConstraints(config, "t>0\nt<=2\nx>5\nx<=6\nv<6\nv>=5");

    bool foundIrregular = false;

    for (int i = 0 ; i < 20; i ++) {

        //cout << "output is " << config.GetInput() << endl;
        string temp = config.GetInput();

        size_t found = temp.find("1-3");
        if (found != string::npos) foundIrregular = true;
        found = temp.find("2-2");
        if (found != string::npos) foundIrregular = true;
    }

    EXPECT_FALSE(foundIrregular);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}