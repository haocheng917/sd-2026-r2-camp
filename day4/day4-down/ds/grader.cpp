#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <bitset>
#include "ds.h"

using namespace std;

static int N, Q;
static vector<int> ql, qr;

static bitset<50001> S;
static vector<int> add_stack;
static bool* reported;
static int report_cnt = 0;
static int S_size = 0;

static long long cnt_add = 0, cnt_del = 0, cnt_revoke = 0, cnt_clear = 0;
static long long C1, C2, C3, C4;

void add(int x) {
    cnt_add++;
    if (x < 1 || x > N) { fprintf(stderr, "add: x=%d out of range\n", x); exit(1); }
    if (S[x]) { fprintf(stderr, "add: x=%d already in S\n", x); exit(1); }
    S[x] = 1; S_size++;
    add_stack.push_back(x);
}

void del(int x) {
    cnt_del++;
    if (C2 == 0) { fprintf(stderr, "del: not allowed in this subtask\n"); exit(1); }
    if (x < 1 || x > N) { fprintf(stderr, "del: x=%d out of range\n", x); exit(1); }
    if (!S[x]) { fprintf(stderr, "del: x=%d not in S\n", x); exit(1); }
    S[x] = 0; S_size--;
}

void revoke() {
    cnt_revoke++;
    if (C3 == 0) { fprintf(stderr, "revoke: not allowed in this subtask\n"); exit(1); }
    while (!add_stack.empty() && !S[add_stack.back()]) add_stack.pop_back();
    if (add_stack.empty()) { fprintf(stderr, "revoke: nothing to revoke\n"); exit(1); }
    int x = add_stack.back();
    add_stack.pop_back();
    S[x] = 0; S_size--;
}

void clear() {
    cnt_clear++;
    S.reset(); S_size = 0;
    add_stack.clear();
}

void report(int k) {
    if (k < 1 || k > Q) { fprintf(stderr, "report: k=%d out of range\n", k); exit(1); }
    if (reported[k]) { fprintf(stderr, "report: %d already reported\n", k); exit(1); }
    int l = ql[k - 1], r = qr[k - 1];
    if (S_size != r - l + 1) {
        fprintf(stderr, "report: wrong size for %d, exp %d got %d\n", k, r - l + 1, S_size);
        exit(1);
    }
    for (int x = l; x <= r; x++) {
        if (!S[x]) {
            fprintf(stderr, "report: %d missing for interval %d\n", x, k);
            exit(1);
        }
    }
    reported[k] = true;
    report_cnt++;
}

int main() {
    if (scanf("%d%d", &N, &Q) != 2) return 0;
    scanf("%lld%lld%lld%lld", &C1, &C2, &C3, &C4);
    ql.resize(Q); qr.resize(Q);
    for (int i = 0; i < Q; i++) scanf("%d%d", &ql[i], &qr[i]);
    reported = new bool[Q + 1]();

    solve(N, Q, ql, qr);

    if (report_cnt != Q) { fprintf(stderr, "FAIL: %d/%d reported\n", report_cnt, Q); return 1; }
    if (cnt_add > C1) { fprintf(stderr, "FAIL: add limit exceeded\n"); return 1; }
    if (cnt_del > C2) { fprintf(stderr, "FAIL: del limit exceeded\n"); return 1; }
    if (cnt_revoke > C3) { fprintf(stderr, "FAIL: revoke limit exceeded\n"); return 1; }
    if (cnt_clear > C4) { fprintf(stderr, "FAIL: clear limit exceeded\n"); return 1; }

    fprintf(stderr, "OK: add=%lld del=%lld revoke=%lld clear=%lld\n", cnt_add, cnt_del, cnt_revoke, cnt_clear);
    delete[] reported;
    return 0;
}
