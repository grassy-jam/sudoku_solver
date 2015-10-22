#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void usage();

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
    if (y % 3 == 0) {
      //printf("\n");
      printf("|---+---+---|\n");
    }
    for (x = 0; x < X; x++) {
      if (x == 0) {
        printf("|");
      }
      switch(b->B[cellIndex(x, y)]) {
        case ONE:
        case R_ONE:
          printf("1");
          break;
        case TWO:
        case R_TWO:
          printf("2");
          break;
        case THREE:
        case R_THREE:
          printf("3");
          break;
        case FOUR:
        case R_FOUR:
          printf("4");
          break;
        case FIVE:
        case R_FIVE:
          printf("5");
          break;
        case SIX:
        case R_SIX:
          printf("6");
          break;
        case SEVEN:
        case R_SEVEN:
          printf("7");
          break;
        case EIGHT:
        case R_EIGHT:
          printf("8");
          break;
        case NINE:
        case R_NINE:
          printf("9");
          break;
        case EMPTY:
          printf(".");
          break;
        case ERROR:
          printf("E");
          break;
        default:
          printf("?");
          break;
      }
      if (x % 3 == 2) {
        printf("|");
        continue;
      }

    }
    printf("\n");
    if (y == 8) {
      printf("|---+---+---|\n");
    }
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

