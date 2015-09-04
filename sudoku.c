/* Sudoku Solver
  April 2, 2015

  by Chris Grass and Jordon McKoy

  Will populate a sudoku board partially, and then solve it.
  This will print the initial board "problem" and then print the solved board (or simply say that there is no solution if there is none)
*/

#include "sudoku.h"

void sortCellArrayByLength(Cell **cells);
