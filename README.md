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
    H --> I[Exact Tree Dynamic Programming O_v]
    I --> J[Greedy Patch on Remaining Vertices]
    J --> K[Update Global High Score]
    K --> L{Execution < 295 Seconds?}
    L -- Yes --> B
    L -- No --> M((Output Optimal Team))
    
    style I fill:#2ea44f,stroke:#22863a,stroke-width:2px,color:#fff
    style M fill:#0366d6,stroke:#005cc5,stroke-width:2px,color:#fff
