#include <cstring>
#include <string>
#include <algorithm>
#include<cassert>
#include "paramParser.h"
#include "engine.h"

using namespace std;
char *testResult[20005];

void parseWordTest(string input, int argc, char *argv[], char *wordAns[], int wordAnsLen, int *optAns) {
    FILE *file = fopen("input.txt", "w");
    fprintf(file, "%s", input.c_str());
    fclose(file);
    paramParser parser = paramParser();
    int options[8];
    parser.parseParams(argc, (const char **) argv, options);
    vector<char *> tmpAns = rawWords;
    vector<char *> wordAnsVector;

    for (int i = 0; i < wordAnsLen; i++) {
        wordAnsVector.push_back(wordAns[i]);
    }

    sort(tmpAns.begin(), tmpAns.end());
    sort(wordAnsVector.begin(), wordAnsVector.end());
    assert(tmpAns.size() == wordAnsVector.size());
    for (int i = 0; i < tmpAns.size(); i++) {
        assert(tmpAns[i] == wordAnsVector[i]);
    }

    for (int i = 0; i < 8; i++) {
        assert(options[i] == optAns[i]);
    }
}

void engineTest(char *input[], int inputLen, int *options, char *ans[], int ansLen) {
    int engineOutput = engine(options, testResult);
    assert(engineOutput == ansLen);
    for (int i = 0; i < engineOutput; i++) {
        assert(testResult[i] == ans[i]);
    }
}