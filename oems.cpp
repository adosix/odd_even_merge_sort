#include <iostream>
#include <algorithm>
#include <math.h>
#include <fstream>
#include "mpi.h"
using namespace std;

int getMinId(int *arr, int length)
{
    int maxId = 0;
    for (int i = 1; i < length; i++)
        if (arr[i] < arr[maxId])
            maxId = i;
    return maxId;
}

int getMaxId(int *arr, int length)
{
    int maxId = 0;
    for (int i = 1; i < length; i++)
        if (arr[i] > arr[maxId])
            maxId = i;
    return maxId;
}

void quickSort(int *arr, int low, int high)
{
    int left = low, right = high;
    int tmp;
    int pivot = arr[(left + right) / 2];

    // partition
    while (left <= right)
    {
        while (arr[left] < pivot)
        {
            left++;
        }
        while (arr[right] > pivot)
        {
            right--;
        }
        if (left <= right)
        {
            tmp = arr[left];
            arr[left] = arr[right];
            arr[right] = tmp;
            left++;
            right--;
        }
    };

    if (low < right)
        quickSort(arr, low, right);
    if (left < high)
        quickSort(arr, left, high);
}

void merge(int *locArr, int numProcessors, int numElements, int rank)
{
    int adjArray[numElements];

    for (int i = 0; i < numProcessors; i++)
    {
        quickSort(locArr, 0, numElements - 1); // quick sort on all elements
        int adjProc;

        if (i % 2 == 0)
        {
            if (rank % 2 == 0)
                adjProc = rank + 1;
            else
                adjProc = rank - 1;
        }
        else
        {
            if (rank % 2 == 0)
                adjProc = rank - 1;
            else
                adjProc = rank + 1;
        }

        if (adjProc >= numProcessors || adjProc < 0)
            continue;

        if (rank % 2 == 0)
        {
            MPI_Send(locArr, numElements, MPI_INT, adjProc, 0, MPI_COMM_WORLD);
            MPI_Recv(adjArray, numElements, MPI_INT, adjProc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else
        {
            MPI_Recv(adjArray, numElements, MPI_INT, adjProc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(locArr, numElements, MPI_INT, adjProc, 0, MPI_COMM_WORLD);
        }
        if (rank < adjProc)
        {
            while (true)
            {
                int minIndex = getMinId(adjArray, numElements);
                int maxId = getMaxId(locArr, numElements);
                if (adjArray[minIndex] < locArr[maxId])
                {
                    int tmp = adjArray[minIndex];
                    adjArray[minIndex] = locArr[maxId];
                    locArr[maxId] = tmp;
                }
                else
                    break;
            }
        }
        else
        {
            while (true)
            {
                int minIndex = getMinId(locArr, numElements);
                int maxId = getMaxId(adjArray, numElements);
                if (adjArray[maxId] > locArr[minIndex])
                {
                    int tmp = adjArray[maxId];
                    adjArray[maxId] = locArr[minIndex];
                    locArr[minIndex] = tmp;
                }
                else
                    break;
            }
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
        // cout << arr[i] << " ";
        i++;
        if (!fin.good())
            break;
    }

    printf("\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int my_rank;                           /* rank current process     */
    int processes;                         /* number of aa processes  */
    int elementsPerProc;                   // length of local array
    int *array, *sortedArray, *localArray; // local array for each processor
    int numElements = 8;                   // number of elements to sort

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    printf("rank: %d , number of processes: %d\n", my_rank, processes);

    elementsPerProc = numElements / processes;

    if (my_rank == 0)
    {
        array = (int *)malloc(sizeof(int) * numElements);
        sortedArray = (int *)malloc(sizeof(int) * numElements);
        ReadFile(array, numElements);

        cout << "Unsorted array:";
        for (int i = 0; i < numElements; i++)
            cout << array[i] << " ";
        cout << "\n";
    }

    localArray = (int *)malloc(sizeof(int) * elementsPerProc);

    MPI_Scatter(array, elementsPerProc, MPI_INT, localArray, elementsPerProc, MPI_INT, 0, MPI_COMM_WORLD);

    merge(localArray, processes, elementsPerProc, my_rank);

    MPI_Gather(localArray, elementsPerProc, MPI_INT, sortedArray, elementsPerProc, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        cout << "Sorted array:";
        for (int i = 0; i < numElements; i++)
        {
            cout << sortedArray[i] << " ";
        }
        cout << "\n";
    }
    MPI_Finalize();
}