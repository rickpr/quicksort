#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

// Built in difftime() had a poor resolution on my CPU
double current_time ()
{
  // timeval is a built in struct
  struct timeval time;
  // and so is timezone
  struct timezone zone;
  // Together, they will give us the time
  gettimeofday(&time, &zone);
  // With microsecond resolution
  return time.tv_sec + time.tv_usec*1e-6;
}


// A helper function two swap two values
void swap (int* a, int* b)
{
  // Use a temp variable
  int temp = *a;
  // There's a method where you can use XOR instead
  *a = *b;
  // But this is fine too 
  *b = temp;
}

// The partition function
int partition (int *array, int initial, int end)
{
  // Our pivot element to compare against
  int pivot = array[end];
  // This tells us where to start comparing. I've modified this from Cormen, as
  // it is just too weird to index outside the array sometimes.
  int start = initial;
  // This inspector moves along ahead of the start point, letting us know which
  // element will be swapped with the start point. 
  int inspector;
  // This loop starts the inspector at the start point, and moves along until it
  // hits the pivot point (always moved to the end of this portion of the array).
  for (inspector = start; inspector < end; inspector++)
  {
    // If the inspector is less than the pivot, move it to the left
    if (array[inspector] <= pivot)
    {
      // Swap inspector's value with start's value. This always moves it left,
      // except on the first element, where nothing happens.
      swap(&array[start], &array[inspector]);
      // I increment start after the swap. This differs from Cormen, and avoids
      // all the indexing outside the array while resulting in a cleaner return
      // statement.
      start++;
    }
  }
  // Swap the final ``start'' point with the actual pivot, this leaves all the
  // lower values on the left of pivot, and the higher values on the right.
  swap(&array[start], &array[end]);
  // Return the index of ``start'' which now contains the pivot.
  return start;
}


 
// The quicksort routine
void quick_sort (int *array, int start, int end)
{
  // If start isn't less than end, we have a singleton array. Skip it.
  if (start < end)
  {
    // Partition returns a pivot index and has side effects
    int pivot_index = partition(array, start, end);
    // Quick sort the ``smaller'' half
    quick_sort(array, start, pivot_index - 1);
    // Quick sort the ``larger'' half
    quick_sort(array, pivot_index + 1, end);
  }
}

// A helper function to call quick_sort and return a time with it
double quick_sort_with_time (int* array, int start, int end)
{
  // Time before the quicksort 
  double start_time = current_time();
  // The actual quicksort
  quick_sort(array, start, end);
  // Time after the quicksort
  double end_time = current_time();
  // Time between starting and finishing quicksort
  return end_time - start_time;
}

// The main function!
int main ()
{
  // Initialize the random seed to help assure ``true'' pseudorandom
  srand(current_time());
  int *array;
  // The size of the array, I'm going to build four arrays
  int size;
  // This is used to build the array 
  int array_builder;
  // Initialize these out here
  double random_sort_time;
  double presorted_time;
  // Build the four arrays, 10, 100, 1000, 10000
  for (size = 10; size <= 10000; size *= 10)
  {
    // Allocate memory for enough integers
    array = (int*)calloc(size, sizeof(int));
    for (array_builder = 0; array_builder < size; array_builder++)
      // Initialize to random values
      array[array_builder] = rand();
    // Sort the array now that it's random
    random_sort_time = quick_sort_with_time(array, 0, size - 1);
    // And again after sorting it
    presorted_time = quick_sort_with_time(array, 0, size - 1);
    // Print out the times
    printf("\n******* QUICKSORT TIME FOR SIZE %d ARRAY *******\n", size);
    printf("Random array took %2f seconds to sort\n", random_sort_time);
    printf("Sorted array took %2f seconds to sort\n", presorted_time);
    // Free the memory so we can do it again!
    free(array);
  }
  return 0;
}
