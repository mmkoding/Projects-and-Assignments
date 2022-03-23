#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <vector>

class Solver{

private:
	std::pair<int, int> findUnfilled();
	bool isValid(int givenNumber, std::pair<int,int>  position);

public:
	void printTable();
	bool Solve();


};

#endif