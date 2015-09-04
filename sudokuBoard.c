/* Sudoku Solver
   sudokuBoard.c
   Sept 4, 2015
*/

include "sudokuBoard.h"

/*
  Board manipulation functions
*/
SBoard *createBoard(int n) {
  SBoard *board;
  int i, j;
  board = malloc(sizeof(SBoard));
  board->size = n;
  board->board = malloc(sizeof(Cell **) * n);
  for (i = 0; i < n; i++) {
    board->board[i] = malloc(sizeof(Cell *) * n);
    //Create each cell
    for (j = 0; j < n; j++) {
      board->board[i][j] = createCell(0, i, j);
    }
  }
  board->emptyCells = createCellPQueue();
  return board;
}
void *destroyBoard(SBoard *board) {
  int i, j;
  for (i = 0; i < board->size; i++) {
    for (j =0; j < board->size; j++) {
      destroyCell(board->board[i][j]);
    }
    free(board->board[i]);
  }
  free(board->board);
  destroyCellPQueue(board->emptyCells);
  
  free(board);
}

bool rowHasOneMissingValue(SBoard *board, int row) {
  int i,
      counter = 0;
  for (i = 0; i < 9; i++) {
    if (getCellValue(board,row,i) == false) {
      counter++;
    }
    if (counter >= 2) {
      return false;
    }
  }
  return true;
}
bool colHasOneMissingValue(SBoard *board, int col) {
  int i,
      counter = 0;
  for (i = 0; i < 9; i++) {
    if (getCellValue(board,i,col) == false) {
      counter++;
    }
    if (counter >= 2) {
      return false;
    }
  }
  return true;
}
bool rowIsFull(SBoard *board, int row) {
  int i;
  bool full = true;
  for (i = 0; i < 9; i++) {
    if (getCellValue(board,row,i) == false) {
      full = false;
    }
  }
  return full;
}
bool colIsFull(SBoard *board, int col) {
  int i;
  bool full = true;
  for (i = 0; i < 9; i++) {
    if (getCellValue(board,i,col) == false) {
      full = false;
    }
  }
  return full;
}

int getCellValue(SBoard *board, int row, int col) {
  int val;
  val = board->board[row][col]->val;
  return val;
}
int *getRowValues(SBoard *board, int row) {
  static int vals[9];
  int col;
  for (col = 0; col < 9; col++) {
    vals[col] = getCellValue(board, row, col);
  }
  return vals;
}
int *getColValues(SBoard *board, int col) {
  static int vals[9];
  int row;
  for (row = 0; row < 9; row++) {
    vals[row] = getCellValue(board, row, col);
  }
  return vals;
}
int *getSubSquareValues(SBoard *board, int row, int col) {
  static int vals[9];
  int i, j, ssI, tlRow, tlCol, val,
  index = 0;
  
  ssI = getSSIndex(row, col);
  switch(ssI) {
    case 0:
      tlRow = 0;
      tlCol = 0;
      break;
    case 1:
      tlRow = 0;
      tlCol = 3;
      break;
    case 2:
      tlRow = 0;
      tlCol = 6;
      break;
    case 3:
      tlRow = 3;
      tlCol = 0;
      break;
    case 4:
      tlRow = 3;
      tlCol = 3;
      break;
    case 5:
      tlRow = 3;
      tlCol = 6;
      break;
    case 6:
      tlRow = 6;
      tlCol = 0;
      break;
    case 7:
      tlRow = 6;
      tlCol = 3;
      break;
    case 8:
      tlRow = 6;
      tlCol = 6;
      break;
  }
  
  //printf("tlrow: %d tlcol: %d for ssI: %d (orig: [%d][%d])\n", tlRow, tlCol, ssI, row, col);
  
  for(i = tlRow; i < (tlRow + 3); i++) {
    for(j = tlCol; j < (tlCol + 3); j++) {
      val = getCellValue(board, i, j);
      vals[index] = val;
      index++;
    }
  }
  return vals;
}

