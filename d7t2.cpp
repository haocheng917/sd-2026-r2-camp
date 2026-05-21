/* 2-SAT题目，线段树建图
 * 2棵树，一课表示“任意”一棵表示“存在”
 */
#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 10;
int n, q, a[N], b[N], c[N], d[N];
int cnt;
vector<int> g[N], g2[N];

#define E(u) (u * 2)
#define A(u) (u * 2 - 1)
#define ls (u << 1)
#define rs (u << 1 | 1)
void build(int u = 1, int l = 1, int r = n) {
	cnt += 2;
	if (l == r) {
		g[A(u)].push_back(E(u));
		g[E(u)].push_back(A(u));
		return;
	}
	int mid = l + r >> 1;
	build(ls, l, mid);
	build(rs, mid + 1, r);
	g[A(u)].push_back(A(ls));
	g[A(u)].push_back(A(rs));
	g[E(ls)].push_back(E(u));
	g[E(rs)].push_back(E(u));
}
void addEdge1(int x, int y, int k, int u = 1, int l = 1, int r = n) {
	if (x <= l && r <= y) {
		g[E(u)].push_back(k);
		return;
	}
	int mid = l + r >> 1;
	if (x <= mid) addEdge1(x, y, k, ls, l, mid);
	if (y > mid) addEdge1(x, y, k, rs, mid + 1, r);
}
void addEdge2(int x, int y, int k, int u = 1, int l = 1, int r = n) {
	if (x <= l && r <= y) {
		g[k].push_back(A(u));
		return;
	}
	int mid = l + r >> 1;
	if (x <= mid) addEdge2(x, y, k, ls, l, mid);
	if (y > mid) addEdge2(x, y, k, rs, mid + 1, r);
}

bool vis[N], inSt[N];
int low[N], dfn[N];
int dfncnt, sc, st[N], top, scc[N], sz[N];
void tarjan(int u) {
	low[u] = dfn[u] = ++dfncnt;
	st[++top] = u;
	inSt[u] = 1;
	for (int v : g[u]) {
		if (!vis[v]) {
			tarjan(v);
			low[u] = min(low[u], low[v]);
		} else if (inSt[v]) {
			low[u] = min(low[u], dfn[v]);
		}
	}
	if (low[u] == u) {
		++sc;
		do {
			scc[st[top]] = sc;
			inSt[st[top]] = 0;
			sz[sc]++;
		} while (st[top--] != u);
	}
}

int main() {
	cin >> n >> q;
	n *= 3;
	build();
	for (int i = 1; i <= q; i++) {
		cin >> a[i] >> b[i] >> c[i] >> d[i];
		addEdge1(a[i], b[i], ++cnt);
		addEdge1(c[i], d[i], cnt);
	}

	for (int i = 1; i <= cnt; i++) {
		if (!vis[i]) {
			tarjan(i);
		}
	}

	for (int i = 1; i <= cnt; i++) {
		for (int v : g[i]) {
			if (scc[i] == scc[v]) continue;
			g2[scc[i]].push_back(scc[v]);
		}
	}
	int p = -1;
	for (int i = 1; i <= sc; i++) {
		sort(g2[i].begin(), g2[i].end());
		g2[i].erase(unique(g2[i].begin(), g2[i].end()), g2[i].end());
		if (sz[i] >= n / 3) {
			p = i;
		}
	}
	
	return 0;
}