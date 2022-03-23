#include <iostream>
#include "Solver.h"

int main(){
	Solver *sudoku = new Solver();
	sudoku->printTable();
	sudoku->Solve();
	std::cout << std::endl;
	sudoku->printTable();
}