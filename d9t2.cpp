/* 题：三维空间n个球体，求第k小距离
 * 解：二分答案x，半径增加x/2，判断是否有k个球相交
 * 按球最大半径的2倍建网格，两球相交则一定在相邻方格中
 * 遍历每个球以及周围网格，大部分小球无法与此球相交
 * 故只用map+hash存储大球，判断大球与大球，大球与小球是否相交
 * 重构网格来判断小球是否相交，重构后网格大小为最大小球半径2倍
 * 若大球的半径大于r/2，则一网格至多O(根号k)个就能产生k个球相交
 * 总时间复杂度O(log^2+log*sqrt)
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 200010;
int n, k;
struct Ball {
    int x, y, z, r;
}a[N];
unsigned long long getHash(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0) return -1;
    return ((unsigned long long)x << 40) + ((unsigned long long)y << 20) + z;
}
unordered_map<unsigned long long, vector<int>> m;
vector<pair<int, int>> v;
bool check(Ball a, Ball b) {
    long long dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz <= (long long)(a.r + b.r) * (a.r + b.r);
}
int dis(Ball a, Ball b) {
    long long dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
    return max(0, (int)ceil((sqrt(dx * dx + dy * dy + dz * dz) - a.r - b.r) / 2));
}
void solve(int n) {
    if (n == 0) return;
    int size = a[n].r * 2;
    int cnt = 0;
    while (a[cnt + 1].r < a[n].r / 2) cnt++;
    m.clear();
    for (int i = n; i >= 1; i--) {
        int x = a[i].x / size, y = a[i].y / size, z = a[i].z / size;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    unsigned long long t = getHash(x + dx, y + dy, z + dz);
                    if (!m.count(t)) continue;
                    for (int j : m[t]) {
                        if (check(a[i], a[j])) {
                            v.emplace_back(i, j);
                            if (v.size() >= k) {
                                return;
                            }
                        }
                    }
                }
            }
        }
        if (i > cnt) m[getHash(x, y, z)].push_back(i);
    }
    solve(cnt);
}
bool check(int x) {
    for (int i = 1; i <= n; i++) {
        a[i].r += x;
    }
    v.clear();
    solve(n);
    for (int i = 1; i <= n; i++) {
        a[i].r -= x;
    }
    return v.size() >= k;
}
int main() {
    // freopen(R"(day9\day9-down\kind\kind2.in)", "r", stdin);
    int T;
    cin >> T;
    while (T--) {
        cin >> n >> k;
        for (int i = 1; i <= n; i++) {
            cin >> a[i].x >> a[i].y >> a[i].z >> a[i].r;
            a[i].x *= 2, a[i].y *= 2, a[i].z *= 2, a[i].r *= 2;
        }
        sort(a + 1, a + 1 + n, [](Ball u, Ball v) {
            return u.r < u.r;
            });
        int l = 0, r = 1e9, ans;
        while (l <= r) {
            int mid = l + r >> 1;
            if (check(mid)) {
                r = mid - 1;
                ans = mid;
            } else {
                l = mid + 1;
            }
        }
        check(ans - 1);
        vector<int> res;
        for (auto it : v) {
            res.push_back(dis(a[it.first], a[it.second]));
        }
        sort(res.begin(), res.end());
        for (int x : res) {
            cout << x << endl;
        }
        for (int i = res.size() + 1; i <= k; i++) {
            cout << ans << endl;
        }
    }
    return 0;
}