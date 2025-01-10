#include "sort_algs.h"
static void swap(unsigned int * a, unsigned int * b);
static int partition(unsigned int  arr[], int low, int high);

void bubble_sort(unsigned int dis_array[], int size)
{
	int i, j;
	for (i = 0; i < size-1; i++)
	{
		for(j = 0; j < size-i-1; j++)
		{
			if(dis_array[j] > dis_array[j+1])
			{
				int temp = dis_array[j];
				dis_array[j] = dis_array[j+1];
				dis_array[j+1] = temp;
			}
		}
	}
}

// Function to swap two elements
static void swap(unsigned int * a, unsigned int * b)
 {
    //*a ^= *b;
    //*b ^= *a;
    //*a ^= *b;
    unsigned int temp = *a;
    *a = *b;
    *b = temp;
    
}

// Partition function to rearrange the array and return the pivot index
static int partition(unsigned int  arr[], int low, int high) 
{
    unsigned int pivot = arr[high];  // Choosing the last element as the pivot
    int i = (low - 1);

    for ( int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Quicksort function
void quick_sort(unsigned int arr[], int low, int high) 
{
    if (low < high) 
    {
        unsigned int pivot = partition(arr, low, high);
        quick_sort(arr, low, pivot - 1);
        quick_sort(arr, pivot + 1, high);
    }
}
