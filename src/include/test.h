#ifndef CMAKETEST_TEST_H
#define CMAKETEST_TEST_H

#include <string>

using namespace std;

void parseWordUnitTest(string input, int argc, char *argv[], char *wordAns[], int wordAnsLen, int *optAns);

void engineUnitTest(char *input[], int inputLen, int *options, char *ans[], int ansLen);

void randomTestEngine(int n, int *options, string input);

#endif //CMAKETEST_TEST_H
