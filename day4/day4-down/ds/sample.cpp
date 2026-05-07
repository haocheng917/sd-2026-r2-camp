#include "ds.h"
#include <bits/stdc++.h>

using namespace std;

void solve(int n, int q, vector<int> L, vector<int> R) {
    for (int i = 0; i < q; i++) {
        clear();
        for (int x = L[i]; x <= R[i]; x++) {
            add(x);
        }
        report(i + 1);
    }
}
