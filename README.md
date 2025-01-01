# General TTP

This code implements the construction algorithm $CONS(\sigma, l)$ described in the paper:

Jingyang Zhao, Mingyu Xiao, *A 5-approximation Algorithm for the Traveling Tournament Problem*. CoRR abs/2309.01902 (2023)

The $CONS(\sigma, l)$ algorithm can be used to heuristically solve the Traveling Tournament Problem (TTP-k).

It works for all feasible values of $n$ and $k$, i.e., for any even $n \geq 4$ and $k \geq 2$.

The source code is written in C++.

## Input

- $k \in [2, n-1]$: `the maximum number of consecutive away/away games`.
- $l \in [1, \min\\{n/2-1, k\\}]$: `the width of the rightmost block`.
- $\sigma$: `a permutation of the labels of all $n$ teams` (by default, $\sigma_i = i$ for each $i \in [1, n]$).

## Output

- A $n \times 2(n-1)$ table, where the $i$-th row corresponds to team $i$, the $j$-th column corresponds to the $j$-th day in the schedule. The entry $x_{i,j}$ (resp., $-x_{i,j}$) at position $(i, j)$ indicates that team $i$ plays an away (resp., home) game with team $x_{i,j}$.
