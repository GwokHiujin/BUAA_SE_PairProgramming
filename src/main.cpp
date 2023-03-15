#include <fstream>
#include <string>
#include <vector>
#include <synchapi.h>
#include "include/bugReport.h"
#include "include/paramParser.h"
#include "include/engine.h"
#include "include/output.h"
#include "include/test.h"

using namespace std;
char *result[20005];
vector<char *> rawWords;

int main(int argc, char *argv[]) {
    try {
//        {
//            string input = "abcdefg hijklmn opq rst uvw xyz";
        int argc = 5;
        char *argv[10] = {"Wordlist.exe", "-n", "input.txt", "-w", "input.txt"};
//            char *wordAns[10] = {"abcdefg", "hijklmn", "opq", "rst", "uvw", "xyz"};
//            int wordAnsLen = 6;
//            int optAns[8] = {0, 1, 0, 'a', 0, 0, 1, 0};
//            parseWordUnitTest(input, argc, argv, wordAns, wordAnsLen, optAns);
//        }

//        int options[8] = {0, 0, 1, 0, 0, 0, 1, 0};
//
//        for (int i = 0; i < 20; i++) {
//            Sleep(1000);
//            randomTestEngine(48, options,
//                             "");
//            randomTestPrint();
//            //randomTestCmp(options);
//        }

        paramParser parser = paramParser();
        int options[8];
        parser.parseParams(argc, (const char **) argv, options);
        int ans = engine(options, result);
        output(options, result, ans);
    } catch (bugReport &e) {
        e.errorReport();
    }
}