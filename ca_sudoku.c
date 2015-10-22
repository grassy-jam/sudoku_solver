#include <stdio.h>
#include <stdlib.h>

#define X 9
#define Y 9
typedef struct Board {
  int *B; //Will be an |X*Y| array, where each value represents a tile on the board
  int *deltaB; //Will be the updates to be applied to the array
} Board;

typedef enum {
  NO_CHANGE = -2,
  ERROR = -1,
  EMPTY = 0,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  //R_* are read-only states (cannot change, used when initializing board)
  R_ONE,
  R_TWO,
  R_THREE,
  R_FOUR,
  R_FIVE,
  R_SIX,
  R_SEVEN,
  R_EIGHT,
  R_NINE
} Cell_state;

//Converts 2D index reference to a 1D index reference
int cellIndex(int x, int y) {
  return x + y * X;
}
//Creates a type of struct Board and initializes it
Board *Board_create() {
  Board *b;
  int i;
  // Use this to initialize the board
  int B[X*Y] = {
    1,2,3,4,0,0,0,0,9,
    4,0,0,0,8,0,0,2,0,
    0,0,0,1,0,0,4,0,6,
    0,0,0,0,6,0,0,9,0,
    5,0,7,0,0,0,0,0,4,
    0,0,1,0,0,0,0,0,0,
    0,8,9,0,0,6,0,0,0,
    0,0,0,0,5,0,1,0,0,
    0,0,0,0,0,0,0,0,0
  };

  b = malloc(sizeof(Board));
  b->B = malloc(sizeof(int) * X * Y);

  //Initialize board (either with input board or all 0's)
  for (i = 0; i < X*Y; i++) {
    switch(B[i]) {
      case 1:
        b->B[i] = R_ONE;
        break;
      case 2:
        b->B[i] = R_TWO;
        break;
      case 3:
        b->B[i] = R_THREE;
        break;
      case 4:
        b->B[i] = R_FOUR;
        break;
      case 5:
        b->B[i] = R_FIVE;
        break;
      case 6:
        b->B[i] = R_SIX;
        break;
      case 7:
        b->B[i] = R_SEVEN;
        break;
      case 8:
        b->B[i] = R_EIGHT;
        break;
      case 9:
        b->B[i] = R_NINE;
        break;
      case 0:
      default:
        b->B[i] = EMPTY;
        break;
    }
    b->B[i] = B[i];
  }

  b->deltaB = malloc(sizeof(int) * X * Y);

  for (i = 0; i < X*Y; i++) {
    b->deltaB[i] = NO_CHANGE;
  }

  return b;
}
//Destroys the given struct Board
void Board_destroy(Board *b) {
  if (b == NULL) return;
  free(b->B);
  free(b->deltaB);
  free(b);
}
void Board_print(Board *b) {
  int x, y;
  for (y = 0; y < Y; y++) {
    for (x = 0; x < X; x++) {
      printf("%d", b->B[cellIndex(x, y)]);
    }
    printf("\n");
  }
}
//Returns the value at the specified 1D index on the given board
Cell_state Board_getCellValue(int index, Board *b) {
  if (b == NULL) return -1;
  return b->B[index];
}

//Given the cell's 2D index, determine whether its state can be changed
// by checking for the read-only states
int Board_cellIsWritable(int index, Board *b) {
  Cell_state cell;
  cell = b->B[index];
  switch (cell) {
    case R_ONE:
    case R_TWO:
    case R_THREE:
    case R_FOUR:
    case R_FIVE:
    case R_SIX:
    case R_SEVEN:
    case R_EIGHT:
    case R_NINE:
      return 0;
      break;
    default:
      return 1;
      break;
  }
}

