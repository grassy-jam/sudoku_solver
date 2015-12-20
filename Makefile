default: sudokuSolver

ca: ca_sudokuSolver

CC = gcc
CFLAGS = -std=c99 -pedantic -Wall
DEBUG = -g

ca_sudokuSolver: ca_sudoku.c
	$(CC) $(CFLAGS) $(DEBUG) -o ca_sudokuSolver ca_sudoku.c
sudokuSolver_bak: sudoku.o main.c
	$(CC) $(CFLAGS) -o sudokuSolver main.c sudoku.o

arrayHelpers.o: arrayHelpers.c arrayHelpers.h
	gcc -fPIC -c -o arrayHelpers.o arrayHelpers.c
cell.o: cell.c cell.h
	gcc -fPIC -c -o cell.o cell.c
cellPriorityQueue.o: cellPriorityQueue.c cellPriorityQueue.h
	gcc -fPIC -c -o cellPriorityQueue.o cellPriorityQueue.c
decisionNode.o: decisionNode.c decisionNode.h
	gcc -fPIC -c -o decisionNode.o decisionNode.c
decisionTree.o: decisionTree.c decisionTree.h
	gcc -fPIC -c -o decisionTree.o decisionTree.c
sudokuBoard.o: sudokuBoard.c sudokuBoard.h
	gcc -fPIC -c -o sudokuBoard.o sudokuBoard.c
sudokuSolver: arrayHelpers.o cell.o cellPriorityQueue.o decisionNode.o decisionTree.o sudokuBoard.o
	gcc $(CFLAGS) -fPIC -o sudokuSolver main.c arrayHelpers.o cell.o cellPriorityQueue.o decisionNode.o decisionTree.o sudokuBoard.o

clean:
	rm -f *.o sudokuSolver ca_sudokuSolver

run: sudokuSolver input.sudoku
	./sudokuSolver
ca_run: ca_sudokuSolver input.sudoku
	./ca_sudokuSolver
