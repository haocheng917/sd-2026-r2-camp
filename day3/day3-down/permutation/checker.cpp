#include "testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);
    int t = inf.readInt();
    for (int tt = 0; tt < t; ++tt) {
        string ans_verdict = ans.readToken();
        string out_verdict = ouf.readToken();
        if (ans_verdict != out_verdict) {
            quitf(_wa, "Case #%d: Wrong answer \"%s\", expected %s", tt+1, out_verdict.c_str(), ans_verdict.c_str());
        }
        if (out_verdict == "NO") {
            int n = inf.readInt();
            int k = inf.readInt();
            for (int i = 0; i < n; ++i) {
                inf.readInt();  // skip colors
            }
        } else {
            int n = inf.readInt();
            int k = inf.readInt();
            vector<int> cu(n), ci(n);
            vector<long long> cs(n+1, 0);
            for (int i = 0; i < n; ++i) {
                cu[i] = ouf.readInt();
                ans.readInt();  // read correct answer (ignore)
                ci[i] = inf.readInt();
                cs[cu[i]] += 2LL * (i + 1) - n - 1;
            }
            sort(ci.begin(), ci.end());
            sort(cu.begin(), cu.end());
            for (int i = 0; i < n; ++i) {
                if (ci[i] != cu[i]) {
                    quitf(_wa, "Case #%d: Set of blocks is incorrect", tt+1);
                }
            }
            for (int i = 1; i <= n; ++i) {
                if (cs[i] != 0) {
                    quitf(_wa, "Case #%d: Color %d is not balanced", tt+1, i);
                }
            }
        }
    }
    quitf(_ok, "Accepted");
}
