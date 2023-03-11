#include "include/bugReport.h"

void bugReport::printMsg(const string &msg) {
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
            reportMsg = "ERROR: The input file is empty or doesn't contain valid words. ";
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