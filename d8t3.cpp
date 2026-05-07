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
    bool connect;
    int delNode;
    int id;
    int tmpRoot;
    Subtree(int del, int idx, int root, bool connect) :
        delNode(del), id(idx), connect(connect), tmpRoot(root) {
        getSize(tmpRoot);
        getControid(tmpRoot);
    }
    void getSize(int u, int fa = 0) {
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
vector<Subtree> tr[2][N];

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
            tr[0][i].emplace_back(Subtree(i, 0, v, true));
        }
        for (int v : g[1][i]) {
            tr[1][i].emplace_back(Subtree(i, 1, v, true));
        }
        for (int v = 1; v <= n; v++) {
            if (!vis[0][v]) {
                tr[0][i].emplace_back(Subtree(i, 0, v, false));
            }
            if (!vis[1][v]) {
                tr[1][i].emplace_back(Subtree(i, 1, v, false));
            }
        }
    }
    return 0;
}