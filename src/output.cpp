#include "include/output.h"
#include "include/engine.h"
#include "gtest/gtest.h"
#include <cstdio>

void output(const int *options, char *result[], int ans) {
    FILE *file = fopen("solution.txt", "w");

    if (options[OP_N]) {
        ASSERT_GE(fprintf(file, "%d\n", ans), 0);
    }
    int resultLen = ans;
    for (int i = 0; i < resultLen; i++) {
        //if (result[i] == nullptr) break; // bad but good enough
        ASSERT_GE(fprintf(file, "%s\n", result[i]), 0);
    }
    fclose(file);
}