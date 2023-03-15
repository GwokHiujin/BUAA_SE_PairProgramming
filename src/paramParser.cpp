#include <string>
#include <queue>
#include "include/paramParser.h"
#include "include/bugReport.h"
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
            if (curArg.length() != 2) {
                throw bugReport(PARAM_INVALID);
            }
            switch (curArg[1]) {
                case 'n':
                case 'w':
                case 'c': {
                    if (param[curArg[1]]) {
                        throw bugReport(PARAM_DUPLICATE);
                    } else {
                        param[curArg[1]] = 1;
                        if (param['n'] && (param['w'] || param['c'])) {
                            throw bugReport(PARAM_CONFLICT_N);
                        } else if (param['w'] && param['c']) {
                            throw bugReport(PARAM_CONFLICT_CW);
                        }
                    }
                    if (i + 1 == argc) {
                        throw bugReport(FILE_MISSING);
                    }
                    string nxtArg = argv[i + 1];
                    if (nxtArg[0] == '-' && nxtArg.length() == 2) {
                        // consider it as a parameter
                        throw bugReport(FILE_MISSING);
                        i++;
                    }
                }
                    break;
                case 'r':
                    if (param_r) {
                        throw bugReport(PARAM_DUPLICATE);
                    } else {
                        param_r = 1;
                    }
                    break;
                case 'h':
                case 't':
                case 'j': {
                    if (letter[curArg[1]]) {
                        throw bugReport(PARAM_DUPLICATE);
                    } else {
                        letter[curArg[1]] = 1;
                    }
                    // Lack of letter:
                    // i+1 reach the end of argv;
                    // nxtArg.length != 1, consider it as a filename
                    if (i + 1 == argc) {
                        throw bugReport(PARAM_LACK_LETTER);
                    }
                    string nxtArg = argv[i + 1];
                    if (nxtArg.size() != 1) {
                        throw bugReport(PARAM_LACK_LETTER);
                    }
                    // Invalid parameter:
                    // nxtArg is not a single letter
                    if (isSingleLetter(nxtArg[0])) {
                        letter[curArg[1]] = toLowercase(nxtArg[0]);
                    } else {
                        throw bugReport(PARAM_INVALID);
                    }
                    i++;
                }
                    break;
                default:
                    throw bugReport(PARAM_INVALID);
                    break;
            }
        } else {
            // Filename
            if (!srcFileName.empty()) {
                throw bugReport(PARAM_DUPLICATE);
            } else {
                srcFileName = curArg;
            }
        }
    }
    // Check parameters' combination
    if (!param['n'] && !param['w'] && !param['c']) {
        throw bugReport(PARAM_LACK_OPT);
    } else if (param['n'] && (param['w'] & param['c'] &
                              letter['h'] & letter['j'] & letter['t'])) {
        throw bugReport(PARAM_CONFLICT_N);
    } else if (param['w'] && param['c']) {
        throw bugReport(PARAM_CONFLICT_CW);
    }

    // cout << "read file name: " << srcFileName << endl;

    // Check file name
    string suffixStr = srcFileName.substr(srcFileName.find_last_of('.') + 1);
    if (suffixStr != "txt") {
        throw bugReport(FILE_INVALID);
    } else {
        const char *path = srcFileName.data();
        srcFile = fopen(path, "r");
        if (srcFile == nullptr) {
            fclose(srcFile);
            throw bugReport(FILE_NONEXIST);
        }
    }

    // Read File
    char curChar;
    string curWord;
    bool flagBegin = true;

    while ((curChar = fgetc(srcFile)) && curChar != EOF) {
        if (isSingleLetter(curChar)) {
            curChar = toLowercase(curChar);
            curWord += curChar;
            if (flagBegin) {
                flagBegin = false;
            }
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
            flagBegin = true;
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
    flagBegin = true;

    // Always remember to close the stream
    fclose(srcFile);

    if (rawWords.empty()) {
        throw bugReport(FILE_EMPTY);
    }

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

void paramParser::parseWords(string words) {
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