void setCellValue(SBoard *board, int row, int col, int val) {
  int ssI;
  board->board[row][col]->val = val;
  //Recompute the vectors relative to this cell
  setRowVector(board, row);
  setColVector(board, col);
  ssI = getSSIndex(row, col);
  setSubSquareVector(board, ssI);
}
void setRowValues(SBoard *board, int row, int rowVals[9]) {
  int col;
  for (col = 0; col < 9; col++) {
    setCellValue(board, row, col, rowVals[col]);
  }
}
void setColValues(SBoard *board, int col, int colVals[9]) {
  int row;
  for (row = 0; row < 9; row++) {
    setCellValue(board, row, col, colVals[row]);
  }
}
void setSubSquareValues(SBoard *board, int row, int col, int ssVals[9]) {
  int i, j,
  ss = 0;
  
  if (row <= 2) {
    row = 0;
  } else if (row <= 5) {
    row = 3;
  } else {
    row = 6;
  }
  
  if (col <= 2) {
    col = 0;
  } else if (col <= 5) {
    col = 3;
  } else {
    col = 6;
  }
  
  for(i = row; i < (row + 3); i++) {
    for(j = col; j < (col + 3); j++) {
      setCellValue(board, row, col, ssVals[ss]);
      ss++;
    }
  }
}

void crossCellVectors(SBoard *board, int row, int col) {
  Cell *cell;
  bool *rowVector, *colVector, *ssVector;
  int num, i;
  
  cell = board->board[row][col];
  rowVector = board->rowVectors[row];
  colVector = board->colVectors[col];
  ssVector = getSubSquareVector(board, row, col);
  
    
  for (num = 1; num <= 9; num++) {
    if (!(rowVector[num - 1] || colVector[num - 1] || ssVector[num - 1])) {
      //This is a possible value for this cell
      cell->length += 1;
      cell->possibleValues = realloc(cell->possibleValues, sizeof(int) * cell->length);
      
      cell->possibleValues[cell->length - 1] = num;
    } else {
      //If this is not possible value for this cell, remove it from possible values
      if (removeValFromArray(cell->possibleValues, cell->length, num)) {
        //if a value was removed, reduce the length of possible values by 1
        cell->length--;
      }
    }
  }
}
void resetPossibleCellValues(SBoard *board, int row, int col) {
  //  set length of possible values array to 0, the array will be realloced to 1 in
  //  crossCellvectors if a match is found
  board->board[row][col]->length = 0;
}
void setPossibleCellValues(SBoard *board) {
  int row, col;
  
  for (row = 0; row < board->size; row++) {
    for (col = 0; col < board->size; col++) {
      if (board->board[row][col]->val == 0) {
        // reset possible values
        resetPossibleCellValues(board, row, col);
        crossCellVectors(board, row, col);
      }
    }
  }
}
bool fillSingleChoiceCells(SBoard *board) {
  bool changes = false;
  int row, col;
  Cell *cell;
  
  for (row = 0; row < board->size; row++) {
    for (col = 0; col < board->size; col++) {
      cell = board->board[row][col];
      if (cell->val == 0 && cell->length == 1) {
        cell->val = cell->possibleValues[0];
        setCellValue(board, row, col, cell->possibleValues[0]);
        changes = true;
      }
    }
  }

  return changes;
}

