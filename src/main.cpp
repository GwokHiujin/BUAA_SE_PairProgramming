#include "fstream"
#include "vector"
#include "bugReport.h"
#include "paramParser.h"
#include "engine.h"
#include "output.h"

using namespace std;
vector<char*> result;
vector<char*> rawWords;

int main(int argc, char* argv[]) {
    try {
        paramParser parser = paramParser();
        int options[8];
        parser.parseParams(0, (const char**)argv, options);
        int ans = engine(options, result);
        output(options, result, ans);
    } catch (bugReport &e) {
        e.errorReport();
    }
}