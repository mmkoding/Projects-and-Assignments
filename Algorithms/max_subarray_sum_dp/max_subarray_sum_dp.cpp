#include "max_subarray_sum_dp.h"

int getMax(int x, int y) { return x>y ? x : y;}


int recursive_sln(int i, int*& arr, int &number_of_calls){ //direct recursive
    number_of_calls+=1;

    //your code here

    int x,y;
    if(i == 0) return (arr[0]);
    if(i == 1) return (getMax(arr[0], arr[1]));
    if(i == 2) return (getMax(getMax(arr[0], arr[1]), arr[2]));
    x = recursive_sln(i - 3, arr, number_of_calls) + arr[i];
    y = recursive_sln(i - 1, arr, number_of_calls);


    return getMax(x, y); // this is a dummy return value. YOU SHOULD CHANGE THIS!
}



int memoization_sln(int i, int*& arr, int*& mem){ //memoization

    int x,y;
    if(i < 3){
        mem[0] = arr[0];
        mem[1] = getMax(arr[0], arr[1]);
        mem[2] = getMax(getMax(arr[0], arr[1]), arr[2]);
    }
    if(i>=3){
        if((mem[i - 3] == -1))
            x = memoization_sln(i-3, arr, mem) + arr[i];
        else
            x = mem[i-3] + arr[i];
    
        if((mem[i - 1] == -1))
            y = memoization_sln(i-1, arr, mem);
        else
            y = mem[i-1];

        mem[i] = getMax(x,y);
    }

    return mem[i];
}




int dp_sln(int size, int*& arr, int*& mem){ //dynamic programming

    mem[0] = getMax(0, arr[0]);
    int i = 1;

    while (i < 3) {
        mem[i] = getMax(mem[i - 1], arr[i]);
        i++;
    }
    i = 3;
 
    while (i < size) {
        mem[i] = getMax(mem[i - 1], arr[i] + mem[i - 3]);
        i++;
    }
    return mem[size - 1];
}


