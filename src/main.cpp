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
<<<<<<< HEAD
        vector<char*> rawWords;
        parser.parseParams(0, (const char**)argv, options, rawWords);
        //engine(rawWords, options, result);
=======
        parser.parseParams(0, (const char**)argv, options);
        int ans = engine(options, result);
        output(options, result, ans);
>>>>>>> bf216d5393ca4be274dc18c3658a7ba634c6871d
    } catch (bugReport &e) {
        e.errorReport();
    }
}