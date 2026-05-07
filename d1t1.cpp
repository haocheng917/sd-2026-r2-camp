#include <bits/stdc++.h>
using namespace std;
const int N = 100200;
int n, fa[N];
vector<int> g[N];
int st[N], top;
int vis[N], ans[N], cnt[N];
struct Circle {
    vector<int> nodes; // 环上的点 
    vector<vector<int>> dis; // 环上每个点的子树到该点的距离 
    int tot; // 树上点数 
    int len;
    Circle(const vector<int>& v) : nodes(v) {
        len = v.size();
        dis.resize(v.size());
        tot = 0;
    }
};
vector<Circle> c;
struct Length {
    int len;
    vector<int> res;
    int base; // 最佳环长为len的环，不断开 
};
vector<Length> bkt;
void dfs(int u, int rt) { // 找环 
    vis[u] = -rt;
    st[++top] = u;
    for (int v : g[u]) {
        if (vis[v] == -rt) {
            vector<int> vec;
            while (st[top] != v) {
                vec.push_back(st[top]);
                vis[st[top]] = 1;
                top--;
            }
            vec.push_back(v);
            vis[v] = 1;
            c.push_back(vector<int>(vec.rbegin(), vec.rend())); // 后面指向前面 
            continue;
        } else if (vis[v]) continue;
        dfs(v, rt);
    }
    top--;
}
void dfs2(int u, int pos, vector<int>& dis, int id) {
    dis.push_back(pos);
    c[id].tot++;
    for (int v : g[u]) {
        if (vis[v] > 0) continue;
        dfs2(v, pos + 1, dis, id);
    }
}
int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> fa[i];
        g[fa[i]].push_back(i);
    }
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) {
            top = 0;
            dfs(i, i);
        }
    }
    for (int j = 0; j < c.size(); j++) {
        for (int i = 0; i < c[j].len; i++) {
            int rt = c[j].nodes[i];
            dfs2(rt, 0, c[j].dis[i], j);
        }
    }
    sort(c.begin(), c.end(), [=](const Circle& a, const Circle& b) {
        return a.len < b.len;
        });
    for (int i = 0; i < c.size();) {
        int tmp = c[i].len;
        for (i++; i < c.size() && c[i].len == tmp; i++);
        bkt.push_back({ tmp, vector<int>(), 0 });
    }
    for (auto& w : bkt) {
        int mod = w.len;
        for (auto& v : c) {
            int tmp = 0, delta = 0;
            memset(cnt, 0, mod * sizeof(int));
            for (int i = 0; i < v.len; i++) { // 枚举断那一条边 
                if (i == 0) {
                    for (int j = 0; j < v.len; j++) {
                        for (int k : v.dis[j]) {
                            cnt[(k + j) % mod]++;
                        }
                    }
                } else {
                    delta++;
                    for (int k : v.dis[i - 1]) {
                        cnt[(k + v.len - 1 + delta) % mod]++;
                    }
                }
                for (int j = 0; j < mod; j++) {
                    tmp = max(tmp, cnt[j]);
                }
                for (int k : v.dis[i]) {
                    cnt[(k + delta) % mod]--;
                }
            }
            // 0<-1<-2<-3
            // <-1<-2<-3<-0 
            if (v.len == mod) {
                if (tmp > w.base) {
                    if (w.base) w.res.push_back(w.base);
                    w.base = tmp;
                    continue;
                }
            }
            w.res.push_back(tmp); // v对环长为w.len的环的贡献 
        }
        sort(w.res.begin(), w.res.end(), greater<int>());
        ans[0] = max(ans[0], w.base);
        for (int i = 0; i < w.res.size(); i++) {
            w.base += w.res[i];
            ans[i + 1] = max(ans[i + 1], w.base);
        }
    }
    sort(c.begin(), c.end(), [=](const Circle& a, const Circle& b) {
        return a.tot > b.tot;
        });
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        if (i - 1 < c.size()) {
            sum += c[i - 1].tot;
            ans[i] = max(sum, ans[i]);
        } else {
            ans[i] = n;
        }
    }
    for (int k = 0; k <= n; k++) {
        cout << ans[k] << ' ';
    }
    return 0;
}