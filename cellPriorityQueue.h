/* Sudoku Solver
   cellPriorityQueue.h
   Sept 4, 2015
*/

#ifndef CELL_P_QUEUE_H
#define CELL_P_QUEUE_H

#include "cell.h"

typedef struct CellPriorityQueue {
  Cell **cells;
  int length;
  
} CellPQueue;

/*
 Cell Priority Queue functions
 */
CellPQueue *createCellPQueue();
//CellPQueue will have an array of cell pointers, only responsible for freeing that array, not the cell pointers themselves
void destroyCellPQueue(CellPQueue *pQueue);
Cell *removeFirstCell(CellPQueue *pQueue);
void appendLastCell(CellPQueue *pQueue, Cell *cell);
void sortCellPQueueByLength(CellPQueue *pQueue);

/*
 Print functions
 */
void printPQueue(CellPQueue *pQueue);

#endif
