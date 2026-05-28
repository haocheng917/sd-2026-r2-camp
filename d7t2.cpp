/* 2-SAT题目，线段树建图
 * 2棵树，一课表示“任意”一棵表示“存在”
 */
#include <bits/stdc++.h>
using namespace std;

const int N = 1.2e6 + 10;
int n, q, a[N], b[N], c[N], d[N];
int id[N];
int cnt;
bool realNode[N];
vector<int> g[N], g2[N], g3[N];

#define E(u) (u * 2)
#define A(u) (u * 2 - 1)
#define ls (u << 1)
#define rs (u << 1 | 1)
void build(int u = 1, int l = 1, int r = n) {
	cnt = max(E(u), cnt);
	if (l == r) {
		id[l] = E(u);
		realNode[E(u)] = true;
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
	vis[u] = true;
	for (int v : g[u]) {
		if (!vis[v]) {
			tarjan(v);
			low[u] = min(low[u], low[v]);
		} else if (inSt[v]) {
			low[u] = min(low[u], dfn[v]);
		}
	}
	if (low[u] == dfn[u]) {
		++sc;
		do {
			scc[st[top]] = sc;
			inSt[st[top]] = 0;
			sz[sc] += realNode[st[top]];
		} while (st[top--] != u);
	}
}

bool vir[N];
int in[N];

int main() {
	// freopen("day7\\day7-down\\robot\\ex_robot3.in", "r", stdin);
	int T;
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> T;
	while (T--) {
		cin >> n >> q;

		memset(vis, 0, sizeof vis);
		memset(vir, 0, sizeof vir);
		memset(in, 0, sizeof in);
		memset(sz, 0, sizeof sz);
		memset(realNode, 0, sizeof realNode);
		for (int i = 0; i <= cnt; i++) {
			g[i].clear();
		}
		for (int i = 0; i <= sc; i++) {
			g2[i].clear();
			g3[i].clear();
		}
		sc = 0;
		top = 0;
		cnt = 0;

		n *= 3;
		build();
		for (int i = 1; i <= q; i++) {
			cin >> a[i] >> b[i] >> c[i] >> d[i];
			addEdge1(a[i], b[i], ++cnt);
			addEdge2(c[i], d[i], cnt);
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
			for (int v : g2[i]) in[v]++;
			if (sz[i] >= n / 3) {
				p = i;
			}
		}

		if (p != -1) {
			queue<int> q;
			q.push(p);
			int sum = 0;
			while (q.size()) {
				int u = q.front();
				q.pop();
				sum += sz[u];
				vir[u] = true;
				for (int v : g2[u]) {
					q.push(v);
				}
			}
			if (sum <= n / 3 * 2) {
				puts("Yes");
				for (int i = 1; i <= n; i++) {
					putchar('0' + vir[scc[id[i]]]);
				}
				putchar('\n');
			} else {
				for (int u = 1; u <= sc; u++) {
					for (int v : g2[u]) {
						g3[v].push_back(u);
					}
					vir[u] = true;
				}
				q.push(p);
				int sum = 0;
				while (q.size()) {
					int u = q.front();
					q.pop();
					sum += sz[u];
					vir[u] = false;
					for (int v : g3[u]) {
						q.push(v);
					}
				}
				if (sum <= n / 3 * 2) {
					puts("Yes");
					for (int i = 1; i <= n; i++) {
						putchar('0' + vir[scc[id[i]]]);
					}
					putchar('\n');
				} else {
					puts("No");
				}
			}
		} else {
			queue<int> q;
			for (int i = 1; i <= sc; i++) {
				if (!in[i]) {
					q.push(i);
				}
				vir[i] = true;
			}
			int sum = 0;
			bool ans = 0;
			while (q.size()) {
				int u = q.front();
				q.pop();
				sum += sz[u];
				vir[u] = false;
				if (n / 3 <= sum && sum <= n / 3 * 2) {
					ans = 1;
					break;
				} else if (sum > n / 3 * 2) {
					break;
				}
				for (int v : g2[u]) {
					in[v]--;
					if (!in[v]) {
						q.push(v);
					}
				}
			}
			if (ans) {
				puts("Yes");
				for (int i = 1; i <= n; i++) {
					putchar('0' + vir[scc[id[i]]]);
				}
				putchar('\n');
			} else {
				puts("No");
			}
		}
	}
	return 0;
}