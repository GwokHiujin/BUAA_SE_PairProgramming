#include <fstream>
#include <string>
#include <vector>
#include <synchapi.h>
#include <unordered_set>
#include "include/bugReport.h"
#include "include/paramParser.h"
#include "include/engine.h"
#include "include/output.h"
#include "include/test.h"
#include "api.h"
#include "gtest/gtest.h"

using namespace std;
char *result[20005];
vector<char *> rawWords;

TEST(FILE_BUG, FILE_INVALID) {
    try {
        paramParser parser = paramParser();
        int argc = 3;
        char *argv[10] = {"Wordlist.exe", "-w", "input.txtt"};
        int options[8];
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * FILE_INVALID);
    }
}

TEST(FILE_BUG, FILE_NONEXIST) {
    try {
        paramParser parser = paramParser();
        int argc = 3;
        char *argv[10] = {"Wordlist.exe", "-n", "noneExist.txt"};
        int options[8];
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * FILE_NONEXIST);
    }
}

TEST(FILE_BUG, FILE_EMPTY) {
    try {
        paramParser parser = paramParser();
        int argc = 3;
        char *argv[10] = {"Wordlist.exe", "-n", "empty.txt"};
        int options[8];
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * FILE_EMPTY);
    }
}

TEST(FILE_BUG, FILE_MISSING) {
    try {
        paramParser parser = paramParser();
        int argc = 2;
        char *argv[10] = {"Wordlist.exe", "-n"};
        int options[8];
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * FILE_MISSING);
    }
}

TEST(FILE_BUG, FILE_FAIL_OUTPUT) {
    try {
        int options[8] = {1, 0, 0, 0, 0, 0, 0, 0};
        char *result[10] = {"output"};
        output(options, result, 1);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * FILE_FAIL_OUTPUT);
    }
}

TEST(PARAM_BUG, PARAM_LACK_LETTER) {
    try {
        paramParser parser = paramParser();
        int options[8];
        int argc = 4;
        char *argv[10] = {"Wordlist.exe", "-w", "input.txt", "-h"};
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * PARAM_LACK_LETTER);
    }
}

TEST(PARAM_BUG, PARAM_LACK_OPT) {
    try {
        paramParser parser = paramParser();
        int options[8];
        int argc = 3;
        char *argv[10] = {"Wordlist.exe", "-h", "a"};
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * PARAM_LACK_OPT);
    }
}

TEST(PARAM_BUG, PARAM_CONFLICT_N) {
    try {
        paramParser parser = paramParser();
        int options[8];
        int argc = 5;
        char *argv[10] = {"Wordlist.exe", "-n", "input.txt", "-h", "a"};
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * PARAM_CONFLICT_N);
    }
}

TEST(PARAM_BUG, PARAM_CONFLICT_CW) {
    try {
        paramParser parser = paramParser();
        int options[8];
        int argc = 5;
        char *argv[10] = {"Wordlist.exe", "-c", "input.txt", "-w", "input.txt"};
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * PARAM_CONFLICT_CW);
    }
}

TEST(PARAM_BUG, PARAM_DUPLICATE) {
    try {
        paramParser parser = paramParser();
        int options[8];
        int argc = 5;
        char *argv[10] = {"Wordlist.exe", "-c", "input.txt", "-c", "input.txt"};
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * PARAM_DUPLICATE);
    }
}

TEST(PARAM_BUG, PARAM_INVALID) {
    try {
        paramParser parser = paramParser();
        int options[8];
        int argc = 3;
        char *argv[10] = {"Wordlist.exe", "-x", "input.txt"};
        parser.parseParams(argc, (const char **) argv, options);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * PARAM_INVALID);
    }
}

TEST(RING_BUG, BUG_RING_EXIST) {
    try {
        int options[8] = {1, 0, 0, 0, 0, 0, 0, 0};
        int ans = engine(options, result);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * BUG_RING_EXIST);
    }
}

TEST(CHAIN_BUG, BUG_CHAIN_TOO_LONG) {
    try {
        paramParser parser = paramParser();
        int options[8];
        int argc = 3;
        char *argv[10] = {"Wordlist.exe", "-n", "input.txt"};
        parser.parseParams(argc, (const char **) argv, options);
        int ans = engine(options, result);
        output(options, result, ans);
        ASSERT_EQ(0, 1);
    } catch (bugReport &e) {
        ASSERT_EQ(e.getErrorFlag(), -1 * BUG_CHAIN_TOO_LONG);
    }
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
//    try {
////        {
////            string input = "abcdefg hijklmn opq rst uvw xyz";
//        int argc = 3;
//        char *argv[10] = {"Wordlist.exe", "-w", "input.txt"};
////            char *wordAns[10] = {"abcdefg", "hijklmn", "opq", "rst", "uvw", "xyz"};
////            int wordAnsLen = 6;
////            int optAns[8] = {0, 1, 0, 'a', 0, 0, 1, 0};
////            parseWordUnitTest(input, argc, argv, wordAns, wordAnsLen, optAns);
////        }
//
////        int options[8] = {0, 0, 1, 0, 0, 0, 1, 0};
////
////        for (int i = 0; i < 20; i++) {
////            Sleep(1000);
////            randomTestEngine(48, options,
////                             "");
////            randomTestPrint();
////            //randomTestCmp(options);
////        }
//
//        paramParser parser = paramParser();
//        int options[8];
//        parser.parseParams(argc, (const char **) argv, options);
//        int ans = engine(options, result);
//        output(options, result, ans);
//    } catch (bugReport &e) {
//        e.errorReport();
//    }
}