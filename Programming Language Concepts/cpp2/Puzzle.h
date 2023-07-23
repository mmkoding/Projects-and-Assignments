#ifndef __PUZZLE_H__
#define __PUZZLE_H__

#include "Edge.h"
#include "Piece.h"
#include "Exception.h"

//////////////////////////////////////////////////////
//  DO NOT CHANGE *SPECIFIED PARTS* OF THIS FILE!   // 
//////////////////////////////////////////////////////
class Puzzle {

    /**/ /**************************/ /**/
	/**/ Puzzle* top_left;            /**/
	/**/ Puzzle* top_right;           /**/
	/**/ Puzzle* bottom_left;         /**/
	/**/ Puzzle* bottom_right;        /**/
	/**/ int size;                    /**/
	/**/ int first_row, first_column; /**/
	/**/ const Piece* piece;          /**/
	/**/ /**************************/ /**/
    // DO NOT MODIFY ABOVE

    // YOU CAN DEFINE YOUR OWN VARIABLES, METHODS, DATA STRUCTURES, etc. HERE!
    /// ...
protected:
	Puzzle();
	const Piece* getPieces(int[2]) const;
    

    // DO NOT MODIFY BELOW
public:
	Puzzle(int, int, int);
	~Puzzle();
	Puzzle(const Puzzle&);
	Puzzle& operator=(const Puzzle&);
	void placePiece(const Piece&, int[2]);
	const Piece& getPiece(int[2]) const;
	Puzzle crop(int[2], int[2]) const ;
	void patch(const Puzzle&);
	friend ostream& operator<< (ostream&, const Puzzle&);

};

#endif