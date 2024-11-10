#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define square(x) ((x)* (x))
#define RAND_LIMIT (1000)
#define NTHREADS (6)

int getRandomSeed()
{
    srand(time(NULL));
    return rand();
}

int main()
{
    omp_set_num_threads(NTHREADS);

    int n;
    printf("Enter the size of the array: ");
    scanf("%d", &n);

    int* localArr;

    double localSum = 0 ;
    double localMean = 0;
    double localSumDiff = 0 ;

    double gSum = 0;
    double gMean = 0.0;

#pragma omp parallel private(localSum, localMean, localSumDiff, localArr) shared(gSum, n)
    {
        localArr = malloc(n * sizeof(int));

        printf("I am thread %d, \n ", omp_get_thread_num());
        for (int i = 0; i < n; i++)
        {
            srand(getRandomSeed() * (i + 1) * (omp_get_thread_num() + 1));
            localArr[i] = rand() % RAND_LIMIT;
            localSum += localArr[i];
        }
        for (int i = 0; i < n; ++i)
        {
            printf("%d, ", localArr[i]);

        }
        printf("\n--------------------------\n");
        localMean = localSum / n ;
        for (int i = 0; i < n; ++i)
        {
            gSum += square(localArr[i] - localMean);
        }


    }
    gMean = gSum / (n * NTHREADS);
    printf("Standard deviation is : %f", sqrt(gMean));

    return 0;
}
