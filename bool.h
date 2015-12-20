/* Sudoku Solver
   bool.h
   Sept 4, 2015
   Chris Grass

   just a simple typedef
*/

#ifndef BOOL_TYPE
#define BOOL_TYPE

/* NOTE: Out of the two possible bool definitions below, which is "better"? */

/*
#define true 1
#define false 0
*/
typedef enum {
  false = 0,
  true
} bool;

#endif
