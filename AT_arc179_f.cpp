#include <bits/stdc++.h>
using namespace std;

const int N = 1000100;

int n;
string s;
int ans[N];

int diff[N], pre[N], len[N];

class BIT {
    pair<int, int> t[N];
public:
    int n;
    void clear() {
        memset(t, 0, (n + 1) * sizeof(pair<int, int>));
    }
    void upd(int i, pair<int, int> data) {
        for (; i <= n; i += i & -i) {
            t[i] = max(t[i], data);
        }
    }
    pair<int, int> query(int i) {
        pair<int, int> res = { 0, 0 };
        for (; i; i -= i & -i) {
            res = max(res, t[i]);
        }
        return res;
    }
}tree;

void build(int l, int r) {
    int left = 1, right = 3, delta = diff[r] - diff[l - 1];
    static int cur_diff[N];

    cur_diff[l - 1] = 0;
    for (int i = l; i <= r; i++) {
        if (s[i - 1] == 'A') {
            ans[i] = left;
            cur_diff[i] = cur_diff[i - 1] + 2;
        } else {
            ans[i] = right;
            cur_diff[i] = cur_diff[i - 1] - 1;
        }

        if (delta > 0) {
            if (cur_diff[i - 1] == 2 && cur_diff[i] == 4) {
                ans[i] = 2;
                swap(left, right);
                delta -= 6;
                cur_diff[i] = -2;
            } else if (cur_diff[i - 1] == 3 && cur_diff[i] == 5) {
                ans[i - 1] = 2;
                ans[i] = right;
                delta -= 6;
                cur_diff[i] = -1;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(0);

    int T;
    cin >> T;
    while (T--) {
        cin >> n;
        cin >> s;

        for (int i = 1; i <= n; i++) {
            diff[i] = diff[i - 1] + (s[i - 1] == 'A' ? 2 : -1);
            pre[i] = len[i] = 0;
        }

        tree.n = n;
        tree.clear();
        pair<int, int> end = { 0, 0 };
        for (int i = 1; i <= n; i++) {
            if (diff[i] % 6 || diff[i] < 0) continue;
            int idx = diff[i] / 6 + 1;
            tie(len[i], pre[i]) = tree.query(idx);
            tree.upd(idx, { ++len[i], i });
            end = max(end, { len[i], i });
        }

        vector<int> points;
        for (int i = end.second; i; i = pre[i]) {
            points.push_back(i);
        }
        points.push_back(0);
        reverse(points.begin(), points.end());
        for (int i = 1; i < points.size(); i++) {
            build(points[i - 1] + 1, points[i]);
        }
        for (int i = points.back() + 1; i <= n; i++) {
            ans[i] = (s[i - 1] == 'A' ? 1 : 3);
        }

        for (int i = 1; i <= n; i++) {
            cout << ans[i];
        }
        cout << endl;
    }
    return 0;
}