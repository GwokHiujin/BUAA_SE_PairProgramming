#include "include/engine.h"
#include "include/bugReport.h"
#include "include/paramParser.h"
#include "include/engine.h"
#include <cstring>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>

typedef pair<int, int> PII;
using namespace std;

/* function 'engine' should return a validCode */


priority_queue<PII, vector<PII>, less<PII>> graph[60][60];
int topsortQueue[MAX_V], din[MAX_V];
int wordsLen, vertexNum;
bool vis[MAX_V];
unordered_map<char *, int> rawWordsSet;
queue<int> spfaQueue;
vector<int> dfsVector;
vector<string> resultVector;
vector<string> selfCircle[26];
vector<char *> tmpResult;
unordered_set<string> tmpUniqueResult;

void init() {
    for (auto &i: graph) {
        for (auto &j: i) {
            while (!j.empty()) {
                j.pop();
            }
        }
    }
}

void add(int a, int b, int c, int num) {
    graph[a][b].push({c, num});
}

void topSort() {

}

void markRawWords(int *options) {
    for (int i = 0; i < rawWords.size(); i++) {
        rawWordsSet[rawWords[i]] = i + 52;
    }
}

int getVertexNum(char c, int pos) {   // pos = 0 front; 1 back
    return (c - 'a') * 2 + pos;
}

void buildGraph(int *options) {
    for (auto &rawWord: rawWords) {
        int weight = options[OP_C] ? strlen(rawWord) : 1;
        add(getVertexNum(rawWord[0], 0), getVertexNum(rawWord[strlen(rawWord) - 1], 1),
            weight, rawWordsSet[rawWord]);
    }

    // between words
    for (int i = 0; i < rawWords.size(); i++) {
        for (int j = 0; j < rawWords.size(); j++) {
            if (i == j) {
                continue;
            }

        }
    }
}

int engine(int *options, char *res[]) { // 0-51 aa'-zz' 52-end rawWords
    markRawWords(options);
    init();


}