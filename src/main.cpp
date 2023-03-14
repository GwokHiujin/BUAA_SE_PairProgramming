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

void test_gen_chain_all(const char *words, char *ans[], int ansLen) {
    char **testRes = (char **) malloc(20005);
    int testLen = gen_chains_all(words, 0, testRes);

    for (int i = 0; i < testLen; i++) {
        printf("%s\n", testRes[i]);
    }
    ASSERT_EQ(testLen, ansLen);
    for (int i = 0; i < testLen; i++) {
        ASSERT_EQ(strcmp(ans[i], testRes[i]), 0);
    }
}

void test_gen_chain_word(char *words, char head, char tail, char prohibit,
                         bool enable_loop, char *ans[], int ansLen) {
    char **testRes = (char **) malloc(20005);
    int testLen = gen_chain_word(words, 0, testRes, head, tail, prohibit, enable_loop);
    ASSERT_EQ(testLen, ansLen);
    for (int i = 0; i < ansLen; i++) {
        ASSERT_EQ(strcmp(ans[i], testRes[i]), 0);
    }
}

void test_gen_chain_char(char *words, char head, char tail, char prohibit,
                         bool enable_loop, char *ans[], int ansLen) {
    char **testRes = (char **) malloc(20005);
    int testLen = gen_chain_char(words, 0, testRes, head, tail, prohibit, enable_loop);
    ASSERT_EQ(testLen, ansLen);
    for (int i = 0; i < ansLen; i++) {
        ASSERT_EQ(strcmp(ans[i], testRes[i]), 0);
    }
}


// parse & no result
TEST(Manual, T1) {
    char *words = "ast_bfw_132!4_s!dfg\n";
    char *ans[10] = {};
    int ansLen = 0;
    test_gen_chain_all(words, ans, ansLen);
    get_execution_time();
}

// -w
TEST(Manual, T2) {
    char *words = "ac cdd ab bb be\n";
    char *ans[10] = {"ab", "bb", "be"};
    int ansLen = 3;
    test_gen_chain_word(words, 0, 0, 0, false, ans, ansLen);
    get_execution_time();
}

// -c
TEST(Manual, T3) {
    char *words = "ac cccdd ab bb be\n";
    char *ans[10] = {"ac", "cccdd"};
    int ansLen = 2;
    test_gen_chain_char(words, 0, 0, 0, false, ans, ansLen);
}

// -w -r
TEST(Manual, T4) {
    char *words = "ab bc cd bd da\n";
    char *ans[10] = {"bc", "cd", "da", "ab", "bd"};
    int ansLen = 5;
    test_gen_chain_word(words, 0, 0, 0, true, ans, ansLen);
}


// -c -r -t
TEST(Manual, T5) {
    char *words = "ab bc cd bdddd da\n";
    char *ans[10] = {"bdddd", "da", "ab"};
    int ansLen = 3;
    test_gen_chain_char(words, 0, 'b', 0, true, ans, ansLen);
}

// -w -r -h
TEST(Manual, T6) {
    char *words = "ab bc cd bd da\n";
    char *ans[10] = {"ab", "bc", "cd", "da"};
    int ansLen = 4;
    test_gen_chain_word(words, 'a', 0, 0, true, ans, ansLen);
}

// -c -r -h
TEST(Manual, T7) {
    char *words = "ab bc cd bddddd da\n";
    char *ans[10] = {"ab", "bddddd", "da"};
    int ansLen = 3;
    test_gen_chain_char(words, 'a', 0, 0, true, ans, ansLen);
}

// -w -h -t
TEST(Manual, T8) {
    char *words = "a ac ad d bc cd bd\n";
    char *ans[10] = {"a", "ac", "cd", "d"};
    int ansLen = 4;
    test_gen_chain_word(words, 'a', 'd', 0, true, ans, ansLen);
}

// -c -h -j
TEST(Manual, T9) {
    char *words = "a ac ad d abc cd bd\n";
    char *ans[10] = {};
    int ansLen = 0;
    test_gen_chain_char(words, 'a', 'd', 'a', false, ans, ansLen);
}

// -c -h -j -t
TEST(Manual, T10) {
    char *words = "a ac ad d ab abc cd bd\n";
    char *ans[10] = {"a", "ab", "bd", "d"};
    int ansLen = 4;
    test_gen_chain_char(words, 'a', 'd', 'c', false, ans, ansLen);
}

TEST(Manual, T11) {
    string input = "abcdefg hijklmn opq rst uvw xyz";
    int argc = 3;
    char *argv[10] = {"Wordlist.exe", "-w", "input.txt"};
    char *wordAns[10] = {"abcdefg", "hijklmn", "opq", "rst", "uvw", "xyz"};
    int wordAnsLen = 6;
    int optAns[8] = {0, 1, 0, 'a', 0, 0, 1, 0};
    parseWordUnitTest(input, argc, argv, wordAns, wordAnsLen, optAns);
}

TEST(Manual, T12) {
    char *words = "a ac ad d ab abc cd bd\n";
    char *ans[10] = {"a", "ab", "bd", "d"};
    string ansStr = "a ab bd d";
    int ansLen = 4;
    test_gen_chain_char(words, 'a', 'd', 'c', false, ans, ansLen);

    char *testRes = getResult();
    for (int i = 0; i < testLen; i++) {
        ASSERT_EQ(strcmp(ansStr, testRes[i]), 0);
    }
}

