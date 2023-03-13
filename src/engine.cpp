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


vector<PII> graph[60][60];
int topsortQueue[MAX_V], din[MAX_V], dist[MAX_E], topSortPath[MAX_E];
int maxRingSize;
bool vis[MAX_E];
unordered_map<char *, int> rawWordsSet;
vector<int> path;
vector<int> dfsVector;
vector<string> resultVector;
vector<PII> selfCircle[26];

void init(int *options) {
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            graph[i][j].clear();
        }
        selfCircle[i].clear();
    }
    rawWordsSet.clear();
    resultVector.clear();
    if(options[OP_T]) {

    }
    memset(dist, -1, sizeof(dist));
    memset(topSortPath, -1, sizeof(topSortPath));
}

void add(int a, int b, int c, int num) {
    graph[a][b].push_back({c, num});
}

char *stringToCharPointer(string a) {
    char *ans = (char *) malloc(a.length() + 1);
    int k = 0;
    for (k = 0; k < a.length(); k++) {
        ans[k] = a[k];
    }
    ans[k] = 0;
    return ans;
}

void markRawWords() {
    for (int i = 0; i < rawWords.size(); i++) {
        rawWordsSet[rawWords[i]] = i;
    }
}

void buildGraph(int *options) {
    for (auto &rawWord: rawWords) {
        if (options[OP_J] == rawWord[0]) {
            continue;
        }
        int weight = options[OP_C] ? strlen(rawWord) : 1;
        add(rawWord[0] - 'a', rawWord[strlen(rawWord) - 1] - 'a', weight, rawWordsSet[rawWord]);
        //printf("%c %c %d %d\n", rawWord[0], rawWord[strlen(rawWord) - 1], weight, rawWordsSet[rawWord]);
        din[rawWord[strlen(rawWord) - 1] - 'a']++;
    }
}

bool checkSelfCircle(int *options) {
    int cnt = 0;
    for (int i = 0; i < 26; i++) {
        if (graph[i][i].size() >= 2) {
            cnt++;
        }
        int nodeWeightofSelfCircle = 0;
        for (auto & j : graph[i][i]) {
            nodeWeightofSelfCircle += j.first;
        }
        dist[i] += nodeWeightofSelfCircle;
    }
    return cnt < 1;
}

void deleteSelfCircle() {
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < graph[i][i].size(); j++) {
            selfCircle[i].push_back(graph[i][i][j]); // the idx in rawWords
            din[i]--;
        }
        graph[i][i].clear();
    }
}

bool topSort(int *options) {
    deleteSelfCircle();
    int tt = -1, hh = 0;
    for (int i = 0; i < 26; i++) {
        if (din[i] == 0) {
            topsortQueue[++tt] = i;
        }
    }

    while (hh <= tt) {
        int t = topsortQueue[hh++]; // get front and pop
        for (int i = 0; i < 26; i++) {
            for (int j = 0; j < graph[t][i].size(); j++) {
                if (dist[t] + graph[t][i][j].first > dist[i]) {
                    dist[i] = dist[t] + graph[t][i][j].first;
                    topSortPath[i] = t;
                }
                if ((--din[i]) == 0) {
                    topsortQueue[++tt] = i;
                }
            }
        }
    }
    return tt == 25;
}

void rebuildGraph(int *options) {
    if (options[OP_R] || options[OP_N]) {
        for (int i = 0; i < 26; i++) {
            for (int j = 0; j < selfCircle[i].size(); j++) {
                graph[i][i].push_back(selfCircle[i][j]);
            }
        }
    } else {    // useless part
        for (int i = 0; i < 26; i++) {
            for (int j = 0; j < 26; j++) {
                if (i == j) {
                    continue;
                }
                int maxLen = 0;
                PII maxIdx;
                for (auto t: graph[i][j]) {
                    if (maxLen < t.first) {
                        maxLen = t.first;
                        maxIdx = t;
                    }
                }
                graph[i][j].clear();
                graph[i][j].push_back(maxIdx);
            }
        }
    }
}

void dfsAllLinks(int s) {
    if (resultVector.size() > 20000) {
        return;
    }
    if (path.size() >= 2) {
        string str;
        for (int i: path) {
            str += rawWords[i];
            str += " ";
        }
        resultVector.push_back(str);
    }
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < graph[s][i].size(); j++) {
            PII t = graph[s][i][j];
            if (!vis[t.second]) {
                vis[t.second] = true;
                path.push_back(t.second);
                dfsAllLinks(i);
                vis[t.second] = false;
                path.pop_back();
            }
        }
    }
}

void getAllLinks() {
    for (int i = 0; i < 26; i++) {
        path.clear();
        memset(vis, 0, sizeof(vis));
        dfsAllLinks(i);
    }
}

void dfsMaxRing(int s, int *options) {  // todo: optimize
    if (path.size() >= 2) {
        string str = rawWords[path.back()];
        if (!options[OP_T] || options[OP_T] == str.back()) {
            int tmpLen = 0;
            for (int i: path) {
                tmpLen += (options[OP_W] ? 1 : strlen(rawWords[i]));
            }
            if (tmpLen > maxRingSize) {
                maxRingSize = tmpLen;
                resultVector.clear();
                for (int i: path) {
                    resultVector.push_back(rawWords[i]);
                }
            }
        }
    }
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < graph[s][i].size(); j++) {
            PII t = graph[s][i][j];
            if (!vis[t.second]) {
                vis[t.second] = true;
                path.push_back(t.second);
                dfsMaxRing(i, options);
                vis[t.second] = false;
                path.pop_back();
            }
        }
    }
}

void getMaxRing(int *options) {
    if (options[OP_H]) {
        path.clear();
        maxRingSize = 0;
        memset(vis, 0, sizeof(vis));
        dfsMaxRing(options[OP_H] - 'a', options);
    } else {
        maxRingSize = 0;
        for (int i = 0; i < 26; i++) {
            path.clear();
            memset(vis, 0, sizeof(vis));
            dfsMaxRing(i, options);
        }
    }
}

int engine(int *options, char *res[]) { // 0-25 a-z 26-end rawWords
    init();
    markRawWords();

    buildGraph(options);

    if (!options[OP_R]) {
        if (!checkSelfCircle()) {
            throw bugReport(BUG_RING_EXIST);
        }
        if (!topSort(options)) {
            throw bugReport(BUG_RING_EXIST);
        }
    }

    rebuildGraph(options);

    if (options[OP_N]) {
        getAllLinks();
    } else if (options[OP_R]) {
        getMaxRing(options);
    } else {

    }

    if (resultVector.size() > 20000) {
        throw bugReport(BUG_CHAIN_TOO_LONG);
    }
    for (int i = 0; i < resultVector.size(); i++) {
        res[i] = stringToCharPointer(resultVector[i]);
    }

    return resultVector.size();
}