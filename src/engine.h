#ifndef CMAKETEST_ENGINE_H
#define CMAKETEST_ENGINE_H

#define MAX_V 10005
#define MAX_E 20005

#define OP_N 0
#define OP_W 1
#define OP_C 2
#define OP_H 3
#define OP_J 4
#define OP_T 5
#define OP_R 6

int engine(const std::vector<char *> &rawWords, int *options, std::vector<char *> &res);

#endif //CMAKETEST_ENGINE_H
