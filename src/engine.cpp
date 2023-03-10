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
int idx, wordsLen, vertexNum, ansWithLoop;
bool vis[MAX_V];
unordered_map<char *, int> words;
unordered_map<int, char *> idxToWords;
queue<int> spfaQueue;
vector<int> dfsVector;
vector<string> resultVector;
vector<string> selfCircle[26];

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
        int w1 = options[OP_W] ? 1 : strlen(rawWords[i]);
        int w2 = ((rawWords[i][strlen(rawWords[i]) - 1]) == (rawWords[i][0]));
        if (w2) {
            selfCircle[rawWords[i][0] - 'a'].push_back(rawWords[i]);
        }
        addEdge(words[rawWords[i]], wordsLen + rawWords[i][strlen(rawWords[i]) - 1] - 'a',
                w1);
        addEdge(wordsLen + rawWords[i][0] - 'a', words[rawWords[i]], w2 ? -w1 : 0);
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
    int tmp = -1, tmpfa = -1, tmpffa = -1;

    while (!spfaQueue.empty()) {
        int t = spfaQueue.front();
        spfaQueue.pop();
        vis[t] = false; // TODO problem with cycle(self + whole)
        for (int i = head[t]; ~i; i = ne[i]) {
            int j = e[i];
            if (dist[j] < dist[t] + w[i]) {
                dist[j] = dist[t] + w[i];
                path[j] = t;
                if (!vis[j]) {
                    spfaQueue.push(j);
                    vis[j] = true;
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

void dfsLoop(int s, int *options) {
    // get ans
    int tmpLen = dfsVector.size(), realLen = 0;
    int tmpAns = 0, lastChar = 0;
    for (int i = 0; i < tmpLen; i++) {
        if (dfsVector[i] >= wordsLen) {
            continue;
        }
        realLen++;
        lastChar = idxToWords[dfsVector[i]][strlen(idxToWords[dfsVector[i]]) - 1];
        if (options[OP_W]) {
            tmpAns++;
        } else {
            tmpAns += strlen(idxToWords[dfsVector[i]]);
        }
    }
    if (realLen >= 2 && (!options[OP_T] || options[OP_T] == lastChar)) {
        if (ansWithLoop < tmpAns) {
            ansWithLoop = tmpAns;
            realLen = 0;
            for (int i = 0; i < tmpLen; i++) {
                if (dfsVector[i] >= wordsLen) {
                    continue;
                }
                path[realLen++] = dfsVector[i];
            }
        }
    }

    for (int i = head[s]; ~i; i = ne[i]) {
        int j = e[i];
        if (j >= wordsLen || !vis[j]) {
            vis[j] = true;
            dfsVector.push_back(j);
            dfsLoop(j, options);
            dfsVector.pop_back();
            vis[j] = false;
        }
    }
}

void maxDistWithLoop(char start, int *options) {    // for -r
    ansWithLoop = 0;
    memset(vis, 0, sizeof(vis));
    memset(path, 0x3f, sizeof(dist));
    dfsVector.clear();
    dfsVector.push_back(start - 'a' + wordsLen);
    dfsLoop(start - 'a' + wordsLen, options);
}

int getResultPath(int *options) {
    int maxLen = 0;
    int maxIdx = -1;
    resultVector.clear();
    if (options[OP_R]) {
        for (int i = 0; i < 20005; i++) {
            if (path[i] == 0x3f3f3f3f) break;
            resultVector.push_back(idxToWords[path[i]]);
            if (options[OP_W]) {
                maxLen += 1;
            } else if (options[OP_C]) {
                maxLen += strlen(idxToWords[path[i]]);
            }
        }
    } else {
        if (options[OP_T]) {
            if (options[OP_W]) {
                maxIdx = wordsLen + options[OP_T] - 'a';
                maxLen = dist[maxIdx];
            } else if (options[OP_C]) {
                for (int i = 0; i < wordsLen; i++) {
                    if (rawWords[i][strlen(rawWords[i]) - 1] == options[OP_T]) {
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

        int tmp = maxIdx, lastVertex = -2;
        while (tmp != -1) {
            if (tmp < wordsLen) {
                resultVector.push_back(idxToWords[tmp]);
                lastVertex = tmp;
            }
            tmp = path[tmp];
            if (tmp == lastVertex) break;
            if (tmp == 0x3f3f3f3f) break;   // to prevent accident
        }
        reverse(resultVector.begin(), resultVector.end());
    }
    vector<string> tmpResult;
    if (!resultVector.empty()) {
        string tmp = resultVector[0];
        if (!selfCircle[tmp[0] - 'a'].empty()) {
            for (auto &j: selfCircle[tmp[0] - 'a']) {
                tmpResult.push_back(j);
            }
        }
    }
    for (auto &tmp: resultVector) {
        if (tmp[0] != tmp[tmp.size() - 1] && tmp != tmpResult.back()) {
            tmpResult.push_back(tmp);
        }
        if (!selfCircle[tmp[tmp.size() - 1] - 'a'].empty()) {
            for (auto &j: selfCircle[tmp[tmp.size() - 1] - 'a']) {
                tmpResult.push_back(j);
            }
        }
    }
    resultVector.clear();
    tmpResult.erase(unique(tmpResult.begin(), tmpResult.end()), tmpResult.end());
    maxLen = 0;
    for (auto &i: tmpResult) {
        if (options[OP_W]) {
            maxLen += 1;
        } else if (options[OP_C]) {
            maxLen += i.size();
        }
        resultVector.push_back(i);
    }
    if (resultVector.size() < 2) {
        resultVector.clear();
        return 0;
    }
    return maxLen;
}

void dfsLink(int s) {
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

    for (int i = head[s]; ~i; i = ne[i]) {
        int j = e[i];
        if (j >= wordsLen || !vis[j]) {
            vis[j] = true;
            dfsVector.push_back(j);
            dfsLink(j);
            dfsVector.pop_back();
            vis[j] = false;
        }
    }
}

void getAllLinks() {
    for (int i = wordsLen; i < vertexNum; i++) {
        memset(vis, 0, sizeof(vis));
        dfsVector.clear();
        dfsVector.push_back(i);
        dfsLink(i);
    }
}

int engine(int *options, char *res[]) {
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
        for (int i = 0; i < resultVector.size(); i++) {
            char *tmp = (char *) malloc(resultVector[i].size() + 1);
            int k = 0;
            for (k = 0; k < resultVector[i].size(); k++) {
                tmp[k] = resultVector[i][k];
            }
            tmp[k] = 0;
            res[i] = tmp;
        }
    } else {
        if (options[OP_H]) {
            if (options[OP_J] != options[OP_H]) {
                if (options[OP_R]) {
                    maxDistWithLoop(options[OP_H], options);
                } else {
                    spfa(options[OP_H]);
                }
                int t = getResultPath(options);
                if (ans < t) {
                    ans = t;
                    for (int i = 0; i < resultVector.size(); i++) {
                        char *tmp = (char *) malloc(resultVector[i].size() + 1);
                        int k = 0;
                        for (k = 0; k < resultVector[i].size(); k++) {
                            tmp[k] = resultVector[i][k];
                        }
                        tmp[k] = 0;
                        res[i] = tmp;
                    }
                }
            }
        } else {
            for (int i = 0; i < 26; i++) {
                char s = 'a' + i;
                if (options[OP_J] == s) continue;
                if (options[OP_R]) {
                    maxDistWithLoop(s, options);
                } else {
                    spfa(s);
                }
                int t = getResultPath(options);
                if (ans < t) {
                    ans = t;
                    for (int j = 0; j < resultVector.size(); j++) {
                        char *tmp = (char *) malloc(resultVector[j].size() + 1);
                        int k = 0;
                        for (k = 0; k < resultVector[j].size(); k++) {
                            tmp[k] = resultVector[j][k];
                        }
                        tmp[k] = 0;
                        res[j] = tmp;
                    }
                }
            }
        }
    }
    // max dist = ans, link = resultVector
    // problem here, has to decide whether the link contains 2 or more words

    return ans;
}