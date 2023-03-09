#include <fstream>
#include <vector>
#include "bugReport.h"
#include "paramParser.h"
#include "engine.h"
#include "output.h"

using namespace std;
vector<char *> result;
vector<char *> rawWords;

int main(int argc, char *argv[]) {
    try {
        paramParser parser = paramParser();
        int options[8];
        const char * test[] = {"-n", "input.txt"};
        parser.parseParams(2, test, options);
        int ans = engine(options, result);
        output(options, result, ans);
    } catch (bugReport &e) {
        e.errorReport();
    }
}