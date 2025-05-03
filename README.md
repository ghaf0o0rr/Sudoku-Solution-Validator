# Sudoku-Solution-Validator
A minimal multithreaded program that validates a 9 × 9 Sudoku solution.  
It spawns **27 threads** (9 rows, 9 columns, 9 sub-grids) using the POSIX `pthread` library.  
If every thread confirms its region contains all digits `1‒9` exactly once, the board is valid.
