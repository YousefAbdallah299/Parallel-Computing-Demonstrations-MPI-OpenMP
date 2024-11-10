# High Performance Computing: MPI & OpenMP Solutions

This repository contains solutions to several High Performance Computing (HPC) problems solved using MPI (Message Passing Interface) and OpenMP (Open Multi-Processing). The project demonstrates parallel computing techniques for solving problems such as election simulation, array search, and standard deviation calculation.

## Problem 1: Election to Choose a New President (Using MPI)

### Description:
This problem simulates an election process with multiple candidates and voters, where the election occurs in up to 2 rounds. In the first round, all candidates compete, and the candidate who receives more than 50% of the votes wins. If no candidate receives the majority, the top 2 candidates compete in a second round, and the one who receives more votes wins.

The program handles the following:
- Input: Number of candidates, number of voters, and the voters' preferences.
- Process: The election is conducted in two rounds (if needed).
- Output: The winning candidate and the round in which they win.

### Steps:
1. The program generates a file containing the voters' preferences.
2. The user can choose to either generate the file or calculate the result using an existing file.
3. The program loads the voter preferences, processes the election results, and announces the winner along with the round in which they win.


## Problem 2: Search in Array (Using OpenMP)

### Description:
This program performs a parallel search for a given key in a 2D array, with matrix dimensions and the key provided as input. The matrix is dynamically allocated and shared across all threads. Every time a key is found, the program prints which thread found it and at what index, and adds that index to an array. The final result is an array of indices where the key appeared.

### Steps:
1. Input the dimensions of the matrix (rows, columns) and the key to search for.
2. A 2D matrix is randomly initialized.
3. The matrix is shared among all processes.
4. A parallel search is conducted using the `#pragma omp parallel for` directive to divide the search loops among threads.


## Problem 3: Calculating Standard Deviation (Using OpenMP)

### Description:
This program calculates the standard deviation of randomly generated numbers in parallel. It divides the task across multiple processes to compute the standard deviation of `n * numberOfProcesses` elements efficiently.

### Steps:
1. Generate `n` random numbers in each process.
2. Each process calculates the local sum, local mean, and local sum of squared differences.
3. The local results are shared to compute the global sum and global mean.
4. The program calculates and outputs the standard deviation.


## Requirements

- **C Compiler**: The project uses C programming language with MPI and OpenMP libraries.
- **MPI**: Message Passing Interface for parallelization across processes.
- **OpenMP**: Open Multi-Processing for parallel execution within a single process.
- **Dynamic Memory Allocation**: Used for matrix creation in Problem 2.
- **Random Number Generation**: Used for generating random datasets in Problem 2 and Problem 3.

Thanks to the course instructors and teaching assistants at the Faculty of Computers and Artificial Intelligence, Cairo University, for their guidance and support.
