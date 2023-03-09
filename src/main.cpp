#include <fstream>
#include <vector>
#include "bugReport.h"
#include "paramParser.h"
#include "engine.h"
#include "output.h"

using namespace std;
vector<string> result;
vector<char *> rawWords;

int main(int argc, char *argv[]) {
    try {
        paramParser parser = paramParser();
        int options[8] = {0, 1, 0, 0, 0, 0, 0, 0};
        //const char * test[] = {"-n", "input.txt"};
        //parser.parseParams(2, test, options);
        char tmp1[10] = "woo", tmp2[10] = "ooom", tmp3[10] = "moon";
        rawWords.push_back(tmp1);
        rawWords.push_back(tmp2);
        rawWords.push_back(tmp3);
        int ans = engine(options, result);
        output(options, result, ans);
    } catch (bugReport &e) {
        e.errorReport();
    }
}