void setEmptyCells(SBoard *board) {
  int row, col;
  
  for (row = 0; row < board->size; row++) {
    for (col = 0; col < board->size; col++) {
      if (board->board[row][col]->val == 0) {
        appendLastCell(board->emptyCells, board->board[row][col]);
      }
    }
  }
}
bool solveEmptyCells(SBoard *board, DTree *dTree) {
  CellPQueue *emptyCells;
  DNode *curr, *currParent;
  Cell *cell;
  int i;
  bool possibleMove, result;
  
  emptyCells = board->emptyCells;
  while (emptyCells->length > 0) {
  //  printf("There are %d empty cells\n", emptyCells->length);
    cell = removeFirstCell(emptyCells);

    setDTreeCurrToCell(dTree, cell);
  //  printf("The node cell is %p\n", dTree->curr->cell);
    if (dTree->curr != NULL && dTree->curr->cell != NULL) {
      curr = dTree->curr;
  //    printf("Going to solve for [%d][%d]\n", curr->cell->row, curr->cell->col);
      for (i = 0; i < curr->numBranches; i++) {
        possibleMove = checkPossibleMove(board, curr->cell, curr->branchVals[i]);
        if (!possibleMove) {
          continue;
        }
        setCellValue(board, curr->cell->row, curr->cell->col, curr->branchVals[i]);
        currParent = curr;
        setDTreeCurr(dTree, curr->branchChildren[i]);
        if (emptyCells->length > 0 && emptyCells->cells[0] != NULL) {
          //do stuff
          if (emptyCells->cells[0]->length != 1) {
            //We need to refactor the empty cells
            setPossibleCellValues(board);
            sortCellPQueueByLength(emptyCells);
          }
          result = solveEmptyCells(board, dTree);
          if (result == true) {
            //It was a correct path
            return true;
          } else {
            //We have no more paths, we need to backtrack
            //push curr's cell back onto
            while (curr != dTree->curr) {
              //Push dTree->curr's cell back onto emptyCells
              appendLastCell(emptyCells, dTree->curr->cell);
              //set dTree->curr to dTree->curr->parent
              setDTreeCurr(dTree, dTree->curr->parent);
            }
            setCellValue(board, curr->cell->row, curr->cell->col, 0);
            setPossibleCellValues(board);
            sortCellPQueueByLength(emptyCells);
          }
        } else {
          return true; //no more empty cells
        }
      }
      if (curr->cell->val == 0) {
        return false;
      }
    }
  }
  return true;
}

bool checkPossibleMove(SBoard *board, Cell *cell, int val) {
  int row, col, ssI;
  int *rv, *cv, *ssv; //vectors

  //Seeing if we can place val into cell
  row = cell->row;
  col = cell->col;
  ssI = getSSIndex(row, col);

  rv = getRowValues(board, row);
  cv = getColValues(board, col);
  ssv = getSubSquareValues(board,row,col);

  if (arrayContains(rv, val) || arrayContains(cv, val) || arrayContains(ssv, val)) {
    return false; //move is not possible
  } else {
    return true;  //val does not exist in other squares, so it is ok to place
  }
}
bool solveCell(SBoard *board, int row, int col) {
  int possibleValues[9];
  int i, j, c;
  int rowI, colI, num, val;
  bool valid, *vector;
  Cell *cell;
  if (rowIsFull(board, row)) {
    //skip it
  } else if (rowHasOneMissingValue(board, row)) {
    //fill the cell with that missing value
    val = 0;
    vector = board->rowVectors[row];
    for (num = 1; num <= 9; num++) {
      if (vector[num - 1] == false) {
        val = num;
        break;
      }
    }
    if (val != 0) {
      for (colI = 0; colI < 9; colI++) {
        if (getCellValue(board, row, colI) == 0) {
          setCellValue(board, row, colI, val);
          vector[val - 1] = true;
        }
      }
    }
  } else {
    if (colIsFull(board, row)) {
      //skip it
    } else if (colHasOneMissingValue(board, col)) {
      //fill the cell with that missing value
      val = 0;
      vector = board->colVectors[col];
      for (num = 1; num <= 9; num++) {
        if (vector[num - 1] == false) {
          val = num;
          break;
        }
      }
      //if val was found,
      if (val != 0) {
        for (rowI = 0; rowI < 9; rowI++) {
          if (getCellValue(board, rowI, col) == 0) {
            setCellValue(board, rowI, col, val);
            vector[val - 1] = true;
          }
        }
      }
    } else {
      //get rowVals, colVals, and ssVals
      //"cross" these vals to see what we cannot use, and invert that list
      crossCellVectors(board, row, col);
      cell = board->board[row][col];
      // into a list of possible values
      //if possibleValues is empty, there are no possible values
      if (cell->length == 0) {
        printf("There are no possible values\n");
        return false;
      } else {
        printf("Checking each possible value\n");
        for (i = 0; i < cell->length; i++) {
          setCellValue(board, row, col, cell->possibleValues[i]);
          //  determine next cell by modifying row/col values
          if (col < 8) {
            valid = solveCell(board, row, col + 1);
          } else {
            if (row < 8) {
              valid = solveCell(board, row + 1, 0);
            }
          }
          if (valid) {
            return true;
          }
        }
        return false;
      }
    }
  }
}
bool solveBoard(SBoard *board) {
  DTree *dTree;
  bool completed;
  printf("Sorting the empty cells by their priority:\n");
  sortCellPQueueByLength(board->emptyCells);
  
  printf("Creating DTree\n");
  dTree = createDTree();
  
  printf("Calling solveEmptyCells\n");
  completed = solveEmptyCells(board, dTree);
  
  destroyDTree(dTree);
  
  return completed;
  /*
  bool completed;
  int i, j;
  //Start at cell [0,0] and solve the board
  completed = solveCell(board, 0, 0);
  
  return completed;
   */
}

