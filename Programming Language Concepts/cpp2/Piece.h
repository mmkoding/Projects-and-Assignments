#ifndef __PIECE_H__
#define __PIECE_H__

#include "Edge.h"

//////////////////////////////////////////////////////
// // // //   DO NOT CHANGE *THIS FILE!*   // // // //
//////////////////////////////////////////////////////
class Piece {

	Edge* edges[4];	// left bottom right top

	Piece();

    // DO NOT MODIFY NEITHER BELOW NOR ABOVE
public:
	Piece(Edge*[4]);
	~Piece();
	Piece(const Piece&);
	friend ostream& operator<< (ostream&, const Piece&);
};

#endif