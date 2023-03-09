#include "output.h"
#include "bugReport.h"
#include "engine.h"
#include <cstdio>

void output(const int *options, const std::vector<string> &result, int ans) {
    FILE *file = fopen("solution.txt", "w");

    if (file == nullptr) {
        throw bugReport(FILE_FAIL_OUTPUT);
    }

    if (options[OP_N]) {
        fprintf(file, "%d\n", ans);
        int resultLen = result.size();
        for(int i = 0; i<resultLen; i++) {
            fprintf(file, "%s\n", result[i].c_str());
        }
    } else {
        int resultLen = result.size();
        for(int i = 0; i<resultLen; i++) {
            fprintf(file, "%s\n", result[i].c_str());
        }
    }
    fclose(file);
}