//Returns an array with the count of all neighbour values for a given index
//TODO: Complete getCellNeighbourValues (need to finish SS neighbour finding)
Cell_state *Board_getCellNeighbourValues(int index, Board *b) {
  int n; //# of neighbours
  int curr;
  Cell_state  R[X-1],
              C[Y-1],
              SS[X-1]; //X-1 or Y-1 could work
  Cell_state *neighbourValues;
  int numFound[X]; //use to count the # of 1..9 found in neighbours (indices 0..8 for values 1..9)
  int r, c, x, y;
  int nx, ny;
  int i;

  // Get 2D index x value from 1D index value
  x = index % X;
  // Get 2d index y value from 1D index value
  y = index / X;

  //Initialize numFound
  for (i = 0; i < X; i++) { numFound[i] = 0; }

  //Find the Row neighbours
  // neighbours 0..x
  for (c = 0; c < x; c++) {
    switch(b->B[cellIndex(c, y)]) {
      case ONE:
      case R_ONE:
        numFound[0]++; 
        break;
      case TWO:
      case R_TWO:
        numFound[1]++;
        break;
      case THREE:
      case R_THREE:
        numFound[2]++;
        break;
      case FOUR:
      case R_FOUR:
        numFound[3]++;
        break;
      case FIVE:
      case R_FIVE:
        numFound[4]++;
        break;
      case SIX:
      case R_SIX:
        numFound[5]++;
        break;
      case SEVEN:
      case R_SEVEN:
        numFound[6]++;
        break;
      case EIGHT:
      case R_EIGHT:
        numFound[7]++;
        break;
      case NINE:
      case R_NINE:
        numFound[8]++;
        break;
    }
  }
  // neighbours x+1..X-1
  for (c = x+1; c < X; c++) {
    switch(b->B[cellIndex(c, y)]) {
      case ONE:
      case R_ONE:
        numFound[0]++; 
        break;
      case TWO:
      case R_TWO:
        numFound[1]++;
        break;
      case THREE:
      case R_THREE:
        numFound[2]++;
        break;
      case FOUR:
      case R_FOUR:
        numFound[3]++;
        break;
      case FIVE:
      case R_FIVE:
        numFound[4]++;
        break;
      case SIX:
      case R_SIX:
        numFound[5]++;
        break;
      case SEVEN:
      case R_SEVEN:
        numFound[6]++;
        break;
      case EIGHT:
      case R_EIGHT:
        numFound[7]++;
        break;
      case NINE:
      case R_NINE:
        numFound[8]++;
        break;
    }
  }

  //Find the Col neighbours
  // neighbours 0..y
  for (r = 0; r < y; r++) {
    switch(b->B[cellIndex(x, r)]) {
      case ONE:
      case R_ONE:
        numFound[0]++; 
        break;
      case TWO:
      case R_TWO:
        numFound[1]++;
        break;
      case THREE:
      case R_THREE:
        numFound[2]++;
        break;
      case FOUR:
      case R_FOUR:
        numFound[3]++;
        break;
      case FIVE:
      case R_FIVE:
        numFound[4]++;
        break;
      case SIX:
      case R_SIX:
        numFound[5]++;
        break;
      case SEVEN:
      case R_SEVEN:
        numFound[6]++;
        break;
      case EIGHT:
      case R_EIGHT:
        numFound[7]++;
        break;
      case NINE:
      case R_NINE:
        numFound[8]++;
        break;
    }
  }
  // neighbours y+1..Y-1
  for (r = y+1; r < Y; r++) {
    switch(b->B[cellIndex(x, r)]) {
      case ONE:
      case R_ONE:
        numFound[0]++; 
        break;
      case TWO:
      case R_TWO:
        numFound[1]++;
        break;
      case THREE:
      case R_THREE:
        numFound[2]++;
        break;
      case FOUR:
      case R_FOUR:
        numFound[3]++;
        break;
      case FIVE:
      case R_FIVE:
        numFound[4]++;
        break;
      case SIX:
      case R_SIX:
        numFound[5]++;
        break;
      case SEVEN:
      case R_SEVEN:
        numFound[6]++;
        break;
      case EIGHT:
      case R_EIGHT:
        numFound[7]++;
        break;
      case NINE:
      case R_NINE:
        numFound[8]++;
        break;
    }
  }

  //TODO: Fix Find the SS neighbours
  /*
    We find the SS neighbours by determining which cell index this
    cell is at, relative to its own neighbours, e.g. 0..8 in
      0 1 2
      3 4 5
      6 7 8
    Given this index, we have the offsets for each of the neighbours
  */
  if (index == 0 || index == 3 || index == 6 ||
      index == 17 || index == 30 || index == 33 ||
      index == 54 || index == 57 || index == 60) {
    //at index 0
    //check +10, +11, +19, +20
  } else if (index == 1 || index == 4 || index == 7 ||
      index == 28 || index == 31 || index == 34 ||
      index == 55 || index == 58 || index == 61) {
    //at index 1
    //check +8, +10, +17, +19
  } else if (index == 2 || index == 5 || index == 8 ||
      index == 29 || index == 31 || index == 35 ||
      index == 56 || index == 59 || index == 62) {
    //at index 2
    //check +7, +8, +16, +17
  } else if (index == 9 || index == 12 || index == 15 ||
      index == 36 || index == 39 || index == 42 ||
      index == 63 || index == 66 || index == 69) {
    //at index 3
    //check -8, -7, +10, +11
  } else if (index == 10 || index == 13 || index == 16 ||
      index == 37 || index == 40 || index == 43 ||
      index == 64 || index == 67 || index == 70) {
    //at index 4
    //check -10, -8, +8, +10
  } else if (index == 11 || index == 14 || index == 17 ||
      index == 38 || index == 41 || index == 44 ||
      index == 65 || index == 68 || index == 71) {
    //at index 5
    //check -11, -10, +7, +8
  } else if (index == 18 || index == 21 || index == 24 ||
      index == 45 || index == 48 || index == 51 ||
      index == 72 || index == 75 || index == 78) {
    //at index 6
    //check -17, -16, -8, -7
  } else if (index == 19 || index == 22 || index == 25 ||
      index == 46 || index == 49 || index == 52 ||
      index == 73 || index == 76 || index == 79) {
    //at index 7
    //check -19, -17, -10, -8
  } else if (index == 20 || index == 23 || index == 26 ||
      index == 47 || index == 50 || index == 53 ||
      index == 74 || index == 77 || index == 80) {
    //at index 8
    //check -20, -19, -11, -10
  }
  /*
  //Find the SS neighbours
  curr = 0;
  if (x <= 2) {
    if (y <= 2) {
      //SS0
      for (nx = 0; nx < 3; nx++) {
        for (ny = 0; ny < 3; ny++) {
          if (x == nx && y == ny) {
            //skip this cell
          } else {
            SS[curr] = b->B[cellIndex(nx, ny)];
            curr++;
          }
        }
      }
    }
    else if (y <= 5) {
      //SS3
      for (nx = 0; nx < 3; nx++) {
        for (ny = 3; ny < 6; ny++) {
          if (x == nx && y == ny) {
            //skip this cell
          } else {
            SS[curr] = b->B[cellIndex(nx, ny)];
            curr++;
          }
        }
      }
    }
    else if (y <= 8) {
      //SS6
      for (nx = 0; nx < 3; nx++) {
        for (ny = 6; ny < 9; ny++) {
          if (x == nx && y == ny) {
            //skip this cell
          } else {
            SS[curr] = b->B[cellIndex(nx, ny)];
            curr++;
          }
        }
      }
    }
  } else if (x <= 5) {
    if (y <= 2) {
      //SS1
      for (nx = 3; nx < 6; nx++) {
        for (ny = 0; ny < 3; ny++) {
          if (x == nx && y == ny) {
            //skip this cell
          } else {
            SS[curr] = b->B[cellIndex(nx, ny)];
            curr++;
          }
        }
      }
    }
    else if (y <= 5) {
      //SS4
      for (nx = 3; nx < 6; nx++) {
        for (ny = 3; ny < 6; ny++) {
          if (x == nx && y == ny) {
            //skip this cell
          } else {
            SS[curr] = b->B[cellIndex(nx, ny)];
            curr++;
          }
        }
      }
    }
    else if (y <= 8) {
      //SS7
      for (nx = 3; nx < 6; nx++) {
        for (ny = 6; ny < 9; ny++) {
          if (x == nx && y == ny) {
            //skip this cell
          } else {
            SS[curr] = b->B[cellIndex(nx, ny)];
            curr++;
          }
        }
      }
    }
  } else if (x <= 8) {
    if (y <= 2) {
      //SS2
      for (nx = 6; nx < 9; nx++) {
        for (ny = 0; ny < 3; ny++) {
          if (x == nx && y == ny) {
            //skip this cell
          } else {
            SS[curr] = b->B[cellIndex(nx, ny)];
            curr++;
          }
        }
      }
    }
    else if (y <= 5) {
      //SS5
      for (nx = 6; nx < 9; nx++) {
        for (ny = 3; ny < 6; ny++) {
          if (x == nx && y == ny) {
            //skip this cell
          } else {
            SS[curr] = b->B[cellIndex(nx, ny)];
            curr++;
          }
        }
      }
    }
    else if (y <= 8) {
      //SS8
      for (nx = 6; nx < 9; nx++) {
        for (ny = 6; ny < 9; ny++) {
          if (x == nx && y == ny) {
            //skip this cell
          } else {
            SS[curr] = b->B[cellIndex(nx, ny)];
            curr++;
          }
        }
      }
    }
  }
  */
  
  n = 1;

  return numFound;
}

