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

TEST(ConfigParseComplicated, ConfigTests) {
    Config config;

    TestConfig::ParseVar(config, "int:t,x,x1,x2,y,y1,y2,z,z1,z2,q,cmd");

    //TODO: currently must be hardcoded like this, "$x$ $x$" generates two values, figure this out
    TestConfig::ParseInput(config, "$t$\n"
            "*t*{$x$ $y$ $z$ $q$\n"
            "*q*{%cmd%{\n"
            "    @3@3 $x1$ $y1$ $z1$ $x2$ $y2$ $z2$@\n"
            "    @0@0 $x1$ $x2$@\n"
            "    @1@1 $y1$ $y2$@\n"
            "    @2@2 $z1$ $z2$@\n"
            "    }\n"
            "}\n"
            "}");

    TestConfig::ParseConstraints(config, "t>0\n"
            "t<=10\n"
            "x>0\n"
            "x<100\n"
            "y>0\n"
            "y<100\n"
            "z>0\n"
            "z<100\n"
            "q>0\n"
            "q<5\n"
            "cmd>=0\n"
            "cmd<=3\n"
            "x1>=0\n"
            "x1<x\n"
            "x2>=x1\n"
            "x2<x\n"
            "y1>=0\n"
            "y1<y\n"
            "y2>=y1\n"
            "y2<y\n"
            "z1>=0\n"
            "z1<z\n"
            "z2>=z1\n"
            "z2<z");


    for (int i = 0 ; i < 20; i ++) {

        cout << "output is " << config.GetInput() << endl;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}