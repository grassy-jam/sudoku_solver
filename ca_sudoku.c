#include <stdio.h>

int X = 9;
int Y = 9;
typedef struct Board {
  int B[X*Y] = {1,2,3,4,0,0,0,0,9,4,0,0,0,8,0,0,2,0,0,0,0,1,0,0,4,0,6,0,0,0,0,6,0,0,9,0,5,0,7,0,0,0,0,0,4,0,0,1,0,0,0,0,0,0,0,8,9,0,0,6,0,0,0,0,0,0,0,5,0,1,0,0,0,0,0,0,0};
} Board;



int cellIndex(int x, int y) {
  return y + x * X;
}
void printBoard(Board b) {
  int i, j;
  for (i = 0; i < X; i++) {
    for (j = 0; j < Y; j++) {
      printf("%d", b->B[cellIndex(x, y)]);
    }
    printf("\n");
  }
}
int *getCellNeighbourValues(int index, Board b) {
  //
}
int cellIsValid(int index, Board b) {
  int n_neighbours;
  int *neighbours;
  int cell;
  int *B = b->B;
  n_neighbours = 20;
  //neighbours will get memory allocated, we are responsible to free
  neighbours = getCellNeighbourValues(index, b);
  cell = getCellValue(index, B);
  for (i = 0; i < n_neighbours; i++) {
    if (cell != -1 && cell != 0) {
      // A cell is valid if it does not contain the same val as any of its neighbours
      if (cell == neighbours[i]) {
        //return false; this cell is not valid
        return 0;
      }
    }
  }

  // return true; this cell is valid
  return 1;
}
//Checks each cell and makes sure none of its states are in conflict with its neighbours
// Returns 1 if there are no errors/conflicts
// Returns 0 if board is not valid
int boardIsValid(Board b) {
  int i, j;
  for (i = 0; i < X; i++) {
    for (j = 0; j < Y; j++) {
      if (!cellIsValid(cellIndex(x, y), b->B)) {
        return 0;
      }
    }
  }
  return 1;
}
//Checks each cell and makes sure that its state is between 1..9
//Additionally, it will check if the board is valid
//If both of these conditions are met, return 1 (true)
//Otherwise, return 0
int boardIsSolved(Board b) {
  int i, j;
  int cell;
  int B = b->B;
  for (i = 0; i < X; i++) {
    for (j = 0; j < Y; j++) {
      cell = getCell(cellIndex(i, j), B);
      if (!(cell >= 1 && cell <= 9)) {
        //Return 0; The board is not solved
        return 0;
      }
    }
  }
}
void updateBoard(Board b) {
  printf("Updating board\n");
}
void applyUpdateBoard(Board b) {
  printf("Applying cell updates\n");
}
int main(void) {
  int i, j;
  printBoard(B);
  if (boardIsValid(B)) {
    while (!boardIsSolved(B)) {
      updateBoard(B);
      applyUpdateBoard(B);
      break;
    }
    printf("Board solved...\n");
    printBoard(B);
  } else {
    fprintf(stderr, "Error: %s\n", "Board is invalid");
    return -1;
  }

  return 0;
}
