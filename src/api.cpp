#include "include/api.h"
#include "include/paramParser.h"
#include "include/engine.h"
#include "include/bugReport.h"
#include <ctime>

double timeCost;
char *apiResult[20005];
int apiLen = 0;

extern "C" __declspec(dllexport) int gen_chains_all(const char *words) {
    apiLen = 0;
    rawWords.clear();
    for (auto &i: apiResult) {
        i = nullptr;
    }
    try {
        paramParser parser = paramParser();

        clock_t t1 = clock();
        parser.parseWords(words);
        int options[8] = {0};
        options[OP_N] = 1;
        apiLen = engine(options, apiResult);
        clock_t t2 = clock();
        timeCost = (double) (t2 - t1) / CLOCKS_PER_SEC;
    } catch (bugReport &e) {
        e.errorReport();
    }

    return apiLen;
}

extern "C" __declspec(dllexport) int
gen_chain_word(const char *words, char head, char tail, char prohibit, bool enable_loop) {
    apiLen = 0;
    rawWords.clear();
    for (auto &i: apiResult) {
        i = nullptr;
    }

    try {
        paramParser parser = paramParser();

        clock_t t1 = clock();
        parser.parseWords(words);
        int options[8] = {0};
        options[OP_W] = 1;
        options[OP_H] = head;
        options[OP_T] = tail;
        options[OP_J] = prohibit;
        options[OP_R] = enable_loop;
        apiLen = engine(options, apiResult);
        clock_t t2 = clock();
        timeCost = (double) (t2 - t1) / CLOCKS_PER_SEC;
    } catch (bugReport &e) {
        e.errorReport();
    }

    return apiLen;
}

extern "C" __declspec(dllexport) int
gen_chain_char(const char *words, char head, char tail, char prohibit, bool enable_loop) {
    apiLen = 0;
    rawWords.clear();
    for (auto &i: apiResult) {
        i = nullptr;
    }

    try {
        paramParser parser = paramParser();
        clock_t t1 = clock();
        parser.parseWords(words);
        int options[8] = {0};
        options[OP_C] = 1;
        options[OP_H] = head;
        options[OP_T] = tail;
        options[OP_J] = prohibit;
        options[OP_R] = enable_loop;
        apiLen = engine(options, apiResult);

        clock_t t2 = clock();
        timeCost = (double) (t2 - t1) / CLOCKS_PER_SEC;
    } catch (bugReport &e) {
        e.errorReport();
    }

    return apiLen;
}

extern "C" __declspec(dllexport) double get_execution_time() {
    return timeCost;
}

char *stringToCharStarTmp(string a) {
    char *ans = (char *) malloc(a.length() + 1);
    int k = 0;
    for (k = 0; k < a.length(); k++) {
        ans[k] = a[k];
    }
    ans[k] = 0;
    return ans;
}

extern "C" __declspec(dllexport) char *getResult() {
    string str;
    for (int i = 0; i < apiLen; i++) {
        str += apiResult[i];
        str += '\n';
    }
    return stringToCharStarTmp(str);
}