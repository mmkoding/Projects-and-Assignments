#include "Edge.h"
#include "Piece.h"
#include "Puzzle.h"

#include <iostream>
#include <sstream>

ostream& operator<< (ostream& os, const Puzzle& puzzle) {

	if (puzzle.size == 1) {
		if (puzzle.piece)
			os << *puzzle.piece << "\n";
		else
			os << "\\  /" << "\n" << " \\/ " << "\n" << " /\\ " << "\n" << "/  \\" << "\n" << "\n";
	}
	else {
		string subpart = "";
		ostringstream objOstream[4] = { ostringstream(""), ostringstream(""), ostringstream(""), ostringstream("") };
		Puzzle* subpuzzles[4] = { puzzle.top_left, puzzle.bottom_left, puzzle.top_right, puzzle.bottom_right };
		
		for (int s = 0; s < 4; s++) {	// top_left bottom_left top_right bottom_right
			if (subpuzzles[s])
				objOstream[s] << *subpuzzles[s];
			else {
				int subsize = puzzle.size / 2;
				for (int i = 0; i < subsize; i++) {
					for (int j = 0; j < subsize; j++)
						objOstream[s] << "\\  /" << "  ";
					objOstream[s] << "\n";
					for (int j = 0; j < subsize; j++)
						objOstream[s] << " \\/ " << "  ";
					objOstream[s] << "\n";
					for (int j = 0; j < subsize; j++)
						objOstream[s] << " /\\ " << "  ";
					objOstream[s] << "\n";
					for (int j = 0; j < subsize; j++)
						objOstream[s] << "/  \\" << "  ";
					objOstream[s] << "\n" << "\n";
				}
			}
		}

		stringstream ss_left, ss_right;
		ss_left << objOstream[0].str() << objOstream[1].str();	// top_left, bottom_left
		ss_right << objOstream[2].str() << objOstream[3].str();	// top_right, bottom_right
		string s_left = ss_left.str(), s_right = ss_right.str();
		
		for (int i = 0; i < (puzzle.size)*4 + (puzzle.size - 1); i++) {
			string subpart_left = s_left.substr(0, s_left.find("\n"));
			s_left = s_left.substr(s_left.find("\n") + 1);
			string subpart_right = s_right.substr(0, s_right.find("\n"));
			s_right = s_right.substr(s_right.find("\n") + 1);
			subpart += subpart_left + subpart_right + "\n";
		}

		os << subpart << "\n";
	}

	return os;
}