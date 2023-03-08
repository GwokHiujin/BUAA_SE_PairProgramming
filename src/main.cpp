#include "fstream"
#include "vector"
#include "bugReport.h"
#include "paramParser.h"
#include "engine.h"

using namespace std;
vector<char*> result;

int main(int argc, char* argv[]) {
    try {
        paramParser parser = paramParser();
        int options[8];
        vector<char*> rawWords;
        parser.parseParams(0, (const char**)argv, options, rawWords);
        //engine(rawWords, options, result);
    } catch (bugReport &e) {
        e.errorReport();
    }
}