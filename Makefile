default: sudokuSolver

ca_sudokuSolver: ca_sudoku.c
	gcc -o ca_sudokuSolver ca_sudoku.c
sudokuSolver: sudoku.o main.c
	gcc -o sudokuSolver main.c sudoku.o

arrayHelpers.o: arrayHelpers.c arrayHelpers.h
	gcc -c -o arrayHelpers.o arrayHelpers.c
cell.o: cell.c cell.h
	gcc -c -o cell.o cell.c
cellPriorityQueue.o: cellPriorityQueue.c cellPriorityQueue.h
	gcc -c -o cellPriorityQueue.o cellPriorityQueue.h
decisionNode.o: decisionNode.c decisionNode.h
	gcc -c -o decisionNode.o decisionNode.c
decisionTree.o: decisionTree.c decisionTree.h
	gcc -c -o decisionTree.o decisionTree.c
sudokuBoard.o: sudokuBoard.c sudokuBoard.h
	gcc -c -o sudokuBoard.o sudokuBoard.h
sudoku.o: arrayHelpers.o cell.o cellPriorityQueue.o decisionNode.o decisionTree.o sudokuBoard.o
	gcc -o sudoku.o arrayHelpers.o cell.o cellPriorityQueue.o decisionNode.o decisionTree.o sudokuBoard.o

clean:
	rm -f *.o sudokuSolver ca_sudokuSolver

run: sudokuSolver input.sudoku
	./sudokuSolver
ca_run: ca_sudokuSolver input.sudoku
	./ca_sudokuSolver
