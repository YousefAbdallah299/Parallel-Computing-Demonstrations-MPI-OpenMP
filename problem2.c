#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void fillMatrix(int** matrix, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = rand() % 10;  //random number between 0 and 9
        }
    }
}

void printMatrix(int** matrix, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    int numColumns, numRows, key;

    printf("Enter the number of columns: ");
    scanf("%d", &numColumns);

    printf("Enter the number of rows: ");
    scanf("%d", &numRows);

    printf("Enter the key: ");
    scanf("%d", &key);

    int** randomNums = (int**)malloc(numRows * sizeof(int*));
    for (int i = 0; i < numRows; i++) {
        randomNums[i] = (int*)malloc(numColumns * sizeof(int));
    }

    srand(time(NULL));

    fillMatrix(randomNums, numRows, numColumns);

    printMatrix(randomNums, numRows, numColumns);

    int maxMatches = numRows * numColumns; 
    int** result = (int**)malloc(maxMatches * sizeof(int*));
    for (int i = 0; i < maxMatches; i++) {
        result[i] = (int*)malloc(2 * sizeof(int)); //the X and Y indexes
    }

    int writePointer = 0; // to use it to write into the result Array

    #pragma omp parallel shared(randomNums, numRows, numColumns, key, result, writePointer)
    {
        #pragma omp for collapse(2)
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numColumns; j++) {
                if (randomNums[i][j] == key) {
                    int currentWritePointer;
                    #pragma omp critical
                    {
                        currentWritePointer = writePointer;
                        writePointer++;
                    }
                    result[currentWritePointer][0] = i;
                    result[currentWritePointer][1] = j;
                    printf("Thread %d Found a Match at row: %d and column: %d\n", omp_get_thread_num(), i, j);
                }
            }
        }
    }

    // Print the indices where the key appeared
    printf("Indices where the key appeared:\n");
    for (int i = 0; i < writePointer; i++) {
        printf("(%d, %d)\n", result[i][0], result[i][1]);
    }

    // Free dynamically allocated memory
    for (int i = 0; i < maxMatches; i++) {
        free(result[i]);
    }
    free(result);

    for (int i = 0; i < numRows; i++) {
        free(randomNums[i]);
    }
    free(randomNums);

    return 0;
}
