#ifndef CMAKETEST_OUTPUT_H
#define CMAKETEST_OUTPUT_H

#include <vector>

void output(int *options, std::vector<char *> result, int ans) {
    if (freopen("solution.txt", "r", stdout) == NULL) {
        throw bugReport(FILE_FAIL_OUTPUT);
    }
    if (options[OP_N]) {
        cout << ans << endl;
        for (auto & i : result) {
            cout << i << endl;
        }
    } else {
        for (auto & i : result) {
            cout << i << endl;
        }
    }
}

#endif //CMAKETEST_OUTPUT_H
