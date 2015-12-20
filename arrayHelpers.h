/* Sudoku Solver
   arrayHelpers.h
   Sept 4, 2015

   NOTE: Right now the array helper functions have a size of 9
   harcoded into them. We should change this.
*/

#include "bool.h"

/*
 Generic helpers
 */
//Returns a true the array contains the specificed num, and false otherwise
bool arrayContains(int* arrayOfSizeNine, int num);
bool isEmpty(int vals[9]);
//returns true if a value was found and removed from array
bool removeValFromArray(int *array, int n, int val);
