#include "engine.h"
#include <cstring>
#include <cstdio>
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
vector<char *> result;

void getWordsIdx(vector<char *> rawWords) {
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
bool topsort(vector<char *> rawWords) {
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
    return tt == vertexNum;
}

void getGraph(vector<char *> rawWords, int *options) {
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

void spfa(vector<char *> rawWords, char start) {
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

int getResultPath(vector<char *> rawWords, int *options) {
    result.clear();
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
    while (path[tmp] != -1) {
        if (path[tmp] < wordsLen) {
            result.push_back(idxToWords[path[tmp]]);
        }
        tmp = path[tmp];
    }

    reverse(result.begin(), result.end());
    if (result.size() < 2) {
        result.clear();
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
    int tmpLen = dfsVector.size();
    string str;
    if (tmpLen >= 2) {
        for (int i = 0; i < tmpLen; i++) {
            if (dfsVector[i] >= wordsLen) {
                continue;
            }
            str += idxToWords[dfsVector[i]];
        }
        result.push_back((char *) str.data());
    }
}

void getAllLinks(vector<char *> rawWords) {
    for (int i = wordsLen; i < vertexNum; i++) {
        memset(vis, 0, sizeof(vis));
        dfsVector.clear();
        dfsVector.push_back(i);
        dfs(i);
    }
}

int engine(const vector<char *> &rawWords, int *options, vector<char *> &res) {
    getWordsIdx(rawWords);
    if (!options[OP_R]) {
        bool t = topsort(rawWords);
        // error handling
        // TODO
    }
    getGraph(rawWords, options);
    if (options[OP_N]) {
        getAllLinks(rawWords);
    } else {
        int ans = 0;    // max dist
        if (options[OP_H]) {
            spfa(rawWords, options[OP_H]);
//            int t = getResultNum(rawWords, options);
//            if (ans < t) {
//                getResultPath(rawWords, options);
//            }
            int t = getResultPath(rawWords, options);
            if (ans < t) ans = t;
        } else {
            for (int i = 0; i < 26; i++) {
                char s = 'a' + i;
                if (options[OP_J] == s) continue;
                spfa(rawWords, s);
//                int t = getResultNum(rawWords, options);
//                if (ans < t) {
//                    getResultPath(rawWords, options);
//                }
                int t = getResultPath(rawWords, options);
                if (ans < t) ans = t;
            }
        }
    }
    // max dist = ans, link = result
    // problem here, has to decide whether the link contains 2 or more words
    //TODO
    for (auto &i: result) {
        res.push_back(i);
    }
    return 1;
}