void fillFull(SBoard *board) {
  //fills the board with a generic solution, irregardless of what was in the cells
  int rowIndex, fillIndex,
  valIndex = 0;
  int vals[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  int filler[9];
  
  for (rowIndex = 0; rowIndex < 9; rowIndex++) {
    //Determine which values to fill the row with
    for (fillIndex = 0; fillIndex < 9; fillIndex++) {
      valIndex = valIndex % 9;
      filler[fillIndex] = vals[valIndex++];
    }
    //increment the val index so that our sudoku board satisfies sudoku conditions
    valIndex += 3;
    //Fill the row
    setRowValues(board, rowIndex, filler);
  }
}
void fillPartial(SBoard *board, char line[10], int index) {
  //fills the board partially from a file
  int row[9];
  int i;

  for (i = 0; i < 9; i++) {
    row[i] = line[i] - 48;
  }
  setRowValues(board,index,row);
}
void fillExample(SBoard *board) {
  //TODO: Change fillExample to work with board->size n
  //fills the board with the example given in the assignment. Used for testing
  int row[9];
  memmove(row, ((int[9]){0, 9, 0, 8, 7, 0, 3, 0, 0}), sizeof(int) *9);
  setRowValues(board, 0, row);
  memmove(row, ((int[9]){4, 0, 1, 0, 2, 0, 8, 0, 0}), sizeof(int) *9);
  setRowValues(board, 1, row);
  memmove(row, ((int[9]){7, 0, 3, 4, 0, 0, 0, 0, 0}), sizeof(int) *9);
  setRowValues(board, 2, row);
  memmove(row, ((int[9]){3, 0, 8, 0, 0, 0, 0, 7, 1}), sizeof(int) *9);
  setRowValues(board, 3, row);
  memmove(row, ((int[9]){6, 0, 0, 0, 0, 0, 0, 0, 9}), sizeof(int) *9);
  setRowValues(board, 4, row);
  memmove(row, ((int[9]){5, 7, 0, 0, 0, 0, 2, 0, 3}), sizeof(int) *9);
  setRowValues(board, 5, row);
  memmove(row, ((int[9]){0, 0, 0, 0, 0, 9, 7, 0, 8}), sizeof(int) *9);
  setRowValues(board, 6, row);
  memmove(row, ((int[9]){0, 0, 2, 0, 1, 0, 4, 0, 5}), sizeof(int) *9);
  setRowValues(board, 7, row);
  memmove(row, ((int[9]){0, 0, 7, 0, 4, 8, 0, 3, 0}), sizeof(int) *9);
  setRowValues(board, 8, row);
}

/*
 Print functions
 */
void printRowVectors(SBoard *board) {
  int i, j;

  for (i = 0; i < 9; i++) {
    printf("%d | ", i+1);
  }
  printf("\n");
  for (i = 0; i < 9; i++) {
    printf("----");
  }
  printf("\n");
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      printf("%d | ", board->rowVectors[i][j]);
    }
    printf("\n");
  }
}
void printColVectors(SBoard *board) {
  int i, j;
  for (i = 0; i < 9; i++) {
    printf("%d | ", i+1);
  }
  printf("\n");
  for (i = 0; i < 9; i++) {
    printf("----");
  }
  printf("\n");
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      printf("%d | ", board->colVectors[i][j]);
    }
    printf("\n");
  }
}
void printSubSquareVectors(SBoard *board) {
  int i, j;
  for (i = 0; i < 9; i++) {
    printf("%d | ", i+1);
  }
  printf("\n");
  for (i = 0; i < 9; i++) {
    printf("----");
  }
  printf("\n");
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      printf("%d | ", board->ssVectors[i][j]);
    }
    printf("\n");
  }
}

