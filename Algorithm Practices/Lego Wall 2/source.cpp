#include <iostream>
#include <vector>
using namespace std;

const int MOD = 8191;

int mod_add(int x, int y) {
    return (x + y) % MOD;
}

int calculate(int width, int height, int D) {
    vector<vector<vector<int>>> s(height + 1, vector<vector<int>>(width + 1, vector<int>(D + 1, 0)));

    for (int i = 1; i <= height; ++i) {
		for (int j = 1; j <= width; ++j) {
			s[i][j][0] = 1;
		}
	}
	s[1][2][1] = 1;
    
    for (int i = 1; i <= height; ++i) {
		for (int j = 1; j <= width; ++j) {
			for (int k = 1; k <= D; ++k) {
				if (j == 1) {
					s[i][j][k] = mod_add(s[i][j][k], s[i-1][width][k]);
				} else if (j == 2) {
					s[i][j][k] = mod_add(s[i][j][k], mod_add(s[i-1][width][k-1], s[i][j-1][k]));
				} else if(j > 2) {
					s[i][j][k] = mod_add(s[i][j-1][k], mod_add(s[i][j-2][k-1], s[i][j][k]));
				}
			}
		}
	}


    return s[height][width][D] % MOD;
}

int main() {
	int width, height, D;
    cin >> width >> height >> D;

    int res = calculate(width, height, D);
    cout << res << endl;

    return 0;
}
