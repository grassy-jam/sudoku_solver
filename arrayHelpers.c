/* Sudoku Solver
   arrayHelpers.c
   Sept 4, 2015
*/

/*
  Generic helpers
*/
bool arrayContains(int* arrayOfSizeNine, int num) {
  int i;
  bool contains;
  contains = false;
  for (i = 0; i < 9; i++) {
    if (arrayOfSizeNine[i] == num) {
      contains = true;
      break;
    }
  }
  return contains;
}
bool isEmpty(int vals[9]) {
  int i;
  bool empty = false;
  for (i = 0; i < 9; i++) {
    if (vals[i] == true) {
      empty = vals[i];
    }  
  }
  return empty;
}
bool removeValFromArray(int *array, int n, int val) {
  int i, foundValIndex;
  bool containsVal;
  
  containsVal = false;
  for (i = 0; i < n; i++) {
    if (array[i] == val) {
      containsVal = true;
      foundValIndex = i;
    }
  }
  if (containsVal) {
    for (i = foundValIndex + 1; i < n; i++) {
      array[i - 1] = array[i];
    }
    array = realloc(array, sizeof(int) * (n - 1));
    return true;
  } else {
    return false;
  }
}
