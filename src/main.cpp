#include "iostream"
#include "fstream"
#include "vector"
#include "stdexcept"

#include "paramException.h"
#include "paramParser.h"
#include "engine.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        /* Read file and pass parameters to Engine */
        /* Parameters:
         * vector<const char*>: rawWords
         * int: A bit vector contains following info:
         *  int: headLetterLimit (-1 or +/-ASCII: +ASCII means set, -ASCII means unset)
         *  int: tailLetterLimit (-1 or +/-ASCII: +ASCII means set, -ASCII means unset)
         *  bool: allowRing (true or false)
         *  int: option (-1: Total Number; 0: Max Words Number; 1: Max Letter Number)
         * */
        // TODO:
        //  parser = paramParser(argv[])
        //  int option = parser::parseOption,
        //  vector<char*> rawWords = parser::parseWords
        //  ret = engine(rawWords, option, result)
        //  write `result` to output_file or reportInvalidRet
    } catch (paramException &e) {
        e.errorReport();
    }
}