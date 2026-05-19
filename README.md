# 🚀 Maximum Weight Independent Set (MWIS) Optimizer

An advanced algorithmic engine designed to solve the NP-Hard **Maximum Weight Independent Set** problem on massive graphs (up to $N = 200,000$ vertices and edges) under a strict 295-second computation time limit. 

This repository documents the architectural shift from a heuristic-based Iterated Local Search (ILS) to an exact subproblem reduction paradigm using the **Adaptive Biased Forest Randomized (ABFR)** engine.

---

## 🧠 The Algorithm: ABFR Engine

Finding the perfect MWIS on a general graph is mathematically impossible within strict time limits. However, finding it on a **Forest** (a graph with zero cycles) can be solved exactly in linear time $O(V)$ using Dynamic Programming. 

The ABFR engine leverages this mathematical loophole by rapidly extracting massive, cycle-free forests from the main graph, solving them perfectly, and patching the remaining nodes.

### Execution Pipeline

```mermaid
graph TD
    A[Raw Graph Input] --> B[Calculate Heuristic Bias: S / deg + 1]
    B --> C{Are Weights Uniform?}
    C -- Yes --> D[Deterministic Sequential Sweeps]
    C -- No --> E[Apply Random Noise Multiplier]
    D --> F[Sort Permutation]
    E --> F
    F --> G[FR14 Forest Filter]
    G --> H[Extract Cycle-Free Sub-Forest F]
    H --> I[Exact Tree Dynamic Programming O(v)]
    I --> J[Greedy Patch on Remaining Vertices]
    J --> K[Update Global High Score]
    K --> L{Execution < 295 Seconds?}
    L -- Yes --> B
    L -- No --> M((Output Optimal Team))
    
    style I fill:#2ea44f,stroke:#22863a,stroke-width:2px,color:#fff
    style M fill:#0366d6,stroke:#005cc5,stroke-width:2px,color:#fff


```

---

## 📊 Benchmark Results

[cite_start]Across a rigorous 23-graph test suite featuring dense clusters, cycles, grids, and massive sparse topologies, the ABFR engine officially averages a higher score than the mathematically established baselines[cite: 36, 51].

| Metric | ILS Engine (Legacy) | ABFR Engine (Current) | Net Improvement |
| :--- | :--- | :--- | :--- |
| **Accepted (Exact Match)** | [cite_start]15 [cite: 38] | [cite_start]**15** [cite: 38] | - |
| **Above Target (New Best)** | [cite_start]3 [cite: 38] | [cite_start]**6** [cite: 38] | [cite_start]📈 **+100%** [cite: 38] |
| **Below Target (Suboptimal)** | [cite_start]5 [cite: 38] | [cite_start]**2** [cite: 38] | [cite_start]📉 **-60%** [cite: 38] |
| **Overall Success Rate** | [cite_start]78.2% [cite: 38] | [cite_start]**91.3%** [cite: 38] | [cite_start]🚀 **+13.1%** [cite: 38] |
| **Global Relative Yield** | [cite_start]99.99% [cite: 38] | [cite_start]**100.62%** [cite: 38] | [cite_start]🏆 **Crossed 100%** [cite: 38] |

### Topological Breakthroughs
* [cite_start]**Dense Graphs (+8.80% Yield):** Dense conflict webs paralyze local swap heuristics[cite: 41]. [cite_start]The FR14 filter effortlessly bypasses these dense clusters, isolating clean trees and allowing the exact DP to extract maximum weight[cite: 42].
* [cite_start]**Massive Sparse Graphs ($N=200,000$):** Replaced slow local swap propagation with linear $O(V)$ tree traversals, overcoming previous timeouts to set new global high scores[cite: 44].
* [cite_start]**Uniform Graph Stabilization:** Flat topologies with identical skill levels previously caused random tie-breakers to shatter the forest structure[cite: 45]. [cite_start]Sequential sweeps guarantee unbroken paths, allowing the Tree DP to extract 100% optimal scores[cite: 46].

---

## 🛠️ Testing & Automation Workflow

This repository utilizes custom PowerShell scripts to automate compilation, testing, and production generation. Both scripts automatically compile the engine with maximum optimizations (`g++ -O3 -std=c++17`).

### 1. Verification Harness (`run_tests.ps1`)
Used for rapid development and benchmarking. This script iterates through all `input_*.txt` files, runs the engine for the 295-second time limit, and cross-references the output against the target scores in `output_*.txt`. 
* Dynamically calculates accuracy percentages.
* Color-codes console output: **<span style="color:green">ACCEPTED</span>**, **<span style="color:cyan">ABOVE TARGET</span>**, or **<span style="color:red">BELOW TARGET</span>**.

### 2. Production Builder (`generate_outputs.ps1`)
Used for packaging the final results. This script runs the engine against all inputs and pipes the standard output directly into a clean `final_submission_outputs/` directory, while simultaneously logging the final scores to the console for review.

---

## 💻 Quick Start

**1. Clone the repository:**
```bash
git clone [https://github.com/yourusername/mwis-optimizer.git](https://github.com/yourusername/mwis-optimizer.git)
cd mwis-optimizer

```
## Run the Benchmark Suite:

```bash
./run_tests.ps1
```

## Generate Final Output Files::

```bash
./generate_outputs.ps1
```


