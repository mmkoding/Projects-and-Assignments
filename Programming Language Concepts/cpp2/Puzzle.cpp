#include "Puzzle.h"


/****************************************************/
/****              *** PUZZLE ***                ****/

/* Contsructor : It takes three integers in the following order:
    The first argument is size of the puzzle which is 2^N for some N >= 0.
    The second argument is the initial row id of the puzzle.
    The third argument is the initial column id of the puzzle.
    
    After initialization with the above arguments, the necessary 
    pointers should be initialized to NULL.
    Note that the final row id in this puzzle is equal to: 
       initial_row_id + size - 1.
    Note that the final column id in this puzzle is equal to: 
       initial_column_id + size - 1.
       
    ATTENTION: If a sub-puzzle member does not include any Piece, namely, 
    if its piece is NULL, then do NOT construct it. In other words, do 
    NOT construct subpuzzles unless they are required. 
*/
Puzzle::Puzzle() {}

Puzzle::Puzzle(int _size, int row, int column) {
    // TODO
    first_row = row;
    first_column = column;
    size = _size;
    top_left = NULL;   
    top_right = NULL;  
    bottom_left = NULL;
    bottom_right = NULL;
}

/* Destructor  : It should destruct its sub-puzzles, yet it is NOT responsible
   of the destruction of its Piece member.
*/
Puzzle::~Puzzle() {
    // TODO
}

/* Copy constructor : It totally applies deep copy except the Piece
   member part. Do not deep copy Piece objects of the puzzle, instead
   do shallow copy for Pieces.
*/
Puzzle::Puzzle(const Puzzle& copyPuzzle) {
    // TODO
    //Puzzle* newPuzzle = new Puzzle();
    this->first_row = copyPuzzle.first_row;
    this->first_column = copyPuzzle.first_column;
    this->size = copyPuzzle.size;
    this->top_left = copyPuzzle.top_left;
    this->bottom_left = copyPuzzle.bottom_left;
    this->top_right = copyPuzzle.top_right;
    this->bottom_right = copyPuzzle.bottom_right;
}

/* Assignment operator : It totally applies shallow copy.
*/
Puzzle& Puzzle::operator=(const Puzzle& copyPuzzle) {
    // TODO
    this->first_row = copyPuzzle.first_row;
    this->first_column = copyPuzzle.first_column;
    this->size = copyPuzzle.size;
    this->top_left = copyPuzzle.top_left;
    this->bottom_left = copyPuzzle.bottom_left;
    this->top_right = copyPuzzle.top_right;
    this->bottom_right = copyPuzzle.bottom_right;
    return *this;
}

/* This method directly assigns (without copying) the given Piece object to
   the corresponding sub-puzzle (cell) whose indices given in the second 
   argument in array form as follows <row_id, column_id>.
   
    ATTENTION: If a sub-puzzle member does not include any Piece, namely, 
    if its piece is NULL, then do NOT construct it. In other words, do 
    NOT construct subpuzzles unless they are required.
*/
void Puzzle::placePiece(const Piece& newPiece, int indices[2]) {
    // TODO
    if(this->size == 1){
        piece = &newPiece;
        return;
    }
    if(indices[0] - this->first_row < size / 2 && indices[1] - this->first_column >= size / 2){
        if(this->top_right == NULL)
            this->top_right = new Puzzle(this->size/2, first_row, first_column + this->size/2);
        top_right->placePiece(newPiece, indices);
    }    
    else if(indices[0] - this->first_row < size / 2 && indices[1] - this->first_column < size / 2){
        if(this->top_left == NULL)
            this->top_left = new Puzzle(this->size/2, first_row, first_column);
        top_left->placePiece(newPiece, indices);
    }
    
    else if(indices[0] - this->first_row >= size / 2 && indices[1] - this->first_column >= size / 2){
        if(this->bottom_right == NULL)
            this->bottom_right = new Puzzle(this->size/2, first_row + this->size/2, first_column + this->size/2);
        bottom_right->placePiece(newPiece, indices);
    }
    else{
        if(this->bottom_left == NULL)
            this->bottom_left = new Puzzle(this->size/2, first_row + this->size/2, first_column);
        bottom_left->placePiece(newPiece, indices);
    }
        
    


}

