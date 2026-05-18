#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

uint32_t rst = 1337;
inline uint32_t fr() {
    rst ^= rst << 13;
    rst ^= rst >> 17;
    rst ^= rst << 5;
    return rst;
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(0); cin.tie(0);
    int tl = 295;
    if (argc > 1) tl = atoi(argv[1]);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<long long> s(n + 1);
    for (int i = 1; i <= n; ++i) cin >> s[i];
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<double> beta(n + 1);
    for (int i = 1; i <= n; ++i) beta[i] = (double)s[i] / (g[i].size() + 1.0);
    vector<int> ord(n + 1), in_F(n + 1, 0), F_n, vs(n + 1, 0), bfs, par(n + 1, 0), ex(n + 1, 0), it(n + 1, 0), bt(n + 1, 0), cc(n + 1, 0);
    vector<double> rs(n + 1);
    vector<long long> dp0(n + 1, 0), dp1(n + 1, 0);
    F_n.reserve(n + 1); bfs.reserve(n + 1);
    long long bs = -1, cs;
    auto t0 = chrono::steady_clock::now();
    int tk = 0, iters = 0;
    while (true) {
        if ((++iters & 15) == 0) {
            auto now = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::seconds>(now - t0).count() >= tl) break;
        }
        for (int i = 1; i <= n; ++i) {
            rs[i] = beta[i] * (0.8 + 0.4 * (fr() / (double)UINT32_MAX));
            ord[i] = i;
        }
        sort(ord.begin() + 1, ord.end(), [&](int a, int b) { return rs[a] > rs[b]; });
        for (int u : F_n) in_F[u] = 0;
        F_n.clear();
        for (int i = 1; i <= n; ++i) {
            int u = ord[i], c = 0;
            for (int v : g[u]) if (in_F[v] && ++c > 1) break;
            if (c <= 1) { in_F[u] = 1; F_n.push_back(u); }
        }
        tk++; bfs.clear();
        for (int i : F_n) {
            if (vs[i] != tk) {
                int st = bfs.size();
                bfs.push_back(i); vs[i] = tk; par[i] = 0;
                for (int p = st; p < (int)bfs.size(); ++p) {
                    int u = bfs[p];
                    for (int v : g[u]) {
                        if (in_F[v] && vs[v] != tk) {
                            vs[v] = tk; par[v] = u; bfs.push_back(v);
                        }
                    }
                }
            }
        }
        for (int i = (int)bfs.size() - 1; i >= 0; --i) {
            int u = bfs[i];
            dp0[u] = 0; dp1[u] = s[u];
            for (int v : g[u]) {
                if (in_F[v] && par[v] == u) {
                    dp0[u] += max(dp0[v], dp1[v]);
                    dp1[u] += dp0[v];
                }
            }
        }
        for (int u : F_n) ex[u] = 0;
        fill(it.begin(), it.end(), 0);
        fill(cc.begin(), cc.end(), 0);
        cs = 0;
        for (int u : bfs) {
            if (!ex[u] && dp1[u] > dp0[u]) {
                it[u] = 1; cs += s[u];
                for (int v : g[u]) {
                    cc[v]++;
                    if (in_F[v] && par[v] == u) ex[v] = 1;
                }
            }
        }
        for (int i = 1; i <= n; ++i) {
            if (!it[i] && cc[i] == 0) {
                it[i] = 1; cs += s[i];
                for (int v : g[i]) cc[v]++;
            }
        }
        if (cs > bs) { bs = cs; bt = it; }
    }
    cout << bs << "\n";
    bool f = true;
    for (int i = 1; i <= n; ++i) {
        if (bt[i]) {
            if (!f) cout << " ";
            cout << i;
            f = false;
        }
    }
    cout << "\n";
    return 0;
}
