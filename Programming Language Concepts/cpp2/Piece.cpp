#include "Piece.h"


/****************************************************/
/****               *** PIECE ***                ****/

/* Contsructor : initializes <edges> of the object to the those given in the
   argument. The edges given in the argument array should be cloned first,
   and its clones should be reserved, not the edges themselves.
   Note that the ordering of the given edges will always be as follows:
   edges[0]: Left side edge, 
   edges[1]: Bottom side edge, 
   edges[2]: Right side edge, 
   edges[3]: Top side edge.
*/
Piece::Piece() {}


Piece::Piece(Edge* edgeList[4] ) {
    // TODO
    for(int i = 0; i < 4; i++){
        Edge* dummy = edgeList[i]->clone();
        edges[i] = dummy;
    }

}

/* Destructor  : It should destruct its edges too.
*/
Piece::~Piece() {
    // TODO
    for(int i = 0; i < 4; i++){
        if(edges[i] != NULL)  
            delete edges[i];
    }
}

/* Copy constructor : Apply deep copy.
*/
Piece::Piece(const Piece& piece) {
    // TODO
    Piece* newPiece = new Piece();

    for(int i = 0; i < 4; i++)
        if(piece.edges[i])
            newPiece->edges[i] = piece.edges[i];
        
}

// This is already implemented for you, do NOT change it!
ostream& operator<< (ostream& os, const Piece& piece) {

	for (int i = 0; i < 4; i++)
		os << piece.edges[i]->getId() << "  \n";
	return os;
}
