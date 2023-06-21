#include <minisat/core/Solver.h>
#include <windows.h>
#include <bits/stdc++.h>
#include <chrono>
#include <unistd.h>
#include <vector>
#include <map>

using namespace std;
using Minisat::mkLit;
using Minisat::lbool;
using Minisat::Lit;
using namespace std::chrono;

int m, n;
vector<int> CC, CR;
vector<vector<int>> CG;
Minisat::Solver S;
unordered_map<int, vector< pair <int, int>> > R;
int maxR = 0;

int clauses = 0, literal = 0;

Minisat::Var h(int i, int j) {
    i--, j--;
    return i*n + j;
}

vector<vector<Lit>> atMost(int k, vector<Lit>& lits) {
    int len = lits.size();
    vector<vector<Lit>> formula;
    for (long long bit = 0; bit < (1ll << len); bit++) {
        if (__builtin_popcountll(bit) != k + 1) continue;
        vector<Lit> clause;
        for (int i = 0; i < len; i++)
            if (bit & (1ll << i)) 
                clause.push_back(~lits[i]);
        clauses++;
        formula.push_back(clause);
    }

    return formula;
}

vector<vector<Lit>> atLeast(int k, vector<Lit>& lits) {
    int len = lits.size();
    vector<Lit> neg = lits;
    for (auto& elm : neg) elm = ~elm;
    return atMost(len - k, neg);
}

void merge(vector<vector<Lit>> &a, vector<vector<Lit>> &b) {
  a.insert(a.end(), b.begin(), b.end());
}

vector<vector<Lit>> equals(int k, vector<Lit>& lits) {
    auto f = atLeast(k, lits), f2 = atMost(k, lits);
    merge(f, f2);
    return f;
}

void phi_1() {
    // for (int i = 1; i <= m; i++)
    //     for (int j = 1; j <= n; j++)
    //         for (int a = i; a <= m; a++) 
    //             for (int b = j + 1; b <= n; b++) {
    //                 if (CG[i][j] == CG[a][b]) {
    //                     S.addClause(~mkLit(h(i, j)), mkLit(h(a, b)));
    //                     S.addClause(mkLit(h(i, j)), ~mkLit(h(a, b)));
    //                     clauses += 2;
    //                 }
    //             }

    for (int k = 1; k <= maxR; k++)
        for (int i = 0; i < R[k].size(); i++) 
            for (int j = i+1; j < R[k].size(); j++) {
                S.addClause(~mkLit(h(R[k][i].first, R[k][i].second)), mkLit(h(R[k][j].first, R[k][j].second)));
                S.addClause(mkLit(h(R[k][i].first, R[k][i].second)), ~mkLit(h(R[k][j].first, R[k][j].second)));
                clauses += 2;
            }
}

void phi_2() {
    for (int i = 1; i <= m; i++) {
        if (CR[i] != -1) {
            vector<Lit> lits;
            for (int j = 1; j <= n; j++) {
                lits.push_back(mkLit(h(i, j)));
            }
            
            auto CREqual = equals(CR[i], lits);
            for (auto& c : CREqual) {
                Minisat::vec<Lit> clause;
                for (auto lit : c) clause.push(lit);
                S.addClause(clause);
            }

        }
    }
}

void phi_3() {
    for (int i = 1; i <= n; i++) {
        if (CC[i] != -1) {
            vector<Lit> lits;
            for (int j = 1; j <= m; j++) {
                lits.push_back(mkLit(h(j, i)));
            }

            auto CREqual = equals(CC[i], lits);
            for (auto& c : CREqual) {
                Minisat::vec<Lit> clause;
                for (auto lit : c) clause.push(lit);
                S.addClause(clause);
            }
        }
    }
}

void readInput() {
    cin >> m >> n;
    CG.resize(m+1, vector<int>(n+1));
    CC.resize(n+1);
    CR.resize(m+1);

    for (int i = 1; i <= n; i++)
        cin >> CC[i];

    for (int i = 1; i <= m; i++)
        cin >> CR[i];

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) {
            cin >> CG[i][j];
            maxR = max(maxR, CG[i][j]);
            R[CG[i][j]].push_back({i, j});
        }
        
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            static_cast<void>(S.newVar());
}

void printSolution() {
    HANDLE  hConsole;	
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    set<int> s;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) {
            if (S.modelValue(h(i, j)).isTrue()) {
                s.insert(CG[i][j]);
                SetConsoleTextAttribute(hConsole, CG[i][j] % 10 + 1), cout << 1 << " \n"[j == n];
            }
            else
                SetConsoleTextAttribute(hConsole, CG[i][j] % 10 + 1), cout << 0 << " \n"[j == n];
        }

    cout << "Solution : ";
    for (int item : s) cout << item << " ";
}

int main() {
    // ios_base::sync_with_stdio(false);cin.tie(NULL);
    readInput();
    
    auto start = high_resolution_clock::now();
    
    phi_1();
    phi_2();
    phi_3();

    cout << endl;
    cout << "Predicted:" << endl;
    cout << "Clauses: " << clauses << " Literals: " << literal << endl; 
    cout << "Actual:" << endl;
    cout << "Clauses: " << S.nClauses() << " Literals: " << S.nVars() << endl; 
    
    S.solve();
    auto end = high_resolution_clock::now();chrono::steady_clock::now();
    
    printSolution();

    cout << "\nExecution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;
}