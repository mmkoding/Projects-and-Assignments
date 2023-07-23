#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

struct Solution {
    int alpha;
    int beta;
    int gamma;
    int label;
};

class PartialMaxs {
private:
  vector<int> s;
  static size_t lsb(size_t i) {
    return i & (i ^ (i - 1));
  }

public:
  PartialMaxs(size_t n) : s(n + 1, 0) {}

  size_t size() const {
    return s.size() - 1;
  }

  void update(size_t k, int delta) {
    int themax = delta;
    for (size_t i = k; i <= size(); i += lsb(i)) {
      s[i] = max(s[i], themax);
    }
  }

  int query(size_t k) const {
    int themax = 0;
    for (size_t i = k; i > 0; i -= lsb(i)) {
      themax = max(s[i], themax);
    }
    return themax;
  }
};

// Sorting and ranking comparators
bool sortByAlpha(const Solution& a, const Solution& b) { return a.alpha > b.alpha; }
bool sortByBeta(const Solution& a, const Solution& b) { return a.beta > b.beta; }

int main() {
    int N; 
    cin >> N;
    vector<Solution> solutions(N);

    for(int i = 0; i < N; i++) {
        cin >> solutions[i].alpha >> solutions[i].beta >> solutions[i].gamma;
        solutions[i].label = i + 1;
    }

    // Rank space reduction for beta
    vector<Solution> alpha_sorted = solutions;
    vector<Solution> beta_sorted = solutions;
    sort(alpha_sorted.begin(), alpha_sorted.end(), sortByAlpha);
    sort(beta_sorted.begin(), beta_sorted.end(), sortByBeta);
    solutions = alpha_sorted;

    unordered_map<int, int> beta_to_rank;

    for(int i = 0; i < N; i++) {
        beta_to_rank[beta_sorted[i].beta] = i + 1;
    }

    // Initialize Fenwick tree
    PartialMaxs tree(N + 1);
	vector<int> result;
    // Find and print Pareto-optimal solutions
    for(unsigned int i = 0; i < solutions.size(); i++) {
        int gamma_max = tree.query(beta_to_rank[solutions[i].beta] - 1);
        if(gamma_max <= solutions[i].gamma) {
            result.push_back(solutions[i].label);
            tree.update(beta_to_rank[solutions[i].beta], solutions[i].gamma);
        }
    }

    // Sort and print the result
    sort(result.begin(), result.end());
    for (int i : result) {
        cout << i << "\n";
    }

    return 0;
}
