#pragma once

#include <exception>
using namespace std;

//////////////////////////////////////////////////////
//             DO NOT CHANGE THIS FILE!             //
//////////////////////////////////////////////////////

class EmptyCellRequest : public exception {

    /* This exception class is used when the cell whose coordinates are given 
       is requested for its piece, yet the cell is empty.
    */

public:
    virtual const char* what() const throw() {
        return "The cell that you request is empty!";
    }
};