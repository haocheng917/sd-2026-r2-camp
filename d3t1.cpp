#include <bits/stdc++.h>
using namespace std;
const int N = 200010;
#define int long long
int vis[N];
bool flag;
int n, k, c[N], cnt[N];
void dfs(int col, int tot, int m, int p) {
	if (flag) return;
	if (col > k) {
		puts("YES");
		for (int i = 1; i <= n; i++) {
			cout << vis[i] << ' ';
		}
		cout << endl;
		flag = true;
		return;
	}
	if (m + (n + n - cnt[col] + tot) * (cnt[col] - tot + 1) / 2 < (n + 1) * cnt[col] / 2) return;
	if (m >= (n + 1) * cnt[col] / 2) return;
	if (tot == cnt[col]) {
		int l = (n + 1) * cnt[col] / 2 - m;
		if (vis[l] || l > n || l < 1) return;
		vis[l] = col;
		dfs(col + 1, 1, 0, 1);
		vis[l] = 0;
		return;
	}
	for (int i = p; i <= n; i++) {
		if (vis[i]) continue;
		vis[i] = col;
		dfs(col, tot + 1, m + i, i + 1);
		vis[i] = 0;
	}
}
void subtask3() {
	int p = 1;
	for (int i = 1; i <= k; i++) {
		if (cnt[i] % 2) {
			for (int j = (n + 1) / 2 - cnt[i] / 2; j <= (n + 1) / 2 + cnt[i] / 2; j++) {
				vis[j] = i;
			}
		} else {
			for (int j = 1; j <= cnt[i]; j += 2) {
				vis[p] = vis[n - p + 1] = i;
				p++;
			}
		}
	}
	puts("YES");
	for (int i = 1; i <= n; i++) {
		cout << vis[i] << ' ';
	}
	cout << endl;
}


void subtask4() {
	int o = n / k;
	int p = 1;
	if (o % 2) {
		if (k % 2 == 0) {
			puts("NO");
			return;
		}
		if (o == 1) {
			puts("NO");
			return;
		}
		for (int i = 1; i <= k; i++) {
			vis[p] = i;
			p++;
		}
		for (int j = k; j >= 1; j -= 2) {
			vis[p] = j;
			p++;
		}
		for (int j = k - 1; j >= 1; j -= 2) {
			vis[p] = j;
			p++;
		}
		for (int j = k - 1; j >= 1; j -= 2) {
			vis[p] = j;
			p++;
		}
		for (int j = k; j >= 1; j -= 2) {
			vis[p] = j;
			p++;
		}
		o -= 3;
	}
	for (int i = 1; i <= o / 2; i++) {
		for (int j = 1; j <= k; j++) {
			vis[p++] = j;
		}
		for (int j = k; j >= 1; j--) {
			vis[p++] = j;
		}
	}
	puts("YES");
	for (int i = 1; i <= n; i++) {
		cout << vis[i] << ' ';
	}
	cout << endl;
}
signed main() {
	//	freopen("ex_permutation4.in", "r", stdin);
	//	freopen("ex_permutation4.out", "w", stdout);
	int t;
	cin >> t;
	while (t--) {
		cin >> n >> k;
		for (int i = 1; i <= k; i++) {
			cnt[i] = 0;
		}
		cout << cnt[1 -100];
		for (int i = 1; i <= n; i++) {
			cin >> c[i];
			cnt[c[i]]++;
			vis[i] = 0;
		}
		int st3 = 0;
		bool no = false, st4 = 1;
		for (int i = 1; i <= k; i++) {
			if ((n + 1) * cnt[i] % 2) {
				no = true;
				break;
			}
			st3 += cnt[i] % 2;
			st4 &= (cnt[i] == cnt[1]);
		}
		if (no) {
			puts("NO");
			continue;
		}
		flag = 0;
		if (st3 <= 1) {
			subtask3();
		} else if (st4) {
			subtask4();
		} else if (st3 > 1) {
			dfs(1, 1, 0, 1);
			if (!flag) {
				puts("NO");
				continue;
			}
		}
	}
	while(1);
	return 0;
}
/*
1~n
n * (n+1)/2 / k
n^2+n /2k
pkpk+pk/2k
p^2*k+p/2
p(pk+1)/2

c * k == n
1 2 3 4
8 7 6 5   2k+1
9 10 11 12
16 15 14 13 6k+1
17 18 19 20  10k+1
24 23 22 21

n / k even
(4k+2 + 4k * (n/2k - 1)) * n/2k / 2
4k+2+2n-4k) * n /4k
(2n+2)n/4k
n^2+n)/2k
n / k odd

123456789 3
45/3=15
1 5 9
2 6 7
3 4 8
123456789101112131415 5

12 4
1 8 9 11
2 6 8 10
3 7 10 12
4 5 9 9

1 9 10
2 12 14
3 8 11
4 11 15
5 7 12
6 10 16




1 14 =15 27 =42 28 45 =115
2 18 =20 22 =42 29 44 =115
3 13 =16 26 =42 30 43
4 17 =21 21 =42 31 42
5 12 =17 25 =42 32 41
6 16 =22 20 =42 33 40
7 11 =18 24 =42 34 39
8 15 =23 19 =42 35 38
9 10 =19 23 =42 36 37

27
3
1 14 27


*/