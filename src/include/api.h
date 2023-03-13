#ifndef CMAKETEST_API_H
#define CMAKETEST_API_H

extern "C" __declspec(dllexport) int gen_chains_all(char *words, int len, char *result[]);
extern "C" __declspec(dllexport) int
gen_chain_word(char *words, int len, char *result[], char head, char tail, char prohibit,
               bool enable_loop); // most words
extern "C" __declspec(dllexport) int
gen_chain_char(char *words, int len, char *result[], char head, char tail, char prohibit,
               bool enable_loop); // most letter

extern "C" __declspec(dllexport) double get_execution_time();

extern "C" __declspec(dllexport) char **get_result();

#endif //CMAKETEST_API_H
