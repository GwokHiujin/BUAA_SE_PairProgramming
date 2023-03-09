#include "engine.h"
#include "bugReport.h"
#include "paramParser.h"
#include "engine.h"
#include <cstring>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>

using namespace std;

/* function 'engine' should return a validCode */

int head[MAX_V], e[MAX_E], ne[MAX_E], w[MAX_E], dist[MAX_V], path[MAX_V];    // e = edge, ne = next, w = weight
int topsortQueue[MAX_V], din[MAX_V];
int idx, wordsLen, vertexNum;
bool vis[MAX_V];
unordered_map<char *, int> words;
unordered_map<int, char *> idxToWords;
queue<int> spfaQueue;
vector<int> dfsVector;
vector<string> resultVector;

void getWordsIdx() {
    wordsLen = rawWords.size();
    vertexNum = wordsLen + 26;
    int tmp = 0;
    for (int i = 0; i < wordsLen; i++) {
        if (!words.count(rawWords[i])) {
            idxToWords[tmp] = rawWords[i];
            words[rawWords[i]] = tmp++;
        }
    }
}

void initGraph() {
    memset(head, -1, sizeof(head));
    idx = 0;
}

void addEdge(int from, int to, int weight) {
    e[idx] = to;
    w[idx] = weight;
    ne[idx] = head[from];
    head[from] = idx++;
}

// true: no ring, false: has ring
bool topsort() {
    // check for word as %...%
    int countHET[26];  //   count words whose head equals tail
    memset(countHET, 0, sizeof(countHET));
    for (int i = 0; i < wordsLen; i++) {
        if (rawWords[i][0] == rawWords[i][strlen(rawWords[i]) - 1]) {
            countHET[rawWords[i][0] - 'a']++;
            if (countHET[rawWords[i][0] - 'a'] == 2) {
                return false;
            }
        }
    }

    // build graph
    initGraph();
    // A: wordsLen, B: wordsLen + 1, etc.
    for (int i = 0; i < wordsLen; i++) {
        if (rawWords[i][0] == rawWords[i][strlen(rawWords[i]) - 1]) {
            continue;
        }
        addEdge(words[rawWords[i]], wordsLen + rawWords[i][strlen(rawWords[i]) - 1] - 'a', 1);
        addEdge(wordsLen + rawWords[i][0] - 'a', words[rawWords[i]], 1);
        din[wordsLen + rawWords[i][strlen(rawWords[i]) - 1] - 'a']++;
        din[words[rawWords[i]]]++;
    }

    // the real topsort itself
    int tt = -1, hh = 0;    // head and tail of the queue
    for (int i = 0; i < vertexNum; i++) {
        if (din[i] == 0) {
            topsortQueue[++tt] = i;
        }
    }
    while (hh <= tt) {
        int t = topsortQueue[hh++]; // get front and pop
        for (int i = head[t]; ~i; i = ne[i]) {
            int j = e[i];
            if (--din[j] == 0) {
                topsortQueue[++tt] = j;
            }
        }
    }
    return tt == vertexNum - 1;
}

void getGraph(int *options) {
    // rebuild graph
    initGraph();
    for (int i = 0; i < wordsLen; i++) {
        if (options[OP_J]) {
            if (rawWords[i][0] == OP_J) continue;
        }
        addEdge(words[rawWords[i]], wordsLen + rawWords[i][strlen(rawWords[i]) - 1] - 'a',
                options[OP_W] ? 1 : strlen(rawWords[i]));
        addEdge(wordsLen + rawWords[i][0] - 'a', words[rawWords[i]], 0);
    }
}

void spfa(char start) {
    memset(vis, 0, sizeof(vis));
    memset(dist, -1, sizeof(dist));
    memset(path, 0x3f, sizeof(dist));
    dist[start - 'a' + wordsLen] = 0;
    vis[start - 'a' + wordsLen] = true;
    path[start - 'a' + wordsLen] = -1;
    spfaQueue.push(start - 'a' + wordsLen);

    while (!spfaQueue.empty()) {
        int t = spfaQueue.front();
        spfaQueue.pop();
        vis[t] = false;
        for (int i = head[t]; ~i; i = ne[i]) {
            int j = e[i];
            if (dist[j] < dist[t] + w[i]) {
                dist[j] = dist[t] + w[i];
                path[j] = t;
                if (!vis[j]) {
                    spfaQueue.push(j);
                }
            }
        }
    }
}

