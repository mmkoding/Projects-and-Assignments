//#include "the2.h"
#include <stdio.h>

//You may write your own helper functions here

void swapper(unsigned short* a, unsigned short* b){

    unsigned short temp = *a;
    *a = *b;
    *b = temp;
}

int partition(unsigned short* arr, int start, int end, long &swap, double & avg_dist, double & max_dist){
    unsigned short pivot = arr[end];
    int i = start - 1;

    for (int j = start; j < end; j++){
        if(arr[j] >= pivot){
            i += 1;
            swapper(&arr[i], &arr[j]);
            swap++;

            if(j - i > max_dist)
                max_dist = j - i;
            if(swap == 1)
                avg_dist = j - i;
            else
                avg_dist = ((swap-1)*avg_dist + j-i) / swap;
        }
    }
    swapper(&arr[i+1], &arr[end]);
    swap++;
    if(end - (i+1) > max_dist)
        max_dist = end - (i+1);
    if(swap == 1)
        avg_dist = end - (i+1);
    else
        avg_dist = ((swap-1)*avg_dist + end - (i+1)) / swap;
    
    return i+1;
}

int floor_helping(int n){
    return n%2 == 1 ? n/2 + 1 : n/2; 
}


int hoarer(unsigned short* arr, int start, int end, long &swap, double & avg_dist, double & max_dist){ 
    
    int x = floor_helping(end - start + 1);
    x += start;
    unsigned short pivot = arr[x - 1];

    int i = start - 1;
    int j = end + 1;

    while(true){
        do{
           j-=1; 
        }while(arr[j] < pivot);

        do{
           i+=1; 
        }while(arr[i] > pivot);

        if(i >= j)
            return j;

        swap++;

        if(j - i > max_dist)
            max_dist = j - i;
        if(swap == 1)
            avg_dist = j - i;
        else
            avg_dist = ((swap-1)*avg_dist + j-i) / swap;

        swapper(&arr[i], &arr[j]);
    }
    
}


void quickSortHelper(unsigned short* arr, long &swap, double & avg_dist, double & max_dist, bool hoare, int size, int start, int end){

    if(!hoare){ //CLASSICAL PARTITION
        if(size > 1){
            int P;
            P = partition(arr, start, end, swap, avg_dist, max_dist);
            quickSortHelper(arr, swap, avg_dist, max_dist, hoare, P - start, start, P - 1);
            quickSortHelper(arr, swap, avg_dist, max_dist, hoare, end - P , P + 1, end);

        }
    }

    else{
        if(size > 1){
            int T;
            T = hoarer(arr, start, end, swap, avg_dist, max_dist);
            quickSortHelper(arr, swap, avg_dist, max_dist, hoare, T - start + 1, start, T);
            quickSortHelper(arr, swap, avg_dist, max_dist, hoare, end - T , T + 1, end);
        }

    }
    
}


void quickSort(unsigned short* arr, long &swap, double & avg_dist, double & max_dist, bool hoare, int size)
{
    //Your code here
    quickSortHelper(arr, swap, avg_dist, max_dist, hoare, size, 0, size - 1);
    
}

void threewaypartition(unsigned short* arr, int start, int end, long &swap, long &comparison,int& L,int& R){
    int i = start, j = start;
    int pivot = end;

    while(i < pivot){
        comparison+=1;
        if(arr[i]>arr[end]){
            swapper(&arr[i], &arr[j]);
            i+=1;
            j+=1;
            swap+=1;
        }
        else if(arr[i] == arr[end]){
            comparison+=1;
            pivot-=1;
            swap+=1;
            swapper(&arr[i],&arr[pivot]);
        }
        
        else{
            comparison+=1;
            i+=1;
        }

    }
    int m;
    
    (pivot-j < end-pivot+1) ? (m=pivot-j) : (m=end-pivot+1);

    for(int w = 0; w < m; w++){
        swap+=1;
        swapper(&arr[j+w], &arr[end+1-m+w]); //not sure
    }

    L = j;
    R = pivot - j; 
}


void quick3Helper(unsigned short* arr, long &swap, long &comparison, int start, int end){

    if(start >= end)
        return;
        
    int L, R;
    
    threewaypartition(arr, start, end, swap, comparison, L, R);
    quick3Helper(arr, swap, comparison, start, L-1);
    quick3Helper(arr, swap, comparison, end - R + 1, end);
    
}


void quickSort3(unsigned short *arr, long &swap, long &comparison, int size) {
    
    //Your code here
    quick3Helper(arr, swap, comparison, 0, size - 1);
    
}


int main(){

    printf("\n***********************************************************\n");
    int sizer = 8;
    unsigned short x[8] = {2, 1, 14, 6, 3, 0, 99, 3};
    long swap = 0;
    double avg_dist = 0, max_dist = 0;

    quickSort(x, swap, avg_dist, max_dist, false, sizer);

    printf("CLASSIC : Swap: %ld, Average Distance: %f, Maximum Distance: %f \n", swap, avg_dist, max_dist);

    for(int i = 0; i < sizer; i++){
        printf("%u, ", x[i]);
    }

    printf("\n***********************************************************\n");

    sizer = 8;
    unsigned short y[8] = {2, 1, 14, 6, 3, 0, 99, 3};
    swap = 0;
    avg_dist = 0; 
    max_dist = 0;

    quickSort(y, swap, avg_dist, max_dist, true, sizer);

    printf("HOARE : Swap: %ld, Average Distance: %f, Maximum Distance: %f \n", swap, avg_dist, max_dist);

    for(int j = 0; j < sizer; j++){
        printf("%u, ", y[j]);
    }

    printf("\n***********************************************************\n");

    sizer = 8;
    unsigned short z[8] = {2, 1, 14, 6, 3, 0, 99, 3};
    swap = 0;
    long comparison = 0;

    quickSort3(z, swap, comparison, sizer);

    printf("THREEWAY : Swap: %ld, Comparison: %ld \n", swap, comparison);

    for(int s = 0; s < sizer; s++){
        printf("%u, ", z[s]);
    }

    printf("\n***********************************************************\n");


}