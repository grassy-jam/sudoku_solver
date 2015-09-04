/* Sudoku Solver
  April 2, 2015

  by Chris Grass and Jordon McKoy

  Will populate a sudoku board partially, and then solve it.
  This will print the initial board "problem" and then print the solved board (or simply say that there is no solution if there is none)

  example usage:
    > gcc sudoku.c -o sudokuSolver
    > ./sudokuSolver

*/

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"

#include "arrayHelpers.h"
#include "cell.h"
#include "cellPriorityQueue.h"
#include "decisionNode.h"
#include "decisionTree.h"
#include "sudokuBoard.h"

/* DATA STRUCTURES
   - Cell
   - CellPriorityQueue
   - SudokuBoard
   - DecisionNode
   - DecisionTree
*/

#endif
