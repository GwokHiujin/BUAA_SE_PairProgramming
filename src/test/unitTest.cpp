#include <cstring>
#include <string>
#include <algorithm>
#include <cassert>
#include <unordered_set>
#include "../include/paramParser.h"
#include "../include/engine.h"

using namespace std;
char *testResult[20005];
unordered_set<string> wordsSet;

bool wordCmp(char *a, char *b) {
    if (strlen(a) != strlen(b)) return false;
    for (int i = 0; i < strlen(a); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void parseWordUnitTest(string input, int argc, char *argv[], char *wordAns[], int wordAnsLen, int *optAns) {
    FILE *file = fopen("input.txt", "w");
    fprintf(file, "%s", input.c_str());
    fclose(file);
    paramParser parser = paramParser();
    int options[8];
    parser.parseParams(argc, (const char **) argv, options);
    vector<char *> tmpAns = rawWords;
    vector<char *> wordAnsVector;

    wordsSet.clear();
    for (int i = 0; i < wordAnsLen; i++) {
        wordAnsVector.push_back(wordAns[i]);
        wordsSet.insert(wordAns[i]);
    }

    if (tmpAns.size() != wordAnsVector.size()) {
        printf("    %-10s |        %-20s\n", "yours", "ans");
        printf("    %-10d |        %-20d\n", tmpAns.size(), wordAnsVector.size());
        for (int i = 0; i < max(wordAnsVector.size(), tmpAns.size()); i++) {
            printf("%d: %-20s %-20s\n", i, tmpAns[i], wordAnsVector[i]);
        }
    }
    assert(tmpAns.size() == wordAnsVector.size());

    for (auto &str: tmpAns) {
        if (!wordsSet.count(str)) {
            printf("wrong word %s\n", str);
        }
        assert(wordsSet.count(str));
    }

    for (int i = 0; i < 8; i++) {
        if (options[i] != optAns[i]) {
            printf("%d: %-20d %-20d\n", i, options[i], optAns[i]);
        }
        assert(options[i] == optAns[i]);
    }
}

void engineUnitTest(char *input[], int inputLen, int *options, char *ans[], int ansLen) {
    int engineOutput = engine(options, testResult);
    if(engineOutput != ansLen) {
        printf("%-20d %-20d\n", engineOutput, ansLen);
    }
    assert(engineOutput == ansLen);

    //TODO
    if(options[OP_N]) {
        wordsSet.clear();

    }
    for (int i = 0; i < engineOutput; i++) {
        if(!wordCmp(testResult[i], ans[i])) {
            printf("%d: %-20s %-20s\n", i, testResult[i], ans[i]);
        }
        assert(testResult[i] == ans[i]);
    }
}