#include <bits/stdc++.h>
using namespace std;
const int N = 2010, p = 998244353;
int n, k, x[N], y[N];
long long qpow(long long x, int b) {
    long long res = 1;
    for (; b; b >>= 1) {
        if (b & 1) {
            res = res * x % p;
        }
        x = x * x % p;
    }
    return res;
}
int main() {
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> x[i] >> y[i];
    }
    long long f = 0;
    for (int i = 1; i <= n; i++) {
        long long mul = 1;
        for (int j = 1; j <= n; j++) {
            if (j == i) continue;
            mul = mul * ((k - x[j] + p) % p) % p * qpow((x[i] - x[j] + p) % p, p - 2) % p;
        }
        f = (f + mul * y[i] % p) % p;
    }
    cout << f << endl;
    return 0;
}