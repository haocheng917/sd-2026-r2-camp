/* 题：给出两个由同一棵树删去2个不同节点并打乱节点编号形成的森林，求原树
 * 解：n^2枚举两个删去的节点在另一棵森林上对应的编号u和v，
 * 对森林A将v在A中删去并将v周围的联通块标记为1其余为0
 * 对另一森林B同理，将两个森林的联通块进行匹配，至多有一个(1,1)（在u,v不相邻时），不可能有(0,0)，0一定匹配1
 * 用map维护A的联通块的哈希值，用B的标记为1的与A标为0的匹配，B中标记为0的与A中标为1的匹配
 *
 */
#include <bits/stdc++.h>
using namespace std;
using hash_t = unsigned long long;

const int N = 2010;
int n, m1, m2;
vector<int> g[2][N];
int siz[2][N];
bool vis[2][N];

hash_t mask = mt19937_64(time(nullptr))();
hash_t shift(hash_t x) {
    x ^= mask;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    x ^= mask;
    return x;
}

class Subtree {
public:
    int delNode;
    int id;
    int tmpRoot;
    vector<int> nodes;
    Subtree(int del, int idx, int root) :
        delNode(del), id(idx), tmpRoot(root) {
        getSize(tmpRoot);
        getControid(tmpRoot);
    }
    void getSize(int u, int fa = 0) {
        nodes.push_back(u);
        siz[id][u] = 1;
        vis[id][u] = true;
        for (int v : g[id][u]) {
            if (v == fa) continue;
            if (v == delNode) continue;
            getSize(v, u);
            siz[id][u] += siz[id][v];
        }
        if (fa == 0) {
            size = siz[id][u];
        }
    }
    void getControid(int u, int fa = 0) {
        int maxw = 0;
        for (int v : g[id][u]) {
            if (v == fa) continue;
            if (v == delNode) continue;
            getControid(v, u);
            maxw = max(maxw, siz[id][v]);
        }
        maxw = max(maxw, size - siz[id][u]);
        if (maxw <= size / 2) {
            centroid.push_back(u);
        }
    }
    hash_t getHash() {
        if (!hash) {
            for (int u : centroid) {
                hash = max(hash, getHash_(u));
            }
        }
        return hash;
    }
    hash_t getHash(int root) {
        return getHash_(root);
    }
    pair<hash_t, Subtree*> toPair() {
        return { getHash(), this };
    }
private:
    hash_t hash = 0;
    int size;
    vector <int> centroid;
    hash_t getHash_(int u, int fa = 0) {
        hash_t subhash = 1;
        for (int v : g[id][u]) {
            if (v == fa) continue;
            if (v == delNode) continue;
            subhash += shift(getHash_(v, u));
        }
        return subhash;
    }
};

map<hash_t, Subtree*> tr[2][N][2];
vector<pair<int, int>> ans;


int main() {
    cin >> n;
    cin >> m1;
    for (int i = 1, u, v; i <= m1; i++) {
        cin >> u >> v;
        g[0][u].push_back(v);
        g[0][v].push_back(u);
    }
    cin >> m2;
    for (int i = 1, u, v; i <= m2; i++) {
        cin >> u >> v;
        g[1][u].push_back(v);
        g[1][v].push_back(u);
    }
    for (int i = 1; i <= n; i++) {
        memset(vis, 0, sizeof(vis));
        for (int v : g[0][i]) {
            tr[0][i][0].insert(Subtree(i, 0, v).toPair());
        }
        for (int v : g[1][i]) {
            tr[1][i][0].insert(Subtree(i, 1, v).toPair());
        }
        for (int v = 1; v <= n; v++) {
            if (i == v) continue;
            if (!vis[0][v]) {
                tr[0][i][1].insert(Subtree(i, 0, v).toPair());
            }
            if (!vis[1][v]) {
                tr[1][i][1].insert(Subtree(i, 1, v).toPair());
            }
        }
    }
    for (int u = 1; u <= n; u++) {
        for (int v = 1; v <= n; v++) {
            if (tr[0][u][0].size() == tr[1][v][1].size() && tr[0][u][1].size() == tr[1][v][0].size()
                || tr[0][u][0].size() == tr[1][v][1].size() - 1 && tr[1][v][0].size() == tr[0][v][1].size() - 1) {
                bool flag = true;

                for (auto& a : tr[0][u][0]) {
                    if (tr[1][v][1].count(a.first)) {
                        flag = false;
                        break;
                    }
                }

                Subtree* p, * q;
                p = q = nullptr;
                for (auto& a : tr[0][u][1]) {
                    if (tr[1][v][0].count(a.first)) {
                        if (!p && !q) {
                            if (tr[1][v][1].count(a.first)) {
                                p = a.second;
                                q = tr[1][v][1][a.first];
                                continue;
                            }
                        }
                        flag = false;
                        break;
                    }
                }

                if (!flag) continue;

                vector<int> indexMap(n + 1, -1);

                if (p && q) {
                    map<hash_t, int> hashToNode;
                    sort(q->nodes.begin(), q->nodes.end());
                    for (int rt : q->nodes) {
                        hashToNode[q->getHash(rt)] = rt;
                    }

                    for (int node : g[0][u]) {
                        if (binary_search(q->nodes.begin(), q->nodes.end(), node)) {
                            indexMap[node] = hashToNode[p->getHash(node)];
                        }
                    }
                } else {
                    ans.emplace_back(u, n);
                }

                vector<bool> enable(n + 1, 1);

                for (auto& t : tr[1][v][0]) {
                    for (int node : t.second->nodes) {
                        enable[node] = 0;
                    }
                }
                for (auto& t : tr[1][v][1]) {
                    for (int node : t.second->nodes) {
                        enable[node] = 0;
                    }
                }

                auto getIndex = [&](int u) {
                    if (indexMap[u] == -1) {
                        static int idx = 1;
                        for (; ; idx++) {
                            if (enable[idx]) {
                                indexMap[u] = idx;
                                break;
                            }
                        }
                    }
                    return indexMap[u];
                };

                auto dfs1 = [&](auto&& self, int u, int fa) -> void {
                    for (int v : g[0][u]) {
                        if (v == fa) continue;
                        ans.emplace_back(getIndex(u), getIndex(v));
                        self(self, v, u);
                    }
                };

                dfs1(dfs1, u, -1);

                auto dfs2 = [&](auto&& self, int u, int fa) -> void {
                    for (int v : g[1][u]) {
                        if (v == fa) continue;
                        ans.emplace_back(u, v);
                        self(self, v, u);
                    }
                };

                dfs2(dfs2, v, -1);

                for (auto edge : ans) {
                    cout << edge.first << ' ' << edge.second << endl;
                }
            }
        }
    }
    return 0;
}