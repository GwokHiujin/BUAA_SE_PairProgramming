#ifndef CMAKETEST_API_H
#define CMAKETEST_API_H

extern "C" __declspec(dllexport) int gen_chains_all(const char *words);

extern "C" __declspec(dllexport) int
gen_chain_word(const char *words, char head, char tail, char prohibit,
               bool enable_loop); // most words
extern "C" __declspec(dllexport) int
gen_chain_char(const char *words, char head, char tail, char prohibit,
               bool enable_loop); // most letter

extern "C" __declspec(dllexport) double get_execution_time();

extern "C" __declspec(dllexport) char *getResult();

#endif //CMAKETEST_API_H
