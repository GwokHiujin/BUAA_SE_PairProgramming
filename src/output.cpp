#include "output.h"
#include "bugReport.h"
#include "engine.h"
#include <cstdio>

void output(const int *options, const std::vector<string> &result, int ans) {
    if (freopen("solution.txt", "w", stdout) == nullptr) {
        throw bugReport(FILE_FAIL_OUTPUT);
    }

    if (options[OP_N]) {
        cout << ans << endl;
        for (auto &i: result) {
            cout << i << endl;
        }
    } else {
        for (auto &i: result) {
            cout << i << endl;
        }
    }
}