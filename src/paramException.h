#ifndef CMAKETEST_PARAMEXCEPTION_H
#define CMAKETEST_PARAMEXCEPTION_H

#include "iostream"

using namespace std;

class paramException {
public:
    paramException(): errorFlag(-1) { };
    explicit paramException(int flag): errorFlag(flag) { };
public:
    void errorReport() const;
private:
    int errorFlag;
    // 1: illegal input file
    // 2: conflicting params
    // 3: illegal params
    // 4: lack of information
    // 5: excess information (?)
};

void paramException::errorReport() const {
    switch (errorFlag) {
        case 1:
            cout << "ERROR: The target input file could not be found! " << endl;
            break;
        case 2:
            cout << "ERROR: You have entered conflicting parameters, please check the specific requirements." << endl;
            break;
        case 3:
            cout << "ERROR: The parameters are invalid. " << endl;
            break;
        case 4:
            cout << "ERROR: Lack of parameters. " << endl;
            break;
        case 5:
            cout << "ERROR: Too many parameters. " << endl;
            break;
        default:
            cout << "Unknown ERROR: Please contact the author. " << endl;
            break;
    }
}

#endif //CMAKETEST_PARAMEXCEPTION_H
