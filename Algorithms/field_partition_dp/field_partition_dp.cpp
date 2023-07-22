#include "the5.h"
// do not add extra libraries here

/*
W: the width of the field
L: the length of the field
partitions: 2D Boolean array for partitions, if a partition of w x l exists then partitions[x][y] is true.
numberOfCalls/numberOfIterations: method specific control parameter
*/
int getMin(int x, int y) { return x<y ? x : y;}

int recursiveMethod(int W, int L, bool** partitions, int* numberOfCalls){

    int area1 = INT_MAX, area2 = INT_MAX, area3 = W*L;
    
    (*numberOfCalls)++;

    if(partitions[W][L])
        area3 = 0;
    else{
        for(int m = 1; m <= W/2; m++){
            area1 = getMin(area1, recursiveMethod(W - m, L, partitions, numberOfCalls) + recursiveMethod(m, L, partitions, numberOfCalls));
        }
        for(int n = 1; n <= L/2; n++){
            area2 = getMin(area2, recursiveMethod(W, L - n, partitions, numberOfCalls) + recursiveMethod(W, n, partitions, numberOfCalls));
        }
    }      

    return getMin(area3, getMin(area1, area2));
}

/*88888888888888888888888888888888888888888888888888888888888888888888888888888888888*/

int memoizationHelper(int W, int L, bool** partitions, int* numberOfCalls, int** mem){

    int area1 = INT_MAX, area2 = INT_MAX, area3 = W*L, area1p1, area1p2, area2p1, area2p2;
    
    (*numberOfCalls)++;

    if(partitions[W][L])
        area3 = 0;
    else{
        for(int m = 1; m <= W/2; m++){
            if(mem[W-m][L] == -1){
                mem[W-m][L] = memoizationHelper(W - m, L, partitions, numberOfCalls, mem);
                area1p1 = mem[W-m][L];
            }
            else{
                area1p1 = mem[W-m][L];
                (*numberOfCalls)++;
            }

            if(mem[m][L] == -1){
                mem[m][L] = memoizationHelper(m, L, partitions, numberOfCalls, mem);
                area1p2 = mem[m][L];
            }
            else{
                area1p2 = mem[m][L];
                (*numberOfCalls)++;
            }
            area1 = getMin(area1, area1p1 + area1p2);
        }
            
        for(int n = 1; n <= L/2; n++){
            if(mem[W][L-n] == -1){
                mem[W][L-n] = memoizationHelper(W, L - n, partitions, numberOfCalls, mem);
                area2p1 = mem[W][L-n];
            }
            else{
                area2p1 = mem[W][L-n];
                (*numberOfCalls)++;
            }

            if(mem[W][n] == -1){
                mem[W][n] = memoizationHelper(W, n, partitions, numberOfCalls, mem);
                area2p2 = mem[W][n];
            }
            else{
                area2p2 = mem[W][n];
                (*numberOfCalls)++;
            }
            area2 = getMin(area2, area2p1 + area2p2);
        }

    }      

    return getMin(area3, getMin(area1, area2));

}


int memoizationMethod(int W, int L, bool** partitions, int* numberOfCalls){

    int** mem = new int*[W+1];
    for(int i = 0; i < W+1; i++){
        mem[i] = new int[L+1];
        for (int j = 0; j < L+1; j++){
            mem[i][j] = -1;
        }
    }
    (*numberOfCalls)++;
    return memoizationHelper(W, L, partitions, numberOfCalls, mem); // this is a dummy return value. YOU SHOULD CHANGE THIS!
}


/*88888888888888888888888888888888888888888888888888888888888888888888888888888888888*/

int memHelper(int W, int L, bool** partitions, int* numberOfCalls, int** mem){

    int area1 = INT_MAX, area2 = INT_MAX, area3 = W*L, area1p1, area1p2, area2p1, area2p2;
    
    (*numberOfCalls)++;

    if(partitions[W][L]){
        area3 = 0;
    }
    else{
        for(int m = 1; m <= W/2; m++){
            if(mem[W-m][L] == -1){
                mem[W-m][L] = memHelper(W - m, L, partitions, numberOfCalls, mem);
                area1p1 = mem[W-m][L];
                (*numberOfCalls)++;
            }
            else{
                area1p1 = mem[W-m][L];
                (*numberOfCalls)++;
            }

            if(mem[m][L] == -1){
                mem[m][L] = memHelper(m, L, partitions, numberOfCalls, mem);
                area1p2 = mem[m][L];
                (*numberOfCalls)++;
            }
            else{
                area1p2 = mem[m][L];
                (*numberOfCalls)++;
            }
            area1 = getMin(area1, area1p1 + area1p2);
        }
            
        for(int n = 1; n <= L/2; n++){
            if(mem[W][L-n] == -1){
                mem[W][L-n] = memHelper(W, L - n, partitions, numberOfCalls, mem);
                area2p1 = mem[W][L-n];
                (*numberOfCalls)++;
            }
            else{
                area2p1 = mem[W][L-n];
                (*numberOfCalls)++;
            }

            if(mem[W][n] == -1){
                mem[W][n] = memHelper(W, n, partitions, numberOfCalls, mem);
                area2p2 = mem[W][n];
                (*numberOfCalls)++;
            }
            else{
                area2p2 = mem[W][n];
                (*numberOfCalls)++;
            }
            area2 = getMin(area2, area2p1 + area2p2);
        }

    }      

    return getMin( getMin(area1, area2), area3);

}


int bottomUpMethod(int W, int L, bool** partitions, int* numberOfIterations){
    int** mem = new int*[W+1];

    for(int k = 0; k < W+1; k++){
        mem[k] = new int[L+1];
        (*numberOfIterations)+=1;
    }
    
    /*
    for(int m = 1; m <= W; m++){
        for(int n = 1; n <= L; n++){
            if(partitions[m][n]){
                mem[m][n] = 0;
            }
            else
                mem[m][n] = memHelper(m, n, partitions, numberOfIterations, mem);
        }
        
    }
    for(int m = 1; m <= W/2; m++){
        area1 = getMin(area1, mem[W-m][L] + mem[m][L]);
        (*numberOfIterations)++;
    }
    for(int n = 1; n <= L/2; n++){
        area2 = getMin(area2, mem[W][L-n] + mem[W][n]);
        (*numberOfIterations)++;
    }

    return getMin(area1, area2);

    */
    int area1 = INT_MAX;
    int area2 = INT_MAX;
    int area3 = W*L;

    for(int i = 1; i < W+1; i++){
        (*numberOfIterations)+=1;
        for (int j = 1; j < L+1; j++){

            (*numberOfIterations)+=1;
            area1 = INT_MAX;
            area2 = INT_MAX;
            area3 = i*j;

            if(partitions[i][j]){
                area3 = 0;
                (*numberOfIterations)+=1;
            }
            else{
                for(int m = 1; m <= i/2; m++){
                    area1 = getMin(area1, mem[i-m][j] + mem[m][j]);
                    (*numberOfIterations)+=1;
                }
                for(int n = 1; n <= j/2; n++){
                    area2 = getMin(area2, mem[i][j-n] + mem[i][n]);
                    (*numberOfIterations)+=1;
                }
                
            }
            (*numberOfIterations)+=1;
            mem[i][j] = getMin(area3, getMin(area1, area2));
        }
        (*numberOfIterations)+=1;
    }


    return mem[W][L];
}



