#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int perm_size = 0;


int countLines(const std::string& str) {
    int lines = 0;
    for (char c : str) {
        if (c == '\n') {
            lines++;
        }
    }
    return lines;
}

int heuristic_small(const vector<int>& perm) {
    int n = perm.size() - 1;
    int t = 0;
    for (int i = 1; i <= n; i++) {
        if (abs(perm[i] - perm[i-1]) != 1) t++;
    }
    return (t + 1) / 2;
}

int heuristic_large(const vector<int>& perm) {
    int n = perm.size() - 1;
    int t = 0;
    for (int i = 0; i < n; i++) {
        if (abs(perm[i] - perm[i+1]) != 1) {
            t++;
            i++;
        }
    }
    return t;
}


void do_move(vector<int>& perm, int i, int j) {
    reverse(perm.begin() + i, perm.begin() + j + 1);
}


int ida_star(vector<int>& perm, int depth, int bound, string& moves) {
    int h;
    if(perm_size>12)
        h = heuristic_large(perm);
    else
        h = heuristic_small(perm);
    if (depth + h > bound) return depth + h;
    if (h == 0) return -1; // goal state reached
    int min_cost = INT_MAX;
    for (unsigned int i = 1; i < perm.size(); i++) {
        for (unsigned int j = i+1; j < perm.size(); j++) {
            vector<int> tmp = perm;
            do_move(tmp, i, j);
			string new_move = to_string(i) + " " + to_string(j);
			moves += new_move + "\n";
			int cost = ida_star(tmp, depth+1, bound, moves);
			if (cost == -1) return -1; // goal state reached
			moves.erase(moves.size() - new_move.size() - 1); // backtrack
			min_cost = min(min_cost, cost);
        }
    }
    return min_cost;
}

int main() {
	int a;
	char c;
	vector<int> perm;
	perm.push_back(0);
	while(cin>>a){ 
		perm.push_back(a);
		c = cin.get();
		if(c == '\n')
			break;
	}
	perm_size = perm.size();

	
	string moves = "";
	int bound;
	if(perm_size>12)
        bound = heuristic_large(perm);
    else
        bound = heuristic_small(perm);
	
    while (true) {
        int cost = ida_star(perm, 0, bound, moves);
        if (cost == -1) break; // goal state reached
        bound = cost;
    }
	cout << countLines(moves) << endl;
    cout << moves;
    return 0;
}