/* Sudoku Solver
   cellPriorityQueue.c
   Sept 4, 2015
*/

#include "cellPriorityQueue.h"
/*
 Cell Priority Queue functions
 */
CellPQueue *createCellPQueue() {
  CellPQueue *pQueue;
  
  pQueue = malloc(sizeof(CellPQueue));
  pQueue->cells = NULL;
  pQueue->length = 0;
  
  return pQueue;
}
void destroyCellPQueue(CellPQueue *pQueue) {
  free(pQueue->cells);
  free(pQueue);
}
Cell *removeFirstCell(CellPQueue *pQueue) {
  Cell *cell;
  int i;
  if (pQueue->length < 1) {
    return NULL;
  }
  cell = pQueue->cells[0];
  for (i = 1; i < pQueue->length; i++) {
    pQueue->cells[i - 1] = pQueue->cells[i];
  }
  pQueue->length--;
  pQueue->cells = realloc(pQueue->cells, sizeof(Cell *) * pQueue->length);
  
  return cell;
}
void appendLastCell(CellPQueue *pQueue, Cell *cell) {
  pQueue->length++;
  pQueue->cells = realloc(pQueue->cells, sizeof(Cell *) * pQueue->length);
  pQueue->cells[pQueue->length - 1] = cell;
}
void sortCellPQueueByLength(CellPQueue *pQueue) {
  Cell *temp;
  int i, j;
//  printf("Before sort: \n");
//  printPQueue(pQueue);
  //Using bubble sort (ascending) on the pQueue->cells->length
  for (i = 0; i < pQueue->length - 1; i++) {
    for (j = 0; j < pQueue->length - 1; j++) {
      //printf("j: %d, j+1: %d\n", j, (j+1));
      //printf("Comparing [%d][%d]:%d with [%d][%d]:%d ", pQueue->cells[j]->row, pQueue->cells[j]->col, pQueue->cells[j]->val, pQueue->cells[j+1]->row, pQueue->cells[j + 1]->col, pQueue->cells[j + 1]->length);
      if (pQueue->cells[j]->length > pQueue->cells[j + 1]->length) {
        temp = pQueue->cells[j];
        pQueue->cells[j] = pQueue->cells[j + 1];
        pQueue->cells[j + 1] = temp;
        //printf(" : swap\n");
      } else {
        //printf(" : no swap\n");
      }
    }
  }
 // printf("After sort: \n");
 // printPQueue(pQueue);
}

/*
 Print functions
 */
void printPQueue(CellPQueue *pQueue) {
  int row, col, num, i;
  for (i = 0; i < pQueue->length; i++) {
    row = pQueue->cells[i]->row;
    col = pQueue->cells[i]->col;
    num = pQueue->cells[i]->length;
    printf("%d - [%d][%d]: %d\n", i, row, col, num);
  }
}