//int getResultNum(vector<char *> rawWords, int *options) {
//    int maxLen = 0;
//    if (options[OP_T]) {
//        if(options[OP_W]) {
//            maxLen = dist[wordsLen + options[OP_T] - 'a'];
//        } else if(options[OP_C]) {
//            // check all words which ends with options[OP_T] - 'a'
//        }
//    } else {
//        if (options[OP_W]) {
//            for (int i = wordsLen; i < vertexNum; i++) {
//                if (dist[i] > maxLen) {
//                    maxLen = dist[i];
//                }
//            }
//        } else if(options[OP_C]) {
//            for (int i = 0; i < wordsLen; i++) {
//                if (dist[i] > maxLen) {
//                    maxLen = dist[i];
//                }
//            }
//        }
//    }
//    return maxLen;
//}

int getResultPath(int *options) {
    resultVector.clear();
    int maxLen = 0;
    int maxIdx = 0;

    if (options[OP_T]) {
        if (options[OP_W]) {
            maxIdx = wordsLen + options[OP_T] - 'a';
            maxLen = dist[maxIdx];
        } else if (options[OP_C]) {
            for (int i = 0; i < wordsLen; i++) {
                if (rawWords[i][strlen(rawWords[i]) - 1] == options[OP_C]) {
                    if (dist[i] > 0) {
                        int t = dist[i] + strlen(rawWords[i]);
                        if (t > maxLen) {
                            maxLen = t;
                            maxIdx = i;
                        }
                    }
                }
            }
        }
    } else {
        if (options[OP_W]) {
            for (int i = wordsLen; i < vertexNum; i++) {
                if (dist[i] > maxLen) {
                    maxLen = dist[i];
                    maxIdx = i;
                }
            }
        } else if (options[OP_C]) {
            for (int i = 0; i < wordsLen; i++) {
                if (dist[i] > 0) {
                    int t = dist[i] + strlen(rawWords[i]);
                    if (t > maxLen) {
                        maxLen = t;
                        maxIdx = i;
                    }
                }
            }
        }
    }

    int tmp = maxIdx;
    while (tmp != -1) {
        if (tmp < wordsLen) {
            resultVector.push_back(idxToWords[tmp]);
        }
        tmp = path[tmp];
        if (tmp == 0x3f3f3f3f) break;   // to prevent accident
    }

    reverse(resultVector.begin(), resultVector.end());
    if (resultVector.size() < 2) {
        resultVector.clear();
        return 0;
    }
    return maxLen;
}

void dfs(int s) {
    for (int i = head[s]; ~i; i = ne[i]) {
        int j = e[i];
        if (!vis[j]) {
            vis[j] = true;
            dfsVector.push_back(s);
            dfs(j);
            dfsVector.pop_back();
            vis[j] = false;
        }
    }

    // get ans
    int tmpLen = dfsVector.size(), realLen = 0;
    string str;
    for (int i = 0; i < tmpLen; i++) {
        if (dfsVector[i] >= wordsLen) {
            continue;
        }
        realLen++;
        str += idxToWords[dfsVector[i]];
        str += " ";
    }
    if (realLen >= 2) {
        if (resultVector.empty() || str != resultVector.back()) {
            resultVector.push_back(str);
            if (resultVector.size() > 20000) {
                resultVector.clear();
                throw bugReport(BUG_CHAIN_TOO_LONG);
            }
        }
    }
}

void getAllLinks() {
    for (int i = wordsLen; i < vertexNum; i++) {
        memset(vis, 0, sizeof(vis));
        dfsVector.clear();
        dfsVector.push_back(i);
        dfs(i);
    }
}

int engine(int *options, vector<string> &res) {
    getWordsIdx();
    if (!options[OP_R]) {
        bool t = topsort();
        if (!t) {
            throw bugReport(BUG_RING_EXIST);
        }
    }
    getGraph(options);
    int ans = 0;    // max dist
    if (options[OP_N]) {
        getAllLinks();
        ans = resultVector.size();
        for (auto &str: resultVector) {
            res.push_back(str);
        }
    } else {
        if (options[OP_H]) {
            spfa(options[OP_H]);
            int t = getResultPath(options);
            if (ans < t) {
                ans = t;
                res.clear();
                for (auto &str: resultVector) {
                    res.push_back(str);
                }
            }
        } else {
            for (int i = 0; i < 26; i++) {
                char s = 'a' + i;
                if (options[OP_J] == s) continue;
                spfa(s);
                int t = getResultPath(options);
                if (ans < t) {
                    ans = t;
                    res.clear();
                    for (auto &str: resultVector) {
                        res.push_back(str);
                    }
                }
            }
        }
    }
    // max dist = ans, link = resultVector
    // problem here, has to decide whether the link contains 2 or more words

    return ans;
}