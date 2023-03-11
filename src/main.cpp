#include <fstream>
#include <string>
#include <vector>
#include "bugReport.h"
#include "paramParser.h"
#include "engine.h"
#include "output.h"
#include "unitTest.h"

using namespace std;
char *result[20005];
vector<char *> rawWords;

int main(int argc, char *argv[]) {
    try {
        {
            string input = "abcdefg hijklmn opq rst uvw xyz";
            int argc = 6;
            char *argv[10] = {"Wordlist.exe", "-w", "input.txt", "-h", "a", "-r"};
            char *wordAns[10] = {"abcdefg", "hijklmn", "opq", "rst", "uvw", "xyz"};
            int wordAnsLen = 6;
            int optAns[8] = {0, 1, 0, 'a', 0, 0, 1, 0};
            parseWordTest(input, argc, argv, wordAns, wordAnsLen, optAns);
        }

//        paramParser parser = paramParser();
//        int options[8];
//        parser.parseParams(argc, (const char**) argv, options);
//        int ans = engine(options, result);
//        output(options, result, ans);
    } catch (bugReport &e) {
        e.errorReport();
    }
}