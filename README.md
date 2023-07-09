# Tilepaint Puzzle SAT Solver

Tilepaint (Tairupeinto) is a logic pencil-and-paper puzzle that has proven NP-Complete. The verification and solver related to the Tilepaint puzzles are in this repository. We implemented the solver using SAT Solver. For more information about the puzzle, see [https://www.nikoli.co.jp/en/puzzles/tilepaint/](https://www.nikoli.co.jp/en/puzzles/tilepaint/).

## Test Cases
There are 250 test cases in the TC folder taken from [https://www.janko.at/Raetsel/Tairupeinto/index.htm](https://www.janko.at/Raetsel/Tairupeinto/index.htm). Each test case is numbered from `1` to `250` where a file with extension \*.in is the input and \*\_sol.txt is the expected answer of such number cases.
There are 3 kinds of instance sizes, i.e. `$10 \times 10$; $12 \times 12$; and $15 \times 15$`.

## Contents

This repository includes the following:

* **Tilepaint Solver**: The solver is implemented in C++ and use the solver MiniSAT. You can find the code in the `codes/tilepaint-solver.cpp` file. Additionally, the compiled executable is also included.
* **Test Cases**: `250` test cases of Tilepaint puzzles in plain text format. You can find them in the `TC` directory.
* **Experimental Data**: Experimental data regarding the runtime of the solver to solve the aformentioned test cases. You can find the data in the `experimental/Tilepaint SAT execution time.xlsx` file.

## How to use

Here are the steps to use the program that you need to follow:

1. Install MiniSAT and build the solver source code. You can skip this step if you just use the executable we provided.
2. Run the resulting executable file.
3. Input the Juosan puzzle according to the [Input/Output Format](#input) section.

## Solver
The verifier code is written in C++ and can be found in the `tilepaint_BF.cpp` (exhausted search) and `tilepaint_BT.cpp` (prune-and-search).
Both of them receive the same input format.
However, the exhausted search output is a set of a number of colored tiles, and the prune-and-search output is a set of a number of colored tiles with also the configuration denoted by an $m \times n$ of `1` or `0`.

### Input
```
m n
cc_1 cc_2 ... cc_n
cr_1 cr_2 ... cr_m
T_{1,1} T_{1,2} ... T_{1,n}
T_{2,1} T_{2,2} ... T_{2,n}
...
T_{m,1} T_{m,2} ... T_{m,n}
```
- m and n respectively are row and column size of the instance.
- cc_{j} representing the constraint column of j. (if there is no constraint in column j, then cc_j = -1).
- cr_{i} representing the constraint row of i. (if there is no constraint in row i, then cr_i = -1).
- T_{i, j} is an integer denoting the cell (i, j) where it belongs to. Each tile denoted by a unique number.

### Output
- The given input has a solution

```
C_{1,1} C_{1,2} ... C_{1,n}
C_{2,1} C_{2,2} ... C_{2,n}
...
C_{m,1} C_{m,2} ... C_{m,n}
Solution : 
T_1 T_2 ... T_p
Execution Time: {execution time} ms
```

- Otherwise
```
No Solution
```

## Example
```
10 10
7 7 7 4 3 4 4 8 6 3
9 7 4 4 6 3 1 5 6 8
1 6 6 6 13 13 19 19 19 27 
1 1 1 10 13 13 20 19 19 27 
1 1 7 10 10 13 20 23 23 25
2 2 7 11 11 13 20 23 25 25 
3 3 7 7 11 16 21 21 21 21
3 3 7 12 12 16 21 21 26 26 
4 4 4 12 12 17 17 22 26 26
5 5 8 8 14 14 22 22 22 26
5 5 8 9 15 15 15 24 24 24 
5 5 9 9 9 18 18 24 24 24
```
```

1 1 1 1 1 1 1 1 1 0 
1 1 1 0 1 1 0 1 1 0 
1 1 1 0 0 1 0 0 0 0 
1 1 1 0 0 1 0 0 0 0 
0 0 1 1 0 0 1 1 1 1 
0 0 1 0 0 0 1 1 0 0 
0 0 0 0 0 0 0 1 0 0 
1 1 0 0 0 0 1 1 1 0 
1 1 0 1 0 0 0 1 1 1 
1 1 1 1 1 0 0 1 1 1 
Solution : 
1 2 5 6 7 9 13 19 21 22 24 
Execution Time: 1ms.
```
