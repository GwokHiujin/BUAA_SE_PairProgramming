#ifndef CMAKETEST_PARAMPARSER_H
#define CMAKETEST_PARAMPARSER_H

#include <vector>
#include <string>

using namespace std;

extern std::vector<char*> rawWords;

class paramParser {
    /* Parse the parameters, return a bit vector contains
     * information about handle option */
public:
    paramParser() = default;
    ~paramParser() = default;
private:
    static inline char toLowercase(char c) ;
    static inline bool isSingleLetter(char c) ;
public:
    int * parseParams(int argc, const char* argv[],
                      int options[8]);
    void parseWords(string words);
};

#endif //CMAKETEST_PARAMPARSER_H
