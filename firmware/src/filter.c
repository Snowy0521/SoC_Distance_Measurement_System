#include "filter.h"
#include "sort_algs.h"

unsigned int median_filter(unsigned int dis_array[], int size)
{
	//bubble_sort(dis_array, size); // sort the array before extracting the median
	quick_sort(dis_array,0, size-1);
	//median value from disatance array
	if(size % 2 == 0)
	{
		return (dis_array[size/2] + dis_array[size/2 - 1])/2;
	}
	else
	{
		return dis_array[(size-1)/2];
	}
}