//Returns true if the cell is valid based on its neighbour values,
// or false (0) otherwise
int cellIsValid(int index, Board *b) {
  int n_neighbours;
  int *neighbours;
  Cell_state cell;
  int i;

  //neighbours will get memory allocated, we are responsible to free
  neighbours = Board_getCellNeighbourValues(index, b);

  printf("This cell's neighbours are...\n");
  for (i = 0; i < X; i++) {
    printf("%d: %d, ", i+1, neighbours[i]);
  }
  printf("\n");

  if (!Board_cellIsWritable(index, b)) {
    return 1; //assume that it is valid, we can't change it anyways
  }
  cell = Board_getCellValue(index, b);
  if (cell == ERROR) {
    return 0; //an erroneous cell is not valid
  }
  // We iterate over the 9 count values in neighbours
  for (i = 0; i < X; i++) {
    //i is the val, and neighbours[i] is the # of instances of i in this cell's neighbours
    if (neighbours[i] > 0) {
      // A cell is valid if it does not contain the same val as any of its neighbours
      switch (i) {
        case 0:
          if (cell == ONE || cell == R_ONE) return 0;
          break;
        case 1:
          if (cell == TWO || cell == R_TWO) return 0;
          break;
        case 2:
          if (cell == THREE || cell == R_THREE) return 0;
          break;
        case 3:
          if (cell == FOUR || cell == R_FOUR) return 0;
          break;
        case 4:
          if (cell == FIVE || cell == R_FIVE) return 0;
          break;
        case 5:
          if (cell == SIX || cell == R_SIX) return 0;
          break;
        case 6:
          if (cell == SEVEN || cell == R_SEVEN) return 0;
          break;
        case 7:
          if (cell == EIGHT || cell == R_EIGHT) return 0;
          break;
        case 8:
          if (cell == NINE || cell == R_EIGHT) return 0;
      }
    }
  }

  //Free the value of neighbours
  free(neighbours);

  // return true; this cell is valid
  return 1;
}

