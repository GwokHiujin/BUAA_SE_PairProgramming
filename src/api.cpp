#include "include/api.h"
#include "include/paramParser.h"
#include "include/engine.h"
#include "bugReport.h"
#include <ctime>

double timeCost;
char *apiResult[20005];
int apiLen = 0;

extern "C" __declspec(dllexport) int gen_chains_all(string words, int len, char *result[]) {
    rawWords.clear();
    paramParser parser = paramParser();
    for (int i = 0; i < 20005; i++) {
        apiResult[i] = nullptr;
    }

    clock_t t1 = clock();
    parser.parseWords(words);
    try {
        int options[8] = {0};
        options[OP_N] = 1;
        len = engine(options, result);
    } catch (bugReport &e) {
        e.errorReport();
    }
    clock_t t2 = clock();
    timeCost = (double) (t2 - t1) / CLOCKS_PER_SEC;

    for (int i = 0; i < len; i++) {
        apiResult[i] = result[i];
    }
    apiLen = len;
    return len;
}

extern "C" __declspec(dllexport) int
gen_chain_word(string words, int len, char *result[], char head, char tail, char prohibit,
               bool enable_loop) {
    rawWords.clear();
    paramParser parser = paramParser();
    for (int i = 0; i < 20005; i++) {
        apiResult[i] = nullptr;
    }

    clock_t t1 = clock();
    parser.parseWords(words);
    try {
        int options[8] = {0};
        options[OP_W] = 1;
        options[OP_H] = head;
        options[OP_T] = tail;
        options[OP_J] = prohibit;
        options[OP_R] = enable_loop;
        len = engine(options, result);
    } catch (bugReport &e) {
        e.errorReport();
    }
    clock_t t2 = clock();
    timeCost = (double) (t2 - t1) / CLOCKS_PER_SEC;
    for (int i = 0; i < len; i++) {
        apiResult[i] = result[i];
    }
    return len;
}

extern "C" __declspec(dllexport) int
gen_chain_char(string words, int len, char *result[], char head, char tail, char prohibit,
               bool enable_loop) {
    rawWords.clear();
    paramParser parser = paramParser();

    clock_t t1 = clock();
    parser.parseWords(words);
    try {
        int options[8] = {0};
        options[OP_C] = 1;
        options[OP_H] = head;
        options[OP_T] = tail;
        options[OP_J] = prohibit;
        options[OP_R] = enable_loop;
        len = engine(options, result);
    } catch (bugReport &e) {
        e.errorReport();
    }
    clock_t t2 = clock();
    timeCost = (double) (t2 - t1) / CLOCKS_PER_SEC;
    for (int i = 0; i < len; i++) {
        apiResult[i] = result[i];
    }
    return len;
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