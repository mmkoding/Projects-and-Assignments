#include "submain.h"

int main() {

	// Piece-1
	StraightEdge edge1(1111);
	InwardsEdge edge2(2222);
	InwardsEdge edge3(3333);
	OutwardsEdge edge4(4444);
	Edge* edges1[4] = {&edge1, &edge2, &edge3, &edge4};

	Piece* piece1 = new Piece(edges1);

	// Piece-2
	InwardsEdge edge5(5555);
	OutwardsEdge edge6(6666);
	OutwardsEdge edge7(7777);
	InwardsEdge edge8(8888);
	Edge* edges2[4] = {&edge5, &edge6, &edge7, &edge8};

	Piece* piece2 = new Piece(edges2);

	// Piece-3
	StraightEdge edge9(9999);
	InwardsEdge edge10(1010);
	OutwardsEdge edge11(1011);
	StraightEdge edge12(1012);
	Edge* edges3[4] = {&edge9, &edge10, &edge11, &edge12};

	Piece* piece3 = new Piece(edges3);


	Puzzle* puzzle = new Puzzle(8, 0, 0);
	int location1[2] = {6, 1};
	puzzle->placePiece(*piece1, location1);
	
	int location2[2] = {4, 5};
	puzzle->placePiece(*piece2, location2);
	
	int location3[2] = {5, 1};
	puzzle->placePiece(*piece3, location3);

	cout << *puzzle << endl;

	try {
		int location[2] = {4, 4};
		const Piece& piece = puzzle->getPiece(location);
		cout << "The following piece has been found:" << endl;
		cout << piece << endl;
	}
	catch(exception& e) {
		cout << e.what() << endl;
	}

	cout << endl << *puzzle << endl;


	delete piece1;
	delete piece2;
	delete piece3;
	delete puzzle;

	return 0;
}