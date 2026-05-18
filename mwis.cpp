#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>

using namespace std;

uint64_t rng_state = 1337;
inline uint32_t fast_rand() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 7;
    rng_state ^= rng_state << 17;
    return rng_state;
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    int time_limit = 295;
    if (argc > 1) time_limit = atoi(argv[1]);
    
    auto t0 = chrono::steady_clock::now();
    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    vector<long long> s(n + 1);
    for (int i = 1; i <= n; ++i) cin >> s[i];
    
    vector<int> head(n + 2, 0);
    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].first >> edges[i].second;
        head[edges[i].first + 1]++;
        head[edges[i].second + 1]++;
    }
    for (int i = 1; i <= n; ++i) head[i + 1] += head[i];
    
    vector<int> to(m * 2);
    vector<int> cur_head = head;
    for (int i = 0; i < m; ++i) {
        int u = edges[i].first, v = edges[i].second;
        to[cur_head[u]++] = v;
        to[cur_head[v]++] = u;
    }
    
    vector<int> in(n + 1, 0), best_in(n + 1, 0), cc(n + 1, 0), cxor(n + 1, 0), aq;
    aq.reserve(n + 1);
    long long cur_score = 0, best_score = 0;
    
    auto add = [&](int u) {
        in[u] = 1;
        cur_score += s[u];
        for (int i = head[u]; i < head[u + 1]; ++i) {
            int v = to[i];
            cc[v]++;
            cxor[v] ^= u;
        }
    };
    
    auto rem = [&](int u) {
        in[u] = 0;
        cur_score -= s[u];
        for (int i = head[u]; i < head[u + 1]; ++i) {
            int v = to[i];
            cc[v]--;
            cxor[v] ^= u;
            if (!in[v] && cc[v] <= 1) aq.push_back(v);
        }
    };
    
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 1);
    vector<double> sc(n + 1);
    
    for (int i = 1; i <= n; ++i) sc[i] = (double)s[i] / (head[i + 1] - head[i] + 1);
    
    sort(ord.begin(), ord.end(), [&](int a, int b) { return sc[a] > sc[b]; });
    
    for (int i : ord) {
        if (cc[i] == 0) add(i);
    }
    
    best_score = cur_score;
    best_in = in;
    
    while (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - t0).count() < time_limit) {
        for (int i = 1; i <= n; ++i) {
            if (in[i] && (fast_rand() % 100 < 10)) rem(i);
        }
            
        for (int i = 1; i <= n; ++i) {
            if (!in[i] && cc[i] <= 1) aq.push_back(i);
        }
            
        while (!aq.empty()) {
            int v = aq.back();
            aq.pop_back();
            
            if (!in[v]) {
                if (cc[v] == 0) {
                    add(v);
                } else if (cc[v] == 1) {
                    int u = cxor[v];
                    if (s[v] > s[u]) {
                        rem(u);
                        add(v);
                    }
                }
            }
        }
        
        if (cur_score > best_score) {
            best_score = cur_score;
            best_in = in;
        } else {
            fill(in.begin(), in.end(), 0);
            fill(cc.begin(), cc.end(), 0);
            fill(cxor.begin(), cxor.end(), 0);
            cur_score = 0;
            for (int i = 1; i <= n; ++i) {
                if (best_in[i]) add(i);
            }
        }
    }
    
    cout << best_score << "\n";
    
    vector<int> ans;
    for (int i = 1; i <= n; ++i) {
        if (best_in[i]) ans.push_back(i);
    }
        
    for (size_t i = 0; i < ans.size(); ++i) {
        cout << ans[i] << (i + 1 == ans.size() ? "" : " ");
    }
    cout << "\n";
    
    bool valid = true;
    for (int u = 1; u <= n; ++u) {
        if (best_in[u]) {
            for (int i = head[u]; i < head[u + 1]; ++i) {
                if (best_in[to[i]]) {
                    valid = false;
                    break;
                }
            }
        }
        if (!valid) break;
    }
    
    if (!valid) {
        cout << "team not valid\n";
    }
    
    return 0;
}
