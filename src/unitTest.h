#ifndef CMAKETEST_UNITTEST_H
#define CMAKETEST_UNITTEST_H

#include <string>

using namespace std;

void parseWordTest(string input, int argc, char *argv[], char *wordAns[], int wordAnsLen, int *optAns);

void engineTest(char *input[], int inputLen, int *options, char *ans[], int ansLen);


#endif //CMAKETEST_UNITTEST_H
