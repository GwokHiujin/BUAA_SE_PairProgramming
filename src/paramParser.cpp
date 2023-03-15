#include <string>
#include <queue>
#include "include/paramParser.h"
#include <algorithm>
#include <unordered_set>

unordered_set<string> tmpUniqueRawWord;

char paramParser::toLowercase(char c) {
    return ('a' <= c && c <= 'z') ? c : (c - 'A' + 'a');
}

bool paramParser::isSingleLetter(char c) {
    if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
        return true;
    }
    return false;
}

int *paramParser::parseParams(int argc, const char *argv[],
                              int options[8]) {
    string srcFileName;
    // If detected, switch to 1;
    // if letter detected, switch to letter's ASCII
    int letter[127] = {0};
    int param[127] = {0};
    int param_r = 0;
    FILE *srcFile;

    for (int i = 1; i < argc; i++) {
        string curArg = argv[i];
        if (curArg[0] == '-') {
            // option
            switch (curArg[1]) {
                case 'n':
                case 'w':
                case 'c':
                    param[curArg[1]] = 1;
                    break;
                case 'r':
                    param_r = 1;
                    break;
                case 'h':
                case 't':
                case 'j': {
                    letter[curArg[1]] = 1;
                    // Lack of letter:
                    // i+1 reach the end of argv;
                    // nxtArg.length != 1, consider it as a filename

                    string nxtArg = argv[i + 1];

                    // Invalid parameter:
                    // nxtArg is not a single letter
                    letter[curArg[1]] = toLowercase(nxtArg[0]);

                    i++;
                }
                    break;
            }
        } else {
            // Filename
            srcFileName = curArg;
        }
    }
    // Check parameters' combination

    // cout << "read file name: " << srcFileName << endl;

    // Check file name
    string suffixStr = srcFileName.substr(srcFileName.find_last_of('.') + 1);

    const char *path = srcFileName.data();
    srcFile = fopen(path, "r");


    // Read File
    char curChar;
    string curWord;

    while ((curChar = fgetc(srcFile)) && curChar != EOF) {
        if (isSingleLetter(curChar)) {
            curChar = toLowercase(curChar);
            curWord += curChar;
        } else {
            // Divide word
            if (curWord.length() > 0) {
                char *rawWord = (char *) malloc(curWord.length() + 1);
                int k = 0;
                for (k = 0; k < curWord.length(); k++) {
                    rawWord[k] = curWord[k];
                }
                rawWord[k] = 0;
                rawWords.push_back(rawWord);
            }
            curWord.clear();
        }
    }

    // last word
    if (curWord.length() > 0) {
        char *rawWord = (char *) malloc(curWord.length() + 1);
        int k = 0;
        for (k = 0; k < curWord.length(); k++) {
            rawWord[k] = curWord[k];
        }
        rawWord[k] = 0;
        rawWords.push_back(rawWord);
    }
    curWord.clear();

    // Always remember to close the stream
    fclose(srcFile);

    options[0] = param['n'];
    options[1] = param['w'];
    options[2] = param['c'];
    options[3] = letter['h'];
    options[4] = letter['t'];
    options[5] = letter['j'];
    options[6] = param_r;

    uniqueWords();

    return options;
}

void paramParser::uniqueWords() {
    tmpUniqueRawWord.clear();
    for (auto &rawWord: rawWords) {
        if (!tmpUniqueRawWord.count(rawWord)) {
            tmpUniqueRawWord.insert(rawWord);
        }
    }
    rawWords.clear();
    for (auto str: tmpUniqueRawWord) {
        char *rawWord = (char *) malloc(str.size() + 1);
        int k = 0;
        for (k = 0; k < str.size(); k++) {
            rawWord[k] = str[k];
        }
        rawWord[k] = 0;
        rawWords.push_back(rawWord);
    }
}

void paramParser::parseWords(string words) {    // TODO
    string curWord;
    for (char curChar: words) {
        if (isSingleLetter(curChar)) {
            curChar = toLowercase(curChar);
            curWord += curChar;
        } else {
            // Divide word
            if (curWord.length() > 0) {
                char *rawWord = (char *) malloc(curWord.length() + 1);
                int k = 0;
                for (k = 0; k < curWord.length(); k++) {
                    rawWord[k] = curWord[k];
                }
                rawWord[k] = 0;
                rawWords.push_back(rawWord);
            }
            curWord.clear();
        }
    }
    // last word
    if (curWord.length() > 0) {
        char *rawWord = (char *) malloc(curWord.length() + 1);
        int k = 0;
        for (k = 0; k < curWord.length(); k++) {
            rawWord[k] = curWord[k];
        }
        rawWord[k] = 0;
        rawWords.push_back(rawWord);
    }
    curWord.clear();

    uniqueWords();
}