#include <iostream>

using namespace std;

#define MOD 8191

unsigned long long ways[100001];
unsigned long long totalWays[100001];
unsigned long long updatedWays[100001];

unsigned long long mod_pow(unsigned long long base, int exponent) {
    if (exponent == 0) return 1;

    unsigned long long temp = mod_pow(base, exponent / 2);
    temp = temp * temp;

    if (temp >= MOD) temp %= MOD;

    if (exponent & 1) {
        temp = temp * base;
        if (temp >= MOD) temp %= MOD;
    }

    return temp;
}

void precompute() {
    ways[1] = 1;
    ways[2] = 2;
    ways[3] = 4;

    for (int i = 4; i < 100001; i++) {
        ways[i] = ways[i - 1] + ways[i - 2] + ways[i - 3];
        if (ways[i] >= MOD)
            ways[i] %= MOD;
    }
}

unsigned long long calculate(int width, int height) {
    for (int i = 1; i <= width; i++) {
        totalWays[i] = mod_pow(ways[i], height);
    }

    for (int i = 1; i <= width; i++) {
        updatedWays[i] = totalWays[i];

        for (int k = 1; k < i; k++) {
            unsigned long long temp = updatedWays[k];
            temp = temp * totalWays[i - k];

            if (temp >= MOD)
                temp %= MOD;

            updatedWays[i] -= temp;

            if (updatedWays[i] >= MOD)
                updatedWays[i] += MOD;
        }
    }

    return updatedWays[width];
}

int main() {
    precompute();

    int width, height;
    cin >> width >> height;

    cout << calculate(width, height) << endl;
}
