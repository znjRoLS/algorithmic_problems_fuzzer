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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}