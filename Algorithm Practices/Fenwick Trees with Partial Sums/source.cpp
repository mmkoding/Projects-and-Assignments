#include<bits/stdc++.h>
using namespace std;

struct student {
    int a, b, rank, id;
};

class PartialSums {
    private:
        vector<int> s;
        static size_t lsb(size_t i){
            return i & (i ^ (i - 1));
        }
    public:
        PartialSums(size_t n) : s(n + 1, 0){
        }

        size_t size() const{
            return s.size() - 1;
        }
        
        void update(size_t k){   
            for (size_t i = k; i <= size(); i = i + lsb(i))
                s[i]++;
        }
        
        int query(size_t k) const{
            int sum = 0;
            for (size_t i = k; i > 0; i = i - lsb(i))
                sum += s[i];
            return sum;
        }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    vector<student> students(N + 1);
    set<int> sorted_set;
    for(int i = 1; i <= N; ++i) {
        students[i].id = i;
        cin >> students[i].a >> students[i].b;
        sorted_set.insert(students[i].b);
    }

    unordered_map<int, int> score_to_index;
    set<int>::iterator itr = sorted_set.begin();
    int j = 1;
    for (; itr != sorted_set.end(); itr++)
        score_to_index[*itr] = j++;

    sort(students.begin() + 1, students.end(), [](const student& a, const student& b) {
        return a.a == b.a ? a.b > b.b : a.a > b.a;
    });

    PartialSums ps(sorted_set.size());
    vector<int> results(N + 1);
    for(int i = 1; i <= N; i++){
        results[students[i].id] = i - ps.query(score_to_index[students[i].b] - 1);
        ps.update(score_to_index[students[i].b]);
    }

    for(int i = 1; i <= N; i++)
        cout << results[i] << "\n";
	
    return 0;
}
