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
int maxRingSize, maxPathSize;
bool vis[MAX_E];
unordered_map<char *, int> rawWordsSet;
vector<int> path;
vector<string> resultVector;
vector<PII> selfCircle[26];

void init() {
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            graph[i][j].clear();
        }
        selfCircle[i].clear();
    }
    rawWordsSet.clear();
    resultVector.clear();
    memset(din, 0, sizeof(din));
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

bool checkSelfCircle() {
    int cnt = 0;
    for (int i = 0; i < 26; i++) {
        if (graph[i][i].size() >= 2) {
            cnt++;
        }
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

bool topSort() {
    deleteSelfCircle();
    int tt = -1, hh = 0;
    for (int i = 0; i < 52; i++) {
        if (din[i] == 0) {
            topsortQueue[++tt] = i;
        }
    }

    while (hh <= tt) {
        int t = topsortQueue[hh++]; // get front and pop
        for (int i = 0; i < 52; i++) {
            for (int j = 0; j < graph[t][i].size(); j++) {
                if ((--din[i]) == 0) {
                    topsortQueue[++tt] = i;
                }
            }
        }
    }
    return tt == 51;
}

void rebuildGraph(int *options) {
    if (options[OP_N] || options[OP_R]) {
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
                if (maxLen != 0) {
                    graph[i][j].clear();
                    graph[i][j].push_back(maxIdx);
                }
            }
        }

        for (int i = 0; i < 26; i++) {
            if (!selfCircle[i].empty()) {
                graph[i][i + 26].push_back(selfCircle[i].front());
                for (int j = 0; j < 26; j++) {  // i --> j => i --> i+26 --> j
                    if (i == j) continue;
                    if (!graph[i][j].empty()) {
                        graph[i + 26][j].push_back(graph[i][j].front());
                        graph[i][j].clear();
                    }
                }
            }
        }

        memset(din, 0, sizeof(din));
        for (int i = 0; i < 52; i++) {
            for (int j = 0; j < 52; j++) {
                if (!graph[i][j].empty()) {
                    din[j]++;
                }
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

void dp(int *options) {
    memset(topsortQueue, 0, sizeof(topsortQueue));
    memset(topSortPath, -1, sizeof(topSortPath));
    if (options[OP_T]) {
        memset(dist, -0x3f, sizeof(dist));
        for (int i = 0; i < 52; i++) {
            if (i == options[OP_T] - 'a' || i + 26 == options[OP_T] - 'a') {
                dist[i] = 0;
            }
        }
    } else {
        memset(dist, 0, sizeof(dist));
        topSort();

        for (int i = 51; i >= 0; i--) {
            for (int j = 0; j < 52; j++) {  // j --> i
                int tmpV = topsortQueue[i];
                if (tmpV == j) continue;
                if (graph[j][tmpV].empty()) continue;
                if (dist[j] < dist[tmpV] + graph[j][tmpV].front().first) {
                    dist[j] = dist[tmpV] + graph[j][tmpV].front().first;
                    topSortPath[j] = tmpV;
                }
            }
        }

        // link len >= 2
        if (options[OP_H]) {
            // find the first path
            int st = options[OP_H] - 'a';
            int maxLen = 0, maxIdx = 0;
            for (int i = 0; i < 52; i++) {
                if (!graph[st][i].empty()) {
                    if (!dist[i]) continue;
                    if (dist[i] + graph[st][i].front().first > maxLen) {
                        maxLen = dist[i] + graph[st][i].front().first;
                        maxIdx = i;
                    }
                }
            }
            if (maxLen) {
                path.push_back(st);
                int t = maxIdx;
                while (t != -1) {
                    path.push_back(t);
                    t = topSortPath[t];
                }
            }
        } else {
            int maxLen = 0, maxIdxi = 0, maxIdxj = 0;
            for (int i = 0; i < 52; i++) {
                for (int j = 0; j < 52; j++) {
                    if (!graph[i][j].empty()) {
                        if (!dist[j]) continue;
                        if (dist[j] + graph[i][j].front().first > maxLen) {
                            maxLen = dist[j] + graph[i][j].front().first;
                            maxIdxi = i;
                            maxIdxj = j;
                        }
                    }
                }
            }
            if (maxLen) {
                path.push_back(maxIdxi);
                int t = maxIdxj;
                while (t != -1) {
                    path.push_back(t);
                    t = topSortPath[t];
                }
            }
        }
    }
}

void getPath(int *options) {
    path.clear();
    dp(options);

    for (int i = 1; i < path.size(); i++) {
        resultVector.push_back(rawWords[graph[path[i-1]][path[i]].front().second]);
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
        if (!topSort()) {
            throw bugReport(BUG_RING_EXIST);
        }
    }

    memset(din, 0, sizeof(din));
    rebuildGraph(options);

    if (options[OP_N]) {
        getAllLinks();
    } else if (options[OP_R]) {
        getMaxRing(options);
    } else {
        getPath(options);
    }

    if (resultVector.size() > 20000) {
        throw bugReport(BUG_CHAIN_TOO_LONG);
    }
    for (int i = 0; i < resultVector.size(); i++) {
        res[i] = stringToCharPointer(resultVector[i]);
    }

    return resultVector.size();
}