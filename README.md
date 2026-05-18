# [cite_start]Hackathon Squad [cite: 1]

## [cite_start]Abstract [cite: 2]
[cite_start]This report details the algorithmic progression in solving the Maximum Weight Independent Set (MWIS) problem on large-scale graphs ($N\le200,000$) under a strict 295-second time limit[cite: 3]. [cite_start]We compare two distinct meta-heuristic engines: the initial Iterated Local Search (ILS) approach and the advanced Adaptive Biased Forest Randomized (ABFR) engine[cite: 4]. [cite_start]The transition from blind heuristic approximation to exact subproblem reduction via Tree Dynamic Programming (DP) yielded a significant performance breakthrough, crossing the 100% global relative yield threshold across the benchmark dataset[cite: 5].

---

## [cite_start]1. Introduction [cite: 6]
[cite_start]The Maximum Weight Independent Set (MWIS) problem is a classic NP-Hard optimization challenge[cite: 7]. [cite_start]The objective is to find a set of mutually non-adjacent vertices in a weighted graph such that the sum of their weights is maximized[cite: 8]. [cite_start]Given the scale of the graphs (up to 200,000 vertices and edges) and the rigid computation time limit (295 seconds), exact global solvers are computationally infeasible[cite: 9]. [cite_start]Consequently, finding highly optimal solutions requires advanced meta-heuristics and fast local evaluations[cite: 10].

---

## [cite_start]2. Phase 1: Iterated Local Search (ILS) [cite: 11]
[cite_start]The initial architecture relied on an Iterated Local Search (ILS) framework, a standard approach in competitive programming for constraint satisfaction problems[cite: 12].

### [cite_start]2.1 Algorithm Pipeline [cite: 13]
* [cite_start]**Greedy Initialization:** Nodes were sorted using the heuristic ratio $\frac{S[i]}{deg(i)+1}$ and inserted into the independent set if no conflicts existed[cite: 14].
* [cite_start]**Local Search (1-swaps & 2-swaps):** The algorithm utilized an $O(1)$ conflict array (XOR-swap queue) to rapidly identify if swapping one node out and a better node in would improve the score[cite: 15].
* [cite_start]**Random Destruction:** To escape local optima, 10% of the current team was randomly dropped when the local search converged, forcing the algorithm to rebuild from a damaged state[cite: 16].

### [cite_start]2.2 Limitations [cite: 17]
[cite_start]While heavily optimized, the ILS engine suffered from the "Local Optima Trap"[cite: 18]. [cite_start]In dense graphs or massive sparse structures, 1-swaps and 2-swaps are insufficient to escape deep performance craters[cite: 19]. [cite_start]The algorithm spent the majority of its 5-minute window blindly guessing and patching, leading to suboptimal yields on complex test cases[cite: 20].

---

## [cite_start]3. Phase 2: Adaptive Biased Forest Randomized (ABFR) Engine [cite: 21]
[cite_start]To break the limitations of ILS, the architecture was completely overhauled[cite: 22]. [cite_start]The ABFR engine adapts the academic FR14 approximation algorithm into a high-performance execution loop, shifting the paradigm from heuristic approximation to exact subproblem reduction[cite: 23].

### [cite_start]3.1 The Mathematical Loophole [cite: 24]
[cite_start]While MWIS is NP-Hard on general graphs, it is solvable in exact linear time $O(V)$ on a Forest (a graph with no cycles) using Dynamic Programming[cite: 25]. [cite_start]The ABFR engine leverages this structural property[cite: 26].

### [cite_start]3.2 Algorithm Pipeline [cite: 27]
1. [cite_start]**Gumbel-max Permutation (Heuristic Bias):** Nodes are assigned a noisy score: $\beta_{i}= (\frac{S[i]}{deg(i)+1})\times noise(0.8,1.2)$[cite: 28]. [cite_start]The array is sorted descendingly[cite: 28].
2. [cite_start]**FR14 Forest Filter:** Iterating through the sorted nodes, a vertex u is added to a set F only if it connects to $\le 1$ node already in F[cite: 29]. [cite_start]This mathematically guarantees that the induced graph on F is a cycle-free forest[cite: 29, 30].
3. [cite_start]**Exact Tree DP:** A post-order traversal DP calculates the exact optimal MWIS for the extracted forest[cite: 31]. [cite_start]Two states are maintained for each node u[cite: 32]:
   * [cite_start]$DP[u][0]=\sum_{v\rightarrow\infty}max(DP[v][0],DP[v][1])$ [cite: 33] 
   * [cite_start]vechildren [cite: 34]
   * [cite_start]DP[u][1] = S[u] +  DP  [0] [cite: 35]
   * [cite_start]vechildren [cite: 36]