//Checks each cell and makes sure none of its states are in conflict with its neighbours
// Returns 1 if there are no errors/conflicts
// Returns 0 if board is not valid
int Board_isValid(Board *b) {
  int x, y;
  for (x = 0; x < X; x++) {
    for (y = 0; y < Y; y++) {
      printf("Validating cell %d (%d, %d)\n", cellIndex(x, y), x, y);
      if (!cellIsValid(cellIndex(x, y), b)) {
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
int Board_isSolved(Board *b) {
  int x, y;
  Cell_state cell;
  for (x = 0; x < X; x++) {
    for (y = 0; y < Y; y++) {
      if (cellIsValid(cellIndex(x, y), b)) {
        //The board is not solved
        return 0;
      }
    }
  }

  // The board is solved
  return 1;
}



//Given the cell's 1D index, amass a list of possible values and return a single one
//It goes through each available value and sets that as the state. If a state is already
// set, it gambles, and may either keep that state or set it to the current avail value.
//If no values are available, return ERROR Cell_state
Cell_state Board_cellAvailableValue(int index, Board *b) {
  int *neighbours;
  int i;
  Cell_state state;

  neighbours = Board_getCellNeighbourValues(index, b);

  state = EMPTY;
  for (i = 0; i < X; i++) {
    //If there are no instances of this value...
    if (neighbours[i] == 0) {
      switch (i) {
        case 0:
          if (state == EMPTY) {
            state = ONE;
          } else {
            //TODO: Gamble for state
          }
          break;
        case 1:
          if (state == EMPTY) {
            state = TWO;
          } else {
            //TODO: Gamble for state
          }
          break;
        case 2:
          if (state == EMPTY) {
            state = THREE;
          } else {
            //TODO: Gamble for state
          }
          break;
        case 3:
          if (state == EMPTY) {
            state = FOUR;
          } else {
            //TODO: Gamble for state
          }
          break;
        case 4:
          if (state == EMPTY) {
            state = FIVE;
          } else {
            //TODO: Gamble for state
          }
          break;
        case 5:
          if (state == EMPTY) {
            state = SIX;
          } else {
            //TODO: Gamble for state
          }
          break;
        case 6:
          if (state == EMPTY) {
            state = SEVEN;
          } else {
            //TODO: Gamble for state
          }
          break;
        case 7:
          if (state == EMPTY) {
            state = EIGHT;
          } else {
            //TODO: Gamble for state
          }
          break;
        case 8:
          if (state == EMPTY) {
            state = NINE;
          } else {
            //TODO: Gamble for state
          }
          break;
      }
    }
  }

  free(neighbours);

  if (state == EMPTY) {
    return ERROR;
  } else {
    return state;
  }
}

void Board_update(Board *b) {
  int x, y;
  Cell_state cell;

  printf("Updating board\n");
  
  for (x = 0; x < X; x++) {
    for (y = 0; y < Y; y++) {
      if (Board_cellIsWritable(cellIndex(x, y), b)) {
        cell = b->B[cellIndex(x, y)];
        if (cell == EMPTY || cell == ERROR) {
          b->deltaB[cellIndex(x, y)] = Board_cellAvailableValue(cellIndex(x, y), b);
        } else {
          //ensure this cell is valid
          if (!cellIsValid(cellIndex(x, y), b)) {
            b->deltaB[cellIndex(x, y)] = ERROR;
          }
        }
      }
    }
  }
}

void Board_applyUpdate(Board *b) {
  int i;

  printf("Applying cell updates\n");
  for (i = 0; i < X*Y; i++) {
    if (b->deltaB[i] != NO_CHANGE) {
      b->B[i] = b->deltaB[i];
      // Reset the update val
      b->deltaB[i] = NO_CHANGE;
    }
  }
}

int main(void) {
  int i, j;
  Board *b;

  b = Board_create();
  Board_print(b);

  if (Board_isValid(b)) {
    while (!Board_isSolved(b)) {
      Board_update(b);
      Board_applyUpdate(b);
      break;
    }
    printf("Board solved...\n");
    Board_print(b);
  } else {
    fprintf(stderr, "Error: %s\n", "Board is invalid");
    Board_destroy(b);
    return -1;
  }

  Board_destroy(b);

  return 0;
}
