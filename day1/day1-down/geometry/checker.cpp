#include <cstdint>

#include "testlib.h"

#include <algorithm>
#include <map>
#include <vector>

using namespace std;

namespace {

int normalizeAngle(int x) {
    x %= 360;
    if (x < 0) {
        x += 360;
    }
    return x;
}

vector<int> buildArcLengths(const vector<pair<int, int>>& cuts) {
    vector<int> points;
    points.reserve(cuts.size() * 2);

    for (const auto& [alpha, type] : cuts) {
        points.push_back(alpha);
        if (type == 1) {
            points.push_back(normalizeAngle(alpha + 180));
        }
    }

    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.empty()) {
        return {360};
    }

    vector<int> arcs;
    arcs.reserve(points.size());
    for (int i = 1; i < static_cast<int>(points.size()); ++i) {
        arcs.push_back(points[i] - points[i - 1]);
    }
    arcs.push_back(points.front() + 360 - points.back());
    return arcs;
}

}  // namespace

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);

    const int n = inf.readInt(1, 16, "n");
    vector<int> required(n);
    for (int i = 0; i < n; ++i) {
        required[i] = inf.readInt(1, 360, "q_i");
    }

    const int juryMinimum = ans.readInt(0, 1000000, "jury_m");
    const int participantM = ouf.readInt(0, 1000000, "m");

    if (participantM != juryMinimum) {
        quitf(_wa, "expected minimum cuts = %d, found %d", juryMinimum, participantM);
    }

    vector<pair<int, int>> cuts;
    cuts.reserve(participantM);
    for (int i = 0; i < participantM; ++i) {
        const int alpha = ouf.readInt(0, 359, "alpha_i");
        const int type = ouf.readInt(0, 1, "tp_i");
        cuts.push_back({alpha, type});
    }
    ouf.skipBlanks();
    ouf.readEof();

    vector<int> arcs = buildArcLengths(cuts);
    map<int, int> arcCount;
    for (int len : arcs) {
        ++arcCount[len];
    }

    for (int len : required) {
        auto it = arcCount.find(len);
        if (it == arcCount.end() || it->second == 0) {
            quitp(60, format("minimum cuts are correct, but required arc length %d is missing", len));
        }
        --it->second;
    }

    quitf(_ok, "accepted with correct minimum cuts and construction");
}
//by Chatgpt 5.4 codex