#ifndef CMAKETEST_BUGREPORT_H
#define CMAKETEST_BUGREPORT_H

#include "iostream"

using namespace std;

#define FILE_INVALID 1
#define FILE_NONEXIST 2
#define FILE_EMPTY 3
#define FILE_MISSING 4
#define FILE_FAIL_OUTPUT 5
#define PARAM_LACK_LETTER 6
#define PARAM_LACK_OPT 7
#define PARAM_CONFLICT_N 8
#define PARAM_CONFLICT_CW 9
#define PARAM_DUPLICATE 10
#define PARAM_INVALID 11
#define BUG_RING_EXIST 12
#define BUG_CHAIN_TOO_LONG 13

class bugReport {
public:
    bugReport(): errorFlag(-1) { };
    explicit bugReport(int flag): errorFlag(flag) { };
    ~bugReport() = default;
public:
    void errorReport() const;
private:
    int errorFlag;
    static void printMsg(const string&) ;
};



#endif //CMAKETEST_BUGREPORT_H
