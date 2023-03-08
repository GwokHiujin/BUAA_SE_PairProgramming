#ifndef CMAKETEST_PARAMPARSER_H
#define CMAKETEST_PARAMPARSER_H

#include <vector>

using namespace std;
extern std::vector<char*> rawWords;

class paramParser {
    /* Parse the parameters, return a bit vector contains
     * information about handle option */
public:
    paramParser();
private:
    static inline char toLowercase(char c) ;
    static inline bool isSingleLetter(char c) ;
public:
    int * parseParams(int argc, const char* argv[],
                      int options[8]);
};


#endif //CMAKETEST_PARAMPARSER_H
