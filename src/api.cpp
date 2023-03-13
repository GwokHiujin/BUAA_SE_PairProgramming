#include "include/api.h"
#include "include/paramParser.h"
#include "include/engine.h"
#include <ctime>

double timeCost;
char *apiResult[20005];

extern "C" __declspec(dllexport) int gen_chains_all(char *words, int len, char *result[]) {
    for (int i = 0; i < 20005; i++) {
        apiResult[i] = nullptr;
    }
    rawWords.clear();
    paramParser parser = paramParser();

    clock_t t1 = clock();
    parser.parseWords(words);

    int options[8] = {0};
    options[OP_N] = 1;
    len = engine(options, result);

    clock_t t2 = clock();
    timeCost = (double) (t2 - t1) / CLOCKS_PER_SEC;
    for (int i = 0; i < len; i++) {
        apiResult[i] = result[i];
    }
    return len;
}

extern "C" __declspec(dllexport) int
gen_chain_word(char *words, int len, char *result[], char head, char tail, char prohibit,
               bool enable_loop) {
    for (int i = 0; i < 20005; i++) {
        apiResult[i] = nullptr;
    }
    rawWords.clear();
    paramParser parser = paramParser();

    clock_t t1 = clock();
    parser.parseWords(words);

    int options[8] = {0};
    options[OP_W] = 1;
    options[OP_H] = head;
    options[OP_T] = tail;
    options[OP_J] = prohibit;
    options[OP_R] = enable_loop;
    len = engine(options, result);

    clock_t t2 = clock();
    timeCost = (double) (t2 - t1) / CLOCKS_PER_SEC;
    for (int i = 0; i < len; i++) {
        apiResult[i] = result[i];
    }
    return len;
}

extern "C" __declspec(dllexport) int
gen_chain_char(char *words, int len, char *result[], char head, char tail, char prohibit,
               bool enable_loop) {
    for (int i = 0; i < 20005; i++) {
        apiResult[i] = nullptr;
    }
    rawWords.clear();
    paramParser parser = paramParser();

    clock_t t1 = clock();
    parser.parseWords(words);

    int options[8] = {0};
    options[OP_C] = 1;
    options[OP_H] = head;
    options[OP_T] = tail;
    options[OP_J] = prohibit;
    options[OP_R] = enable_loop;
    len = engine(options, result);

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

extern "C" __declspec(dllexport) char **get_result() {
    return apiResult;
}