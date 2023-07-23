#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

int N;
vector<int> initial;
vector<pair<int, pair<int, int>>> moves;
vector<pair<int, pair<int, int>>> best_moves;

int heuristic(const vector<int>& p) {
    int res = 0;
    for (int k = 0; k < N - 1; k++) {
        if (!((p[k] + 1 == p[k + 1]) || (p[k + 1] + 1 == p[k]))) {
            res++;
        }
    }
    return (res + 1) / 2;
}

bool ida_star(vector<int>& state, int depth, int max_depth) {
    int h = heuristic(state);
    int f = depth + h;

    if (f > max_depth) {
        return false;
    }

    if (h == 0) {
        best_moves = moves;
        return true;
    }

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int prev_h = h;
            for (int s = 1; s <= j - i; s++) {
                rotate(state.begin() + i, state.begin() + i + s, state.begin() + j + 1);
                h = heuristic(state);
                moves.push_back({i + 1, {j + 1, s}});

                if (ida_star(state, depth + 1, max_depth)) {
                    return true;
                }

                moves.pop_back();
                rotate(state.begin() + i, state.begin() + j + 1 - s, state.begin() + j + 1);
            }
            h = prev_h;
        }
    }

    return false;
}

int main() {
    int a;
    char c;
    while (cin >> a) {
        initial.push_back(a);
        c = cin.get();
        if (c == '\n') {
            break;
        }
    }
    N = initial.size();

    int max_depth = heuristic(initial);
    while (!ida_star(initial, 0, max_depth)) {
        max_depth++;
    }

    cout << best_moves.size() << endl;
    for (const auto& move : best_moves) {
        cout << move.first << " " << move.second.first << " " << move.second.second << endl;
    }

    return 0;
}
