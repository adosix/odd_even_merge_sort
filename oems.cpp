#include <iostream>
#include <fstream>
#include <mpi.h>

#include "oems.h"
using namespace std;

void oddEvenMerge(int my_rank, int processInput[2])
{
    int min, max;
    if (processInput[0] < processInput[1])
    {
        min = processInput[0];
        max = processInput[1];
    }
    else
    {
        min = processInput[1];
        max = processInput[0];
    }
    MPI_Send(&min, 1, MPI_INT, merging_scheme[my_rank][1], merging_scheme[my_rank][2], MPI_COMM_WORLD);
    MPI_Send(&max, 1, MPI_INT,
             merging_scheme[my_rank][3],
             merging_scheme[my_rank][4],
             MPI_COMM_WORLD);
}

void ReadFile(int *arr, int numElements)
{
    int i = 0;
    fstream fin;
    fin.open("numbers", ios::in);

    while (fin.good() && i < numElements)
    {
        arr[i] = fin.get();
        i++;
        if (!fin.good())
            break;
    }
}

int main(int argc, char *argv[])
{
    int my_rank;
    int *array;
    int numElements = 8;
    int processInput[2];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // printf("rank: %d , number of processes: %d\n", my_rank, processes);

    if (my_rank == 0)
    {
        array = (int *)malloc(sizeof(int) * numElements);
        ReadFile(array, numElements);

        cout << "Unsorted array: ";
        for (int i = 0; i < numElements; i++)
            cout << array[i] << " ";
        cout << "\n";

        for (int i = 0; i < numElements; i += 2)
        {
            MPI_Send(&(array[i]), 1, MPI_INT, i / 2, 0, MPI_COMM_WORLD);
            MPI_Send(&(array[i + 1]), 1, MPI_INT, i / 2, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Recv(&processInput[0], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, nullptr);
    MPI_Recv(&processInput[1], 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, nullptr);

    oddEvenMerge(my_rank, processInput);

    if (my_rank == 0)
    {

        int *nums = new int[numElements];
        for (int i = 0; i < numElements; i++)
        {
            MPI_Recv(&nums[i], 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, nullptr);
        }

        cout << "Sorted array:   ";
        for (int i = 0; i < numElements; i++)
        {
            cout << nums[i] << " ";
        }
        cout << "\n";
        delete nums;
    }
    MPI_Finalize();
}