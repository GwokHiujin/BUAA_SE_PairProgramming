#include "api.h"
#include "paramParser.h"
#include "engine.h"

int gen_chains_all(string words, int len, char *result[]) {
    rawWords.clear();
    paramParser parser = paramParser();
    parser.parseWords(words);

    int options[8] = {0};
    options[OP_N] = 1;
    len = engine(options, result);

    return len;
}

int gen_chain_word(string words, int len, char *result[], char head, char tail, char prohibit,
                   bool enable_loop) {
    rawWords.clear();
    paramParser parser = paramParser();
    parser.parseWords(words);

    int options[8] = {0};
    options[OP_W] = 1;
    options[OP_H] = head;
    options[OP_T] = tail;
    options[OP_J] = prohibit;
    options[OP_R] = enable_loop;
    len = engine(options, result);

    return len;
}

int gen_chain_char(string words, int len, char *result[], char head, char tail, char prohibit,
                   bool enable_loop) {
    rawWords.clear();
    paramParser parser = paramParser();
    parser.parseWords(words);

    int options[8] = {0};
    options[OP_C] = 1;
    options[OP_H] = head;
    options[OP_T] = tail;
    options[OP_J] = prohibit;
    options[OP_R] = enable_loop;
    len = engine(options, result);

    return len;
}