#include "Solver.h"

std::vector<std::vector<int> > table = {{0,8,0,9,1,0,0,0,0},
    									{0,4,0,0,0,0,5,0,8},
    									{0,3,0,0,2,0,4,0,0},
    									{0,0,0,0,0,0,2,1,6},
    									{5,0,0,0,0,0,0,0,0},
    									{0,0,0,8,0,9,0,0,0},
    									{0,1,9,0,0,0,0,0,4},
    									{0,0,4,0,0,0,0,8,3},
    									{0,0,8,1,0,3,0,7,0}};



std::pair<int, int> Solver::findUnfilled(){
	int column = table[0].size();
	int row = table.size();
	std::pair<int, int> res = {-1, -1};

	for(int i = 0; i < row; i++){
		for(int j = 0; j < column; j++){
			if(table[i][j] == 0){
				res = {i,j};
				return res;			
			}	
		}
	}

	return res;
}

bool Solver::isValid(int givenNumber, std::pair<int,int> position){
	int columnSize = table[0].size();
	int rowSize = table.size();

	for(int j = 0; j < columnSize; j++){ //traverse the row and check if there is a same number
		if(table[position.first][j] == givenNumber && position.second != j)
			return false;
	}

	for(int i = 0; i < rowSize; i++){ //traverse the column and check if there is a same number
		if(table[i][position.second] == givenNumber && position.first != i)
			return false;
	}

	std::pair<int,int> box = {(int)position.second/3, (int)position.first/3};
	int a = box.second*3;
	int b = box.first*3;
	
	for(int i = a; i < a + 3; i++){ //check the box if there is a duplicate
		for(int j = b; j < b + 3; j++){
			if(table[i][j] == givenNumber && i != position.first && j != position.second)
				return false;
		}
	}
	

	return true;

}


void Solver::printTable(){
	int column = table[0].size();
	int row = table.size();

	for(int i = 0; i < row; i++){
		if(i % 3 == 0 && i != 0)
			std::cout << "----------------------" << std::endl;
		for(int j = 0; j < column; j++){
			if(j % 3 == 0 && j != 0)
				std::cout << "| ";
			if(table[i][j] == 0)
				std::cout << "□ ";
			else
				std::cout << table[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

bool Solver::Solve(){
	std::pair<int, int> test = findUnfilled();
	std::pair<int, int> val;
	if(test.first == -1 && test.second == -1){
			return true;
	}
	else
		val = test;

	for(int i = 1; i < 10; i++){
		if(isValid(i, val)){
			table[val.first][val.second] = i;
	
			if(Solve() == true)
				return true;
	
			table[val.first][val.second] = 0;
		}
	}

	return false;
}