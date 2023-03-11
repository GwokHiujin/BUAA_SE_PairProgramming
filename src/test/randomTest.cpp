#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <ctime>
#include <sstream>
#include "../include/engine.h"

using namespace std;
const int randomN = 10005, randomM = 100005;

vector<string> randomWords;
vector<string> randomGlobalAns;
int randE[randomM], randNe[randomM], randH[randomN], randVis[randomN];
int randIdx;

vector<vector<string>> randPaths;
vector<string> singlePath;

unordered_set<string> randomWordsSet;

void createData(int n) {
    randomWords.clear();
    randomWordsSet.clear();
    for (int i = 0; i < n; i++) {
        string str;
        while (true) {
            int wordLen = rand() % 6 + 1; // 1-7
            for (int j = 0; j < wordLen; j++) {
                int s = rand() % 26;
                str += (s + 'a');
            }
            if (!randomWordsSet.count(str)) {
                randomWordsSet.insert(str);
                break;
            }
        }
        randomWords.push_back(str);
    }
}

void add(int a, int b) {
    randE[randIdx] = b;
    randNe[randIdx] = randH[a];
    randH[a] = randIdx++;
}

void dfs(int t) {
    for (int i = randH[t]; ~i; i = randNe[i]) {
        int j = randE[i];
        if (randVis[j]) continue;
        randVis[j] = true;
        singlePath.push_back(randomWords[j]);
        dfs(j);
        singlePath.pop_back();
        randVis[j] = false;
    }
    randPaths.push_back(singlePath);
}

void bruteForce(int n, int *options) {
    int globalMaxAns = 0;
    randomGlobalAns.clear();
    for (int i = 0; i < n; i++) {
        if (options[OP_J] == randomWords[i].front()) {
            continue;
        }
        if (!options[OP_H] || (options[OP_H] == randomWords[i].front())) {
            randPaths.clear();
            dfs(i);

            int maxAns = 0, maxIdx = 0;
            for (int j = 0; j < randPaths.size(); j++) {
                if (randPaths[j].size() < 2) {
                    continue;
                }
                if (options[OP_N]) {
                    string str;
                    for (auto &k: randPaths[j]) {
                        str += k;
                        str += " ";
                    }
                    randomGlobalAns.push_back(str);
                } else {
                    if (options[OP_J]) {
                        for (auto &k: randPaths[j]) {
                            if (options[OP_J] == k.front()) {
                                continue;
                            }
                        }
                    }
                    if (options[OP_T]) {
                        string str = randPaths[j].back();
                        if (options[OP_T] != str.back()) {
                            continue;
                        }
                    }
                    if (options[OP_W]) {
                        if (randPaths[j].size() > maxAns) {
                            maxAns = randPaths[j].size();
                            maxIdx = j;
                        }
                    } else if (options[OP_C]) {
                        int charLen = 0;
                        for (auto &k: randPaths[j]) {
                            charLen += k.size();
                        }
                        if (charLen > maxAns) {
                            maxAns = charLen;
                            maxIdx = j;
                        }
                    }
                }
            }

            if (options[OP_W] || options[OP_C]) {
                if (maxAns > globalMaxAns) {
                    globalMaxAns = maxAns;
                    randomGlobalAns.clear();
                    for (auto &j: randPaths[maxIdx]) {
                        randomGlobalAns.push_back(j);
                    }
                }
            }
        }
    }
}

void randomTestEngine(int n, int *options, string input) {
    srand(time(0));
    memset(randH, -1, sizeof(randH));

    if (input.empty()) {
        createData(n);
    } else {
        stringstream tmpInput;
        tmpInput << input;
        string str;
        while (tmpInput >> str) {
            randomWords.push_back(str);
        }
    }

    for (auto &randomWord: randomWords) {
        printf("%s ", randomWord.c_str());
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (randomWords[i].back() == randomWords[j].front()) {
                if (i == j) continue;
                add(i, j);
            }
        }
    }

    bruteForce(n, options);

    //TODO
    printf("ans: %d\n", randomGlobalAns.size());
    for (auto &randomGlobalAn: randomGlobalAns) {
        printf("%s\n", randomGlobalAn.c_str());
    }
}