#ifndef DS_H
#define DS_H

#include <vector>

void add(int x);
void del(int x);
void revoke();
void clear();
void report(int k);

void solve(int n, int q, std::vector<int> L, std::vector<int> R);

#endif
