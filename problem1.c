#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define MASTER_RANK 0
#define FILENAME "in.txt"
#define chkCondtion(x)  ((x != -1  || x != local_arr[i][j]))


void findMostFrequent(int arr[], int n, int *first_max_pos, int *second_max_pos) {
    int max_freq = 0, second_max_freq = 0;

    for (int i = 0; i < n; ++i)
    {
        if (arr[i] > max_freq)
        {
            max_freq = arr[i];
            *first_max_pos = i;
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if (i == *first_max_pos) continue;
        if (arr[i] > second_max_freq)
        {
            second_max_freq = arr[i];
            *second_max_pos = i;
        }
    }
}

int find(int array[], int size, int target)
{
    for (int i = 0; i < size; i++)
        if (array[i] == target)
            return i;

    return -1;
}

int main(int argc, char** argv)
{
    int rank, size;
    int nVoters, nCandidates;
    int* localFreq;
    int* globalFreq;
    int firstCandidate = -1, secondCandidate = -1;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int counts[size];
    int displacements[size];

    if (rank == MASTER_RANK)
    {
        FILE* fptr = fopen("in.txt", "r");
        fscanf(fptr, "%d %d", &nCandidates, &nVoters);
        fclose(fptr);

        globalFreq = (int*)malloc((nCandidates + 1) * sizeof(int*));
        const int range = nVoters / size;
        const int rem = nVoters % size;

        for (int i = 0; i < size; i++)
        {
            counts[i] = (i == 0 ? rem + range : range);
            displacements[i] = (i == 0 ? 0 : displacements[i - 1] + counts[i - 1]);
        }
    }
    MPI_Bcast(&firstCandidate, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    MPI_Bcast(&secondCandidate, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    MPI_Bcast(&counts, size, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    MPI_Bcast(&displacements, size, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    int** local_arr = (int**)malloc(counts[rank] * sizeof(int*));
    for (int i = 0; i < counts[rank]; i++)
        local_arr[i] = (int*)malloc(nCandidates * sizeof(int));

    FILE* fptr = fopen(FILENAME, "r");

    fscanf(fptr, "%d %d", &nCandidates, &nVoters);
    int digit;

    for (int i = 0; i < nVoters; ++i)
        for (int j = 0; j < nCandidates; ++j)
        {
            fscanf(fptr, "%d", &digit);
            if (i >= displacements[rank] && i < displacements[rank] + counts[rank])
                local_arr[i - displacements[rank]][j] = digit;
        }

    fclose(fptr);


    localFreq = (int*)malloc((nCandidates + 1) * sizeof(int*));
    for (int i = 0; i < nCandidates + 1; i++) localFreq[i] = 0;

    for (int i = 0; i < counts[rank]; ++i)
    {
        localFreq[local_arr[i][0]]++;
    }

    MPI_Reduce(localFreq, globalFreq, nCandidates + 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == MASTER_RANK)
    {
        findMostFrequent(globalFreq, nCandidates + 1, &firstCandidate, &secondCandidate);

        double percent1 = 1.0 * globalFreq[firstCandidate] / nVoters;
        if (percent1 >= 0.5)
        {
            printf("Candidate %d wins in round %d", firstCandidate, 1);
            return 0 ;
        }
    }
    // same thing as above
    MPI_Bcast(&firstCandidate, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    MPI_Bcast(&secondCandidate, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    MPI_Bcast(&counts, size, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    MPI_Bcast(&displacements, size, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);


    FILE* fptr2 = fopen(FILENAME, "r");

    fscanf(fptr2, "%d %d", &nCandidates, &nVoters);

    for (int i = 0; i < nVoters; ++i)
        for (int j = 0; j < nCandidates; ++j)
        {
            fscanf(fptr2, "%d", &digit);
            if (i >= displacements[rank] && i < displacements[rank] + counts[rank])
                local_arr[i - displacements[rank]][j] = digit;
        }

    fclose(fptr2);


    localFreq = (int*)malloc((nCandidates + 1) * sizeof(int*));
    for (int i = 0; i < nCandidates + 1; i++) localFreq[i] = 0;

    for (int i = 0; i < counts[rank]; ++i)
    {
        int j = 0;
        while (local_arr[i][j] != firstCandidate && local_arr[i][j] != secondCandidate)
        {
            j++;
        }

        localFreq[local_arr[i][j]]++;
    }

    MPI_Reduce(localFreq, globalFreq, nCandidates + 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    if (rank == MASTER_RANK)
    {
        findMostFrequent(globalFreq, nCandidates + 1, &firstCandidate, &secondCandidate);

        printf("Candidate %d wins in round %d", firstCandidate, 2);
    }


    MPI_Finalize();

    return 0;
}
