#include <iostream>
#include <fstream>
#include <mpi.h>
using namespace std;

void oddEvenMerge(int my_rank, int recievedNumbers[2])
{
    int min, max;
    if (recievedNumbers[0] < recievedNumbers[1])
    {
        min = recievedNumbers[0];
        max = recievedNumbers[1];
    }
    else
    {
        min = recievedNumbers[1];
        max = recievedNumbers[0];
    }

    switch (my_rank)
    {
    case 0:
    {
        MPI_Send(&min, 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
        break;
    }
    case 1:
    {
        MPI_Send(&min, 1, MPI_INT, 4, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 5, 1, MPI_COMM_WORLD);
        break;
    }
    case 2:
    {
        MPI_Send(&min, 1, MPI_INT, 6, 0, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 7, 0, MPI_COMM_WORLD);
        break;
    }
    case 3:
    {
        MPI_Send(&min, 1, MPI_INT, 6, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 7, 1, MPI_COMM_WORLD);
        break;
    }
    case 4:
    {
        MPI_Send(&min, 1, MPI_INT, 10, 0, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 8, 0, MPI_COMM_WORLD);
        break;
    }
    case 5:
    {
        MPI_Send(&min, 1, MPI_INT, 8, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 13, 0, MPI_COMM_WORLD);
        break;
    }
    case 6:
    {
        MPI_Send(&min, 1, MPI_INT, 10, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 9, 0, MPI_COMM_WORLD);
        break;
    }
    case 7:
    {
        MPI_Send(&min, 1, MPI_INT, 9, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 13, 1, MPI_COMM_WORLD);
        break;
    }
    case 8:
    {
        MPI_Send(&min, 1, MPI_INT, 12, 0, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 11, 0, MPI_COMM_WORLD);
        break;
    }
    case 9:
    {
        MPI_Send(&min, 1, MPI_INT, 12, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 11, 1, MPI_COMM_WORLD);
        break;
    }
    case 10:
    {
        MPI_Send(&min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 14, 0, MPI_COMM_WORLD);
        break;
    }
    case 11:
    {
        MPI_Send(&min, 1, MPI_INT, 14, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 18, 0, MPI_COMM_WORLD);
        break;
    }
    case 12:
    {
        MPI_Send(&min, 1, MPI_INT, 16, 0, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 15, 0, MPI_COMM_WORLD);
        break;
    }
    case 13:
    {
        MPI_Send(&min, 1, MPI_INT, 15, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 0, 7, MPI_COMM_WORLD);
        break;
    }
    case 14:
    {
        MPI_Send(&min, 1, MPI_INT, 16, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 17, 0, MPI_COMM_WORLD);
        break;
    }
    case 15:
    {
        MPI_Send(&min, 1, MPI_INT, 17, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 18, 1, MPI_COMM_WORLD);
        break;
    }
    case 16:
    {
        MPI_Send(&min, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        break;
    }
    case 17:
    {
        MPI_Send(&min, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
        break;
    }
    case 18:
    {
        MPI_Send(&min, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
        MPI_Send(&max, 1, MPI_INT, 0, 6, MPI_COMM_WORLD);
        break;
    }
    }
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
    int recievedNumbers[2];

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

    MPI_Recv(&recievedNumbers[0], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, nullptr);
    MPI_Recv(&recievedNumbers[1], 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, nullptr);

    oddEvenMerge(my_rank, recievedNumbers);

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