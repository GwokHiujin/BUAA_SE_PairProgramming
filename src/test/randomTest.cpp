#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <ctime>
#include <sstream>
#include <cassert>
#include "../include/engine.h"
#include "../include/paramParser.h"

using namespace std;
const int randomN = 10005, randomM = 100005;

vector<string> randomWords;
vector<string> randomGlobalAns;
char *randomResult[20005];
int randE[randomM], randNe[randomM], randH[randomN], randVis[randomN];
int randIdx;

vector<vector<string>> randPaths;
vector<string> singlePath;

unordered_set<string> randomWordsSet;


string charStarToString(char *a) {
    string ans;
    for (int i = 0; i < strlen(a); i++) {
        ans += a[i];
    }
    return ans;
}

char *stringToCharStar(string a) {
    char *ans = (char *) malloc(a.length() + 1);
    int k = 0;
    for (k = 0; k < a.length(); k++) {
        ans[k] = a[k];
    }
    ans[k] = 0;
    return ans;
}

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
    if(singlePath.size() > 1) {
        randPaths.push_back(singlePath);
    }
    for (int i = randH[t]; ~i; i = randNe[i]) {
        int j = randE[i];
        if (randVis[j]) continue;
        randVis[j] = true;
        singlePath.push_back(randomWords[j]);
        dfs(j);
        singlePath.pop_back();
        randVis[j] = false;
    }
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
            singlePath.clear();
            memset(randVis, 0, sizeof(randVis));
            singlePath.push_back(randomWords[i]);
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
}

void randomTestPrint() {
    printf("ans: %d\n", randomGlobalAns.size());
    for (auto &randomGlobalAn: randomGlobalAns) {
        printf("%s\n", randomGlobalAn.c_str());
    }
}

void randomTestCmp(int *options) {
    rawWords.clear();
    for (auto &randomWord: randomWords) {
        char *str = stringToCharStar(randomWord);
        rawWords.push_back(str);
    }

    int ans = engine(options, randomResult);
    if (ans != randomGlobalAns.size()) {
        printf("    %-10s |        %-20s\n", "yours", "ans");
        printf("    %-10d |        %-20d\n", ans, randomGlobalAns.size());


        printf("------------------------------------------------------\n");
        printf("-----------------------ans----------------------------\n");
        printf("ans: %d\n", randomGlobalAns.size());
        for (auto &randomGlobalAn: randomGlobalAns) {
            printf("%s\n", randomGlobalAn.c_str());
        }
        printf("------------------------------------------------------\n");
        printf("-----------------------yours--------------------------\n");
        printf("ans: %d\n", ans);
        for (int i = 0; i<ans; i++) {
            printf("%s\n", randomResult[i]);
        }
    }
    assert(ans == randomGlobalAns.size());

    if (!options[OP_N]) {
        for (int i = 1; i < ans; i++) {
            string str1 = charStarToString(randomResult[i - 1]);
            string str2 = charStarToString(randomResult[i]);
            if (str1.back() != str2.front()) {
                printf("%d: %-20s %-20s\n", i, str1.c_str(), str2.c_str());
            }
            assert(str1.back() == str2.front());
        }
    }

    if (options[OP_C]) {
        int len1 = 0, len2 = 0;
        for (int i = 0; i < ans; i++) {
            len1 += strlen(randomResult[i]);
            len2 += randomGlobalAns[i].size();
        }
        if (len1 != len2) {
            printf("    %-10s |        %-20s\n", "yours", "ans");
            printf("    %-10d |        %-20d\n", len1, len2);
            printf("--------------------------------------------------------\n");

            for (int i = 0; i < ans; i++) {
                printf("%d: %-20s %-20s\n", i, randomResult[i], randomGlobalAns[i].c_str());
            }
        }
        assert(len1 == len2);
    }

    if (options[OP_H]) {
        if (ans) {
            string str = charStarToString(randomResult[0]);
            assert(str.front() == options[OP_H]);
        }
    }

    if (options[OP_T]) {
        if (ans) {
            string str = charStarToString(randomResult[ans - 1]);
            assert(str.back() == options[OP_T]);
        }
    }

    if (options[OP_J]) {
        for (int i = 0; i < ans; i++) {
            string str = charStarToString(randomResult[i]);
            if (str.front() == options[OP_J]) {
                printf("%d: %-20s %-20c\n", i, str.c_str(), options[OP_J]);

            }
            assert(str.front() != options[OP_J]);
        }
    }
}