4. [cite_start]**Greedy Patch:** A final $O(1)$ sweep sweeps up any remaining vertices in the original graph that do not conflict with the optimal forest team[cite: 37].

---

## [cite_start]4. Comparative Results & Empirical Analysis [cite: 38]
[cite_start]The benchmark consisted of 23 test cases with varying topologies (dense, sparse, grid, cycle, star, and massive $N=200,000$ graphs)[cite: 39].

[cite_start]**Table 1: Performance Comparison: ILS vs. ABFR (23 Test Cases)** [cite: 40]

| Metric | ILS Engine | ABFR Engine | Improvement |
| :--- | :--- | :--- | :--- |
| [cite_start]**Accepted (Exact Match)** [cite: 41] | [cite_start]15 [cite: 41] | [cite_start]14 [cite: 41] | |
| [cite_start]**Above Target (New Best)** [cite: 41] | [cite_start]3 [cite: 41] | [cite_start]6 [cite: 41] | [cite_start]+100% [cite: 41] |
| [cite_start]**Below Target (Suboptimal)** [cite: 41] | [cite_start]5 [cite: 41] | [cite_start]3 [cite: 41] | [cite_start]-40% [cite: 41] |
| [cite_start]**Overall Success Rate** [cite: 41] | [cite_start]78.2% [cite: 41] | [cite_start]86.9% [cite: 41] | [cite_start]+8.7% [cite: 41] |
| [cite_start]**Global Relative Yield** [cite: 41] | [cite_start]99.99% [cite: 41] | [cite_start]100.44% [cite: 41] | [cite_start]Crossed 100% [cite: 41] |

### [cite_start]4.1 Topological Breakthroughs & Bottlenecks [cite: 43]
* [cite_start]**Dense Graphs (e.g., input_15, +8.80%):** Dense graphs paralyze local search heuristics[cite: 44]. [cite_start]The FR14 filter effortlessly bypassed these dense clusters, allowing the exact DP to pull nearly 9% more weight[cite: 45].
* [cite_start]**Cycle Graphs (e.g., input_05, +3.25%):** By randomly breaking a single edge to form a forest, the Tree DP instantly calculated the mathematically perfect independent set for the rest of the ring[cite: 46].
* [cite_start]**Massive Sparse Graphs (e.g., input_18, +0.07%):** ABFR successfully chewed through 200,000 nodes and edges to set a new global high score, overcoming previous ILS timeouts[cite: 47].
* **Bottleneck Uniform Weights (input_13, -4.00%):** When all skills equal 1, the heuristic bias $\frac{S[i]}{deg(i)+1}$ loses its sorting power[cite: 48]. [cite_start]The algorithm relies too heavily on uniform random noise, which accidentally breaks perfect alternating paths[cite: 49].
* [cite_start]**Bottleneck Time Constraints (input_16 & 17, -0.06% to -1.16%):** For massive sparse graphs (100,000+ nodes), the 295-second limit simply isn't long enough for the engine to randomly generate the specific forest configuration needed to unlock the final few nodes[cite: 50].

---

## 5. Conclusion [cite: 51]
The transition to the ABFR engine demonstrated that structural subproblem reduction vastly outperforms brute-force heuristic swapping[cite: 52]. By replacing randomized guessing with mathematically guaranteed optimal sub-teams (Tree DP), the ABFR engine eliminated local optima traps[cite: 53]. The algorithm crossed the 100% global relative yield threshold, meaning it now computes configurations that average higher than the pre-established mathematical baselines across the entire dataset[cite: 54].
