#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N = 1010;
int n, m, q;
char a[N][N];

struct vec {
    int x, y;
    vec() : x(0), y(0) {}
    vec(int a, int b) : x(a), y(b) {}
    vec operator +(vec other) const {
        return { x + other.x, y + other.y };
    }
    vec operator -(vec other) const {
        return { x - other.x, y - other.y };
    }
    vec operator *(int other) const {
        return { x * other, y * other };
    }
    vec operator -() const {
        return { -x, -y };
    }
    bool zero() const {
        return x == 0 && y == 0;
    }
};

struct CC {
    vec a;
    int b = 0;
    void add(vec x) {
        if (a.zero()) {
            a = x;
            return;
        }
        if (x.x < 0) x = -x;
        if (a.x < x.x) swap(a, x);
        while (x.x != 0) {
            int cnt = a.x / x.x;
            a = a - x * cnt;
            swap(a, x);
        }
        b = __gcd(b, x.y);
    }
    bool query(vec v) {
        if (v.x) {
            if (!a.x || v.x % a.x != 0) return false;
            v.y -= v.x / a.x * a.y;
        }
        if (v.y == 0) return true;
        if (b == 0) return false;
        return (v.y % b == 0);
    }
}cc[N * N];

int fa[N * N];
void init() {
    for (int i = 0; i < n * m; i++) {
        fa[i] = i;
    }
}
int find(int x) {
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(int u, int v) {
    u = find(u), v = find(v);
    if (u != v) {
        fa[u] = v;
    }
}

const vec dir[] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
#define id(x, y) (x * n + y)
vector<pair<int, vec>> g[N * N];
bool vis[N * N];
vec dep[N * N];

void dfs(int u) {
    vis[u] = true;
    for (auto e : g[u]) {
        int v = e.first;
        if (!vis[v]) {
            dep[v] = dep[u] + e.second;
            dfs(v);
        } else {
            cc[find(u)].add(dep[u] - dep[v] + e.second);
        }
    }
}

signed main() {
    cin >> n >> m;
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }

    init();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (a[i][j] == '#') continue;
            for (int k = 0; k < 4; k++) {
                int nx = (i + dir[k].x + n) % n, ny = (j + dir[k].y + m) % m;
                if (a[nx][ny] == '#') continue;
                merge(id(nx, ny), id(i, j));
                g[id(i, j)].emplace_back(id(nx, ny), dir[k]);
            }
        }
    }

    for (int i = 0; i < n * m; i++) {
        if (fa[i] == i) {
            dfs(i);
        }
    }

    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < m; j++) {
    //         cout << dep[id(i, j)].x << ',' << dep[id(i, j)].y << '\t';
    //     }
    //     cout << endl;
    // }

    cin >> q;
    while (q--) {
        int sx, sy, tx, ty;
        cin >> sy >> sx >> ty >> tx;
        int u = id(sx % n, sy % m), v = id(tx % n, ty % m);
        if (find(u) != find(v)) {
            puts("No");
        } else {
            vec tmp = vec{ tx, ty } - vec{ sx, sy } - (dep[v] - dep[u]);
            cout << tmp.x << ' ' << tmp.y << endl;
            puts(cc[find(u)].query(tmp) ? "Yes" : "No");
        }
    }
    return 0;
}