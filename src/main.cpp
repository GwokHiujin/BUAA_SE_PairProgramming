#include "fstream"
#include "vector"
#include "bugReport.h"
#include "paramParser.h"
#include "engine.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        paramParser parser = paramParser();
        int options[8];
        vector<char*> rawWords;
        parser.parseParams(0, (const char**)argv, options, rawWords);
        // engine(options, rawWords);
    } catch (bugReport &e) {
        e.errorReport();
    }
}