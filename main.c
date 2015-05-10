/* Sudoku Solver
  main.c
  May 10, 2015

  by Chris Grass and Jordon McKoy

  Will populate a sudoku board partially, and then solve it.
  This will print the initial board "problem" and then print the solved board (or simply say that there is no solution if there is none)
*/

#include "sudoku.h"

int main(int argc, char *argv[]) {
  FILE* input;
  SBoard *board;
  bool completed;
  int i;
  char input_values[15];

  //Create the board
  board = createBoard(9);

  input = fopen("input.sudoku", "rb");
  
  for (i = 0; i < 9; i++) {
    fgets(input_values, 15, input);
    fillPartial(board,input_values,i);
  }

  fclose(input);

  printBoard(board);

  initialFindAndPrune(board);
  printBoard(board);

  setEmptyCells(board);

  completed = solveBoard(board);

  if (completed) {
    printf("The sudoku was successfully solved.\n");
    printBoard(board);
  } else {
    printBoard(board);
    printf("The sudoku board was unsolvable.\n");
  }

  destroyBoard(board);
  return 0;
}

