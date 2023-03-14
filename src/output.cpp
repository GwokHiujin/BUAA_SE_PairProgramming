#include "include/output.h"
#include "include/engine.h"
#include <cstdio>

void output(const int *options, char *result[], int ans) {
    FILE *file = fopen("solution.txt", "w");

    if (options[OP_N]) {
        fprintf(file, "%d\n", ans);
    }
    int resultLen = ans;
    for (int i = 0; i < resultLen; i++) {
        if(result[i] == nullptr) break; // bad but good enough
        fprintf(file, "%s\n", result[i]);
    }
    fclose(file);
}