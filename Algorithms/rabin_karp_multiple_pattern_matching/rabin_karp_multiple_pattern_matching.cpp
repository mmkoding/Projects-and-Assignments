#include "the8.h"

//DO NOT ADD OTHER LIBRARIES

using namespace std;

#define d 26
#define q 256

vector<int> the8(const string& text, const vector<string>& patterns){

    vector<int> shifts; //DO NOT CHANGE THIS

    /*****************/
    int M = patterns[0].length();
    int N = text.length();
    int i, j;
    int patternSize = (int)patterns.size();
    vector<long> p(patternSize, 0);
    long t = 0;
    long h = 1;
    std::vector<std::vector<string> > patternList(q, std::vector<string>());

    for (i = 0; i < M - 1; i++)
        h = (h * d);
    h = h % q;

    for (i = 0; i < M; i++){
        t = (d*t + text[i]) % q;
        for(j = 0; j < patternSize; j++){
            p[j] = (d * p[j] + patterns[j][i]) % q;
        }
        
    }

    for(int j = 0; j < patternSize; j++){
        patternList[p[j]].push_back(patterns[j]);
    }
    
 
    for (i = 0; i <= N - M; i++){
        if (patternList[t].size()){
            for(unsigned int k = 0; k < patternList[t].size(); k++){
                for (j = 0; j < M; j++){
                    if (text[i+j] != patternList[t][k][j]){
                        break;
                    }
                }
                if(j == M){
                    shifts.push_back(i);
                }
            }
        }
        if ( i < N-M ){
            t = (d*(t - text[i]*h) + text[i+M])%q;
            
            if (t < 0)
                t = (t + q);
        }
    }
    /*****************/

    return shifts; //DO NOT CHANGE THIS
}