// -n
TEST(Random, T1) {
    int options[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 5; i++) {
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w
TEST(Random, T2) {
    int options[8] = {0, 1, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 5; i++) {
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -h
TEST(Random, T3) {
    for (int i = 0; i < 5; i++) {
        int h = rand() % 26;
        int options[8] = {0, 1, 0, h + 'a', 0, 0, 0, 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -t
TEST(Random, T4) {
    for (int i = 0; i < 5; i++) {
        int t = rand() % 26;
        int options[8] = {0, 1, 0, 0, t + 'a', 0, 0, 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -j
TEST(Random, T5) {
    for (int i = 0; i < 5; i++) {
        int j = rand() % 26;
        int options[8] = {0, 1, 0, 0, 0, j + 'a', 0, 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -h -t
TEST(Random, T6) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int t = rand() % 26;
        int options[8] = {0, 1, 0, h + 'a', t + 'a', 0, 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -h -j
TEST(Random, T7) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 1, 0, h + 'a', 0, j + 'a', 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -t -j
TEST(Random, T8) {
    for (int i = 0; i < 10; i++) {
        int t = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 1, 0, 0, t + 'a', j + 'a', 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -h -t -j
TEST(Random, T9) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int t = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 1, 0, h + 'a', t + 'a', j + 'a', 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -r
TEST(Random, T10) {
    int options[8] = {0, 1, 0, 0, 0, 0, 1, 0};
    for (int i = 0; i < 5; i++) {
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -h -r
TEST(Random, T11) {
    for (int i = 0; i < 5; i++) {
        int h = rand() % 26;
        int options[8] = {0, 1, 0, h + 'a', 0, 0, 1, 0};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -t -r
TEST(Random, T12) {
    for (int i = 0; i < 5; i++) {
        int t = rand() % 26;
        int options[8] = {0, 1, 0, 0, t + 'a', 0, 1, 0};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -j -r
TEST(Random, T13) {
    for (int i = 0; i < 5; i++) {
        int j = rand() % 26;
        int options[8] = {0, 1, 0, 0, 0, j + 'a', 1, 0};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -h -t -r
TEST(Random, T14) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int t = rand() % 26;
        int options[8] = {0, 1, 0, h + 'a', t + 'a', 0, 1};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -h -j -r
TEST(Random, T15) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 1, 0, h + 'a', 0, j + 'a', 1};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -t -j -r
TEST(Random, T16) {
    for (int i = 0; i < 10; i++) {
        int t = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 1, 0, 0, t + 'a', j + 'a', 1};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -w -h -t -j -r
TEST(Random, T17) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int t = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 1, 0, h + 'a', t + 'a', j + 'a', 1};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c
TEST(Random, T18) {
    int options[8] = {0, 0, 1, 0, 0, 0, 0, 0};
    for (int i = 0; i < 5; i++) {
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}


// -c -h
TEST(Random, T19) {
    for (int i = 0; i < 5; i++) {
        int h = rand() % 26;
        int options[8] = {0, 0, 1, h + 'a', 0, 0, 0, 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -t
TEST(Random, T20) {
    for (int i = 0; i < 5; i++) {
        int t = rand() % 26;
        int options[8] = {0, 0, 1, 0, t + 'a', 0, 0, 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -j
TEST(Random, T21) {
    for (int i = 0; i < 5; i++) {
        int j = rand() % 26;
        int options[8] = {0, 0, 1, 0, 0, j + 'a', 0, 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -h -t
TEST(Random, T22) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int t = rand() % 26;
        int options[8] = {0, 0, 1, h + 'a', t + 'a', 0, 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -h -j
TEST(Random, T23) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 0, 1, h + 'a', 0, j + 'a', 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -t -j
TEST(Random, T24) {
    for (int i = 0; i < 10; i++) {
        int t = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 0, 1, 0, t + 'a', j + 'a', 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -h -t -j
TEST(Random, T25) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int t = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 0, 1, h + 'a', t + 'a', j + 'a', 0};
        Sleep(1000);
        randomTestEngine(100, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -r
TEST(Random, T26) {
    int options[8] = {0, 0, 1, 0, 0, 0, 1, 0};
    for (int i = 0; i < 5; i++) {
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -h -r
TEST(Random, T27) {
    for (int i = 0; i < 5; i++) {
        int h = rand() % 26;
        int options[8] = {0, 0, 1, h + 'a', 0, 0, 1, 0};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -t -r
TEST(Random, T28) {
    for (int i = 0; i < 5; i++) {
        int t = rand() % 26;
        int options[8] = {0, 0, 1, 0, t + 'a', 0, 1, 0};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -j -r
TEST(Random, T29) {
    for (int i = 0; i < 5; i++) {
        int j = rand() % 26;
        int options[8] = {0, 0, 1, 0, 0, j + 'a', 1, 0};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -h -t -r
TEST(Random, T30) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int t = rand() % 26;
        int options[8] = {0, 0, 1, h + 'a', t + 'a', 0, 1};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -h -j -r
TEST(Random, T31) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 0, 1, h + 'a', 0, j + 'a', 1};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -t -j -r
TEST(Random, T32) {
    for (int i = 0; i < 10; i++) {
        int t = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 0, 1, 0, t + 'a', j + 'a', 1};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
    }
}

// -c -h -t -j -r
TEST(Random, T33) {
    for (int i = 0; i < 10; i++) {
        int h = rand() % 26;
        int t = rand() % 26;
        int j = rand() % 26;
        int options[8] = {0, 0, 1, h + 'a', t + 'a', j + 'a', 1};
        Sleep(1000);
        randomTestEngine(40, options, "");
        //randomTestPrint();
        randomTestCmp(options);
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