/* It returns directly the piece object located in the sub-puzzle (cell)
   whose row id and column id are given in the first and second elements of 
   2D array as argument, respectively. If there is no piece in the requested 
   indices, then this method throws an EmptyCellRequest exception defined in 
   Exception.h file.
*/
const Piece& Puzzle::getPiece(int indices[2]) const {
    // TODO
    if(this->size == 1){
        return *piece;
    }
    if(indices[0] - this->first_row < size / 2 && indices[1] - this->first_column >= size / 2){
        if(this->top_right == NULL)
            throw EmptyCellRequest();
        top_right->getPiece(indices);
    }    
    else if(indices[0] - this->first_row < size / 2 && indices[1] - this->first_column < size / 2){
        if(this->top_left == NULL)
            throw EmptyCellRequest();
        top_left->getPiece(indices);
    }
    
    else if(indices[0] - this->first_row >= size / 2 && indices[1] - this->first_column >= size / 2){
        if(this->bottom_right == NULL)
            throw EmptyCellRequest();
        bottom_right->getPiece(indices);
    }
    else{
        if(this->bottom_left == NULL)
            throw EmptyCellRequest();
        bottom_left->getPiece(indices);
    }
}

const Piece* Puzzle::getPieces(int indices[2]) const {
    // TODO
    if(this->size == 1){
        return piece;
    }
    if(indices[0] - this->first_row < size / 2 && indices[1] - this->first_column >= size / 2){
        if(this->top_right == NULL)
            return NULL;
        top_right->getPieces(indices);
    }    
    else if(indices[0] - this->first_row < size / 2 && indices[1] - this->first_column < size / 2){
        if(this->top_left == NULL)
            return NULL;
        top_left->getPieces(indices);
    }
    
    else if(indices[0] - this->first_row >= size / 2 && indices[1] - this->first_column >= size / 2){
        if(this->bottom_right == NULL)
            return NULL;
        bottom_right->getPieces(indices);
    }
    else{
        if(this->bottom_left == NULL)
            return NULL;
        bottom_left->getPieces(indices);
    }
}

/* This method returns the sub-puzzle whose initial and final row-column ids 
   are given in the first and second arguments, respectively. The indices
   given in the arguments will be given as an exact correspondence to one of
   the sub-puzzles of the current puzzle object.
   
   ATTENTION: The sub-puzzle to be cropped may correspond to a NULL part of the
   Puzzle object. In that case, it is enough to you to create a new Puzzle 
   object that satisfies the asked properties and return it. OF course, do NOT 
   forget to do this operation by escaping from memory leaks!!!
*/
Puzzle Puzzle::crop(int from[2], int to[2]) const {
    // TODO
    Puzzle x;

    //for(int i = from[0]; i < to[0]; i++){
    //    for(int j = from[1]; j < to[1]; j++){
    //        int list[2] = {i,j};
    //        if(getPieces(list))
    //            x->placePiece(*getPieces(list), list);
    //        delete[] list;
    //    }
    //}
    //cout << x << endl;
    return x;
}

/****************************************************/
/*        THE BELOW PART WILL NOT BE GRADED.        */
/*      YOU CAN IMPLEMENT IF YOU WOULD LIKE TO.     */
/****************************************************/
/*
    This method puts (patches) the given puzzle onto the corresponding part
    of the current puzzle object. To which part it will be patched on could
    be determined by just checking the initial row-column indices and the size
    of the puzzle given in the argument. 
    Unlike crop() method, the puzzle given in the argument may not be an exact 
    correspondence of some sub-puzzle. It is ensured that the given argument will
    be located on some sub-space of the current puzzle, yet it may *partially* 
    overlap with some or more sub-puzzles (See the figure in the pdf).
    The patch operation requires deep-copy of the corresponding sub-puzzles
    whereas it requires shallow copy of the corresponding Piece objects.
    
    HINT: You may get benefit from placePiece() method and you may start your
    operation by clearing the corresponding parts of the current puzzle.
*/
void Puzzle::patch(const Puzzle&) {
    // TODO
}