//Returns a new array[9] with the count of all neighbour values for a given index
//TODO: Complete getCellNeighbourValues (need to finish SS neighbour finding)
Cell_state *Board_getCellNeighbourValues(int index, Board *b) {
  int n; //# of neighbours
  int *neighbourIndices;
  int curr;
  int x, y;
  int c, r;
  int *numFound; //number of 1..9 values found
  int i;

  n = 20; //there are 20 neighbours for any given cell in a 9x9 sudoku board
  //row neighbours 0..7
  //col neighbours 8..15
  //ss neighbours 16..19
  neighbourIndices = malloc(sizeof(int) * n);

  //we use curr to denote the current index we are looking for
  curr = 0;

  //Determine the 2D index from 1D index
  x = index % X;
  y = index / X;

  for (c = 0; c < x; c++) {
    neighbourIndices[curr++] = cellIndex(c, y);
  }
  for (c = x+1; c < X; c++) {
    neighbourIndices[curr++] = cellIndex(c, y);
  }
  //printf("curr should be at 8, curr is %d\n", curr);
  for (r = 0; r < y; r++) {
    neighbourIndices[curr++] = cellIndex(x, r);
  }
  for (r = y+1; r < Y; r++) {
    neighbourIndices[curr++] = cellIndex(x, r);
  }
  //printf("curr should be at 16, curr is %d\n", curr);
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
    neighbourIndices[curr++] = index + 10;
    neighbourIndices[curr++] = index + 11;
    neighbourIndices[curr++] = index + 19;
    neighbourIndices[curr++] = index + 20;
  } else if (index == 1 || index == 4 || index == 7 ||
      index == 28 || index == 31 || index == 34 ||
      index == 55 || index == 58 || index == 61) {
    //at index 1
    //check +8, +10, +17, +19
    neighbourIndices[curr++] = index + 8;
    neighbourIndices[curr++] = index + 10;
    neighbourIndices[curr++] = index + 17;
    neighbourIndices[curr++] = index + 19;
  } else if (index == 2 || index == 5 || index == 8 ||
      index == 29 || index == 31 || index == 35 ||
      index == 56 || index == 59 || index == 62) {
    //at index 2
    //check +7, +8, +16, +17
    neighbourIndices[curr++] = index + 7;
    neighbourIndices[curr++] = index + 8;
    neighbourIndices[curr++] = index + 16;
    neighbourIndices[curr++] = index + 17;
  } else if (index == 9 || index == 12 || index == 15 ||
      index == 36 || index == 39 || index == 42 ||
      index == 63 || index == 66 || index == 69) {
    //at index 3
    //check -8, -7, +10, +11
    neighbourIndices[curr++] = index - 8;
    neighbourIndices[curr++] = index - 7;
    neighbourIndices[curr++] = index + 10;
    neighbourIndices[curr++] = index + 11;
  } else if (index == 10 || index == 13 || index == 16 ||
      index == 37 || index == 40 || index == 43 ||
      index == 64 || index == 67 || index == 70) {
    //at index 4
    //check -10, -8, +8, +10
    neighbourIndices[curr++] = index - 10;
    neighbourIndices[curr++] = index - 8;
    neighbourIndices[curr++] = index + 8;
    neighbourIndices[curr++] = index + 10;
  } else if (index == 11 || index == 14 || index == 17 ||
      index == 38 || index == 41 || index == 44 ||
      index == 65 || index == 68 || index == 71) {
    //at index 5
    //check -11, -10, +7, +8
    neighbourIndices[curr++] = index - 11;
    neighbourIndices[curr++] = index - 10;
    neighbourIndices[curr++] = index + 7;
    neighbourIndices[curr++] = index + 8;
  } else if (index == 18 || index == 21 || index == 24 ||
      index == 45 || index == 48 || index == 51 ||
      index == 72 || index == 75 || index == 78) {
    //at index 6
    //check -17, -16, -8, -7
    neighbourIndices[curr++] = index - 17;
    neighbourIndices[curr++] = index - 16;
    neighbourIndices[curr++] = index - 8;
    neighbourIndices[curr++] = index - 7;
  } else if (index == 19 || index == 22 || index == 25 ||
      index == 46 || index == 49 || index == 52 ||
      index == 73 || index == 76 || index == 79) {
    //at index 7
    //check -19, -17, -10, -8
    neighbourIndices[curr++] = index - 19;
    neighbourIndices[curr++] = index - 17;
    neighbourIndices[curr++] = index - 10;
    neighbourIndices[curr++] = index - 8;
  } else if (index == 20 || index == 23 || index == 26 ||
      index == 47 || index == 50 || index == 53 ||
      index == 74 || index == 77 || index == 80) {
    //at index 8
    //check -20, -19, -11, -10
    neighbourIndices[curr++] = index - 20;
    neighbourIndices[curr++] = index - 19;
    neighbourIndices[curr++] = index - 11;
    neighbourIndices[curr++] = index - 10;
  }
  //printf("curr should be 20, curr is %d\n", curr);


  numFound = malloc(sizeof(int) * 9);
  for (i = 0; i < 9; i++) { numFound[i] = 0; }

  for (i = 0; i < n; i++) {
    //printf("A neighbour at index %d is %2d\n", neighbourIndices[i], b->B[neighbourIndices[i]]);
    switch(b->B[neighbourIndices[i]]) {
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

  free(neighbourIndices);

  return numFound;
}

//Returns true if the cell is valid based on its neighbour values,
// or false (0) otherwise
int cellIsValid(int index, Board *b) {
  //int n_neighbours;
  int *neighbours;
  Cell_state cell;
  int i;

  //neighbours will get memory allocated, we are responsible to free
  neighbours = Board_getCellNeighbourValues(index, b);

  /*
  printf("Cell %d or (%d, %d)'s neighbours: ", index, index % X, index / X);
  for (i = 0; i < X; i++) {
    if (neighbours[i] > 0)
      printf("%d: %d, ", i+1, neighbours[i]);
  }
  printf("\n");
  */

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
      //printf("Validating cell %d (%d, %d)\n", cellIndex(x, y), x, y);
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
  for (x = 0; x < X; x++) {
    for (y = 0; y < Y; y++) {
      if (cellIsValid(cellIndex(x, y), b)) {
        //The board is not solved
        return 0;
      }
      printf("Cell at index (%d, %d) is valid.\n", x, y);
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
            if (rand() % X > (X/2)) {
              state = ONE;
            }
          }
          break;
        case 1:
          if (state == EMPTY) {
            state = TWO;
          } else {
            //TODO: Gamble for state
            if (rand() % X > (X/2)) {
              state = TWO;
            }
          }
          break;
        case 2:
          if (state == EMPTY) {
            state = THREE;
          } else {
            //TODO: Gamble for state
            if (rand() % X > (X/2)) {
              state = THREE;
            }
          }
          break;
        case 3:
          if (state == EMPTY) {
            state = FOUR;
          } else {
            //TODO: Gamble for state
            if (rand() % X > (X/2)) {
              state = FOUR;
            }
          }
          break;
        case 4:
          if (state == EMPTY) {
            state = FIVE;
          } else {
            //TODO: Gamble for state
            if (rand() % X > (X/2)) {
              state = FIVE;
            }
          }
          break;
        case 5:
          if (state == EMPTY) {
            state = SIX;
          } else {
            //TODO: Gamble for state
            if (rand() % X > (X/2)) {
              state = SIX;
            }
          }
          break;
        case 6:
          if (state == EMPTY) {
            state = SEVEN;
          } else {
            //TODO: Gamble for state
            if (rand() % X > (X/2)) {
              state = SEVEN;
            }
          }
          break;
        case 7:
          if (state == EMPTY) {
            state = EIGHT;
          } else {
            //TODO: Gamble for state
            if (rand() % X > (X/2)) {
              state = EIGHT;
            }
          }
          break;
        case 8:
          if (state == EMPTY) {
            state = NINE;
          } else {
            //TODO: Gamble for state
            if (rand() % X > (X/2)) {
              state = NINE;
            }
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

  for (i = 0; i < X*Y; i++) {
    if (b->deltaB[i] != NO_CHANGE) {
      //Uncommenting the following printf with slow the program down a LOT
      //printf("Changing %d->%d at index %d\n", b->B[i], b->deltaB[i], i);
      b->B[i] = b->deltaB[i];
      // Reset the update val
      b->deltaB[i] = NO_CHANGE;
    }
  }
}

int main(int argc, char *argv[]) {
  int timestep, maxTimestep;
  Board *b;

  if (argc < 2) {
    usage(argv[0]);
    return -1;
  }

  sscanf(argv[1], "%d", &maxTimestep);
  //printf("timestep is %d\n", maxTimestep);
  if (maxTimestep < 1) {
    printf("Must specify a valid timestep!\n");
    return -1;
  }

  b = Board_create();
  //Board_print(b);

  if (Board_isValid(b)) {
    timestep = 0;
    srand(time(NULL));
    while (!Board_isSolved(b) && timestep < maxTimestep) {
      timestep++;
      //printf("==============\n   TIMESTEP  %2d\n==========\n", timestep);
      Board_update(b);
      //printf("Updating board\n");
      Board_applyUpdate(b);
      //printf("Applying cell updates\n");
      //Board_print(b);
    }
    //printf("Board solved...\n");
    if (timestep == maxTimestep) {
      printf("M %d ", timestep);
    } else {
      printf(". %d ", timestep);
    }
    if (Board_isSolved(b)) {
      printf("Solved.\n");
    } else {
      printf("Errors.\n");
    }
    //printf("%d Timesteps passed.\n", timestep);
    //Board_print(b);
  } else {
    fprintf(stderr, "Error: %s\n", "Board is invalid");
    Board_destroy(b);
    return -1;
  }

  Board_destroy(b);

  return 0;
}

void usage(char *exe) {
  printf("USAGE\n=====\n    %s n_timesteps\n", exe);
}