void printBoard(SBoard *board) {
  FILE *output;
  int row, col, val;
  char emptyChar = '.';
  char separator = '|';

  //output = fopen("output.sudoku","w");
  output = stdout;
  
  fprintf(output,"-------------------------------------\n");
  for (row = 0; row < 9; row++) {
    fprintf(output,"%c", separator);
    for (col = 0; col < 9; col++) {
      val = getCellValue(board, row, col);
      if (val == 0) {
        fprintf(output," %c %c", emptyChar, separator);
      } else {
        fprintf(output," %d %c", val, separator);
      }
    }
    fprintf(output,"\n");
    fprintf(output,"-------------------------------------\n");
  }

  //fclose(output);
}

/*
 Vector manipulation functions
 */
void setRowVector(SBoard *board, int row) {
  int num, *rowValues;
  rowValues = getRowValues(board, row);
  for (num = 1; num <= 9; num++) {
    if (arrayContains(rowValues, num)) {
      board->rowVectors[row][num - 1] = true;
    } else {
      board->rowVectors[row][num - 1] = false;
    }
  }
}
void setColVector(SBoard *board, int col) {
  int num, *colValues;

  //Set col vectors
  colValues = getColValues(board, col);
  for (num = 1; num <= 9; num++) {
    if (arrayContains(colValues, num)) {
      board->colVectors[col][num - 1] = true;
    } else {
      board->colVectors[col][num - 1] = false;
    }
  }
}
void setSubSquareVector(SBoard *board, int ssI ) {
  int row, col, num;
  int *ssValues;
  switch(ssI) {
    case 0:
      row = 0;
      col = 0;
      break;
    case 1:
      row = 0;
      col = 3;
      break;
    case 2:
      row = 0;
      col = 6;
      break;
    case 3:
      row = 3;
      col = 0;
      break;
    case 4:
      row = 3;
      col = 3;
      break;
    case 5:
      row = 3;
      col = 6;
      break;
    case 6:
      row = 6;
      col = 0;
      break;
    case 7:
      row = 6;
      col = 3;
      break;
    case 8:
      row = 6;
      col = 6;
      break;
  }
  ssValues = getSubSquareValues(board, row, col);

  for (num = 1; num <= 9; num++) {
    if (arrayContains(ssValues, num)) {
      board->ssVectors[ssI][num - 1] = true;
    } else {
      board->ssVectors[ssI][num - 1] = false;
    }
  }
}
void setBoardVectors(SBoard *board) {
  int i;

  for (i = 0; i < board->size; i++) {
    setRowVector(board, i);
    setColVector(board, i);
    setSubSquareVector(board, i);
  }
}

int getSSIndex(int row, int col) {
  int ssI;
  if (row < 3 && col < 3) {
    ssI = 0;
  } else if (row < 3 && col < 6) {
    ssI = 1;
  } else if (row < 3) {
    ssI = 2;
  } else if (row < 6 && col < 3) {
    ssI = 3;
  } else if (row < 6 && col < 6) {
    ssI = 4;
  } else if (row < 6) {
    ssI = 5;
  } else if (col < 3) {
    ssI = 6;
  } else if (col < 6) {
    ssI = 7;
  } else {
    ssI = 8;
  }
  
  return ssI;
}
bool *getSubSquareVector(SBoard *board, int row, int col) {
  bool *vector;
  int ssI;
  
  ssI = getSSIndex(row, col);
  /*
  if (row == 4 && col == 4) {
    printf("The ssI = %d\n", ssI);
  }*/

  vector = board->ssVectors[ssI];

  return vector;
}

void initialFindAndPrune (SBoard *board) {
  bool changes = false;

//  printf("Setting the board vectors\n");
  setBoardVectors(board);
//  printf("Determining possible cell values\n");
  setPossibleCellValues(board);
//  printf("Pruning the search tree\n");
  changes = fillSingleChoiceCells(board);
 
  // if changes to the board were made, set the new possible values and call this function again to fill new single choices
  if (changes == true) {
//    printf("Changes to the board made, restarting process\n");
    initialFindAndPrune(board);
  }
}
