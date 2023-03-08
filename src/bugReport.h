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

void bugReport::printMsg(const string& msg) {
    cout << msg << endl;
}

void bugReport::errorReport() const {
    string reportMsg;
    switch (errorFlag) {
        case FILE_INVALID:
            reportMsg = "ERROR: The file extension is illegal, "
                        "please enter a file name ending with .txt . ";
            break;
        case FILE_NONEXIST:
            reportMsg = "ERROR: The file does not exist, "
                        "please check if the file path is correct. ";
            break;
        case FILE_EMPTY:
            reportMsg = "ERROR: The input file is empty and can not be parsed. ";
            break;
        case FILE_MISSING:
            reportMsg = "ERROR: Missing input file path. ";
            break;
        case FILE_FAIL_OUTPUT:
            reportMsg = "ERROR: Fail to output the solution. ";
            break;
        case PARAM_LACK_LETTER:
            reportMsg = "ERROR: Lack of a specified letter after -h, -t or -j.";
            break;
        case PARAM_LACK_OPT:
            reportMsg = "ERROR: Lack of option, please choose one option "
                        "from -n, -w and -c. ";
            break;
        case PARAM_CONFLICT_N:
            reportMsg = "ERROR: -n can not be used in combination with other options. ";
            break;
        case PARAM_CONFLICT_CW:
            reportMsg = "ERROR: -w and -c can not be used in combination. "
                        "Please choose one of them. ";
            break;
        case PARAM_DUPLICATE:
            reportMsg = "ERROR: Duplicate parameters were found. ";
            break;
        case PARAM_INVALID:
            reportMsg = "ERROR: Non-existent parameter option. "
                        "Please check the parameter format. ";
            break;
        case BUG_RING_EXIST:
            reportMsg = "ERROR: The input data contains word rings. ";
            break;
        case BUG_CHAIN_TOO_LONG:
            reportMsg = "ERROR: There are more than 20000 word chains. ";
            break;
        default:
            reportMsg = "Unknown Error.";
            break;
    }
    printMsg(reportMsg);
}



#endif //CMAKETEST_BUGREPORT_H
