default: sudokuSolver

sudokuSolver: sudoku.o main.c
	gcc -o sudokuSolver main.c sudoku.o

sudoku.o: sudoku.c sudoku.h
	gcc -c -o sudoku.o sudoku.c

clean:
	rm -f sudoku.o sudokuSolver

run: sudokuSolver input.sudoku
	./sudokuSolver
