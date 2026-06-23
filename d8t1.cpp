// 考虑加入每个剩余数的后果，也就是计算当前状态下加入 $x$ 后，最长上升子序列长度为 $f(x)$。\
// 那么追加一个数 $u$ 后，所有 $v>u$，$f(v)=max(f(u)+1,f(v))$，即$\forall v>u 且 f(v)=f(u)$，$f(v)$ 增加 $1$。\
// 对于规则 1，若存在 $f(x)=k$，则爱丽丝必胜且方案数等于 $f(x)=k$ 的数量。
// 否则此后不可能出现 $f(x)=k$，所以双方只能选取 $f(x)<k$ 或者 $f(x)=k$ 且 $x$ 为所有满足 $f(x)=k$ 中最大的一个。\
// 对于规则 2，每次可以将一部分 $f(x) $ 变大1，删掉一个。
#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int T, n, v, q, k, c[N];
bool vis[N];
void add(int i, int x) {
    for (; i <= n; i += i & -i) {
        c[i] = max(c[i], x);
    }
}
int query(int i) {
    int res = 0;
    for (; i; i -= i & -i) {
        res = max(res, c[i]);
    }
    return res;
}
int main() {
    cin >> T;
    while (T--) {
        cin >> n >> v >> q >> k;
        memset(vis, 0, sizeof vis);
        memset(c, 0, sizeof c);
        for (int i = 1, x; i <= q; i++) {
            cin >> x;
            vis[x] = true;
            add(x, query(x - 1) + 1);
        }
        vector<pair<int, int> > s;
        for (int i = 1; i <= n; i++) {
            if (vis[i]) continue;
            s.emplace_back(query(i - 1) + 1, i);
        }
        sort(s.begin(), s.end());
        int ans = 0;
        int cnt = 0;
#define fx item.first
        if (v == 1) {
            for (auto item : s) {
                if (fx == k) {
                    ans = true;
                    cnt++;
                }
            }
            if (ans) {
                cout << "YES " << cnt << endl;
            } else {
                int add = 0;
                ans = 0;
                for (auto item : s) {
                    if (fx >= k) break;
                    ans++;
                    if (fx < k - 1) {
                        cnt++;
                    } else if (fx == k - 1) {
                        add = true;
                    }
                }
                ans %= 2;
                if (ans) {
                    cout << "YES " << cnt + add << endl;
                } else {
                    puts("NO");
                }
            }
        } else {
            for (auto item : s) {
                if (fx >= k) break;
                ans++;
                if (fx == k - 1) {
                    cnt++;
                }
            }
            if (cnt > 1) {
                puts("YES 1");
            } else {
                if (ans % 2) {
                    int cnt2 = 0;
                    for (auto item : s) {
                        if (fx >= k) break;
                        if (fx == k - 2) {
                            cnt2++;
                        }
                    }
                    cout << "YES " << ans - max(0, (cnt == 0 ? cnt2 - 2 : cnt2 - 1)) << endl;
                } else {
                    puts("NO");
                }
            }
        }
#undef fx
    }
    return 0;
}