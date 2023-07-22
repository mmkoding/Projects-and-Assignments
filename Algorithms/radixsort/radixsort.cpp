#include <iostream>
#include <climits>
#include <cmath>
#include <string>


int indexFinder(std::string word){
	int length = word.size();
	int index = 0, x = length - 1;

	while(x >= 0){
		char yanChar;
		yanChar = word.at(x);
		index += (int(yanChar)-65)*pow(26, length-1-x);
		x--;
	}

	return index;
}



int CountingSort(std::string arr[], bool ascending, int n, int s, int l, int k, const int size){

	int result = 0;


	std::string *B = new std::string[n];
	int* C = new int[k];


	for(int i = 0; i < n; i++){
		std::string portion, portion2;
		if(size-s-1 >= 0 && size-l-s >= 0 && size-l-s < 13 && size-s-1 < 13){
			
			for(int j = 0; j < l; j++){
				portion.push_back(arr[i][size-s-1-j]);
			}
			for(int w = 0; w < l; w++){
				portion2.push_back(portion[l-w-1]);
			}
			
		}
		

		int index = indexFinder(portion2);
		

		
		C[index] = C[index] + 1;

		result++;
	}

	for(int index = 1; index < k; index++){
		C[index] += C[index-1];

		result++;
	}

	if(ascending){
		for(int i = n - 1; i >= 0; i--){
			std::string portion, portion2;
			if(size-s-1 >= 0 && size-l-s >= 0 && size-l-s < 13 && size-s-1 < 13){
				
				for(int j = 0; j < l; j++){
					portion.push_back(arr[i][size-s-1-j]);
				}
				for(int w = 0; w < l; w++){
					portion2.push_back(portion[l-w-1]);
				}
				
			}
			

			int index = indexFinder(portion2);

			if(C[index] > 0){
				B[C[index] - 1] = arr[i];
				C[index]--;
			}
			result++;
		}
	}
	else{
		for(int i = 0; i < n; i++){
			std::string portion, portion2;
			if(size-s-1 >= 0 && size-l-s >= 0 && size-l-s < 13 && size-s-1 < 13){
				
				for(int j = 0; j < l; j++){
					portion.push_back(arr[i][size-s-1-j]);
				}

				for(int w = 0; w < l; w++){
					portion2.push_back(portion[l-w-1]);
				}
				
			}
			

			int index = indexFinder(portion2);


			if(C[index] > 0){
				B[n - C[index]] = arr[i];
				C[index]--;
			}

			result++;
		}
	}
	for(int i = 0; i < n; i++){
		arr[i] = B[i];
		result++;
	}

	delete[] B;

	result++;

	return result;

}

int radixSort(std::string arr[], bool ascending, int n, int l){
	int result = 0;
	int k = pow(26, l);
	int stringSize = int(arr[0].size());

	for(int s = 0; s < stringSize; s+=l){
		if(stringSize - s - l < 0)
			result += CountingSort(arr, ascending, n, stringSize - s, l, k, stringSize);
		result += CountingSort(arr, ascending, n, s, l, k, stringSize);
	}

	return result;
}

int main(){

	std::cout << "\n***********************************************************\n";
    int sizer = 10;
    std::string x[10] = {"NWLRBBMQBHCD", "ARZOWKKYHIDD", "QSCDXRJMOWFR", "XSJYBLDBEFSA", "RCBYNECDYGGX", "XPKLORELLNMP", "APQFWKHOPKMC", "OQHNWNKUEWHS", "QMGBBUQCLJJI", "VSWMDKQTBXIX"};
    int swap = 0;
    //double avg_dist = 0, max_dist = 0;

    swap = radixSort(x, true, sizer, 3);

    //printf("CLASSIC : Swap: %ld, Average Distance: %f, Maximum Distance: %f \n", swap, avg_dist, max_dist);
    std::cout << "RADIX SORT : Iteration count: " << swap << std::endl;
    
    for(int i = 0; i < sizer; i++){
        std::cout<<x[i] << ", ";
    }


    std::cout << "\n***********************************************************\n";


}
