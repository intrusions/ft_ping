#include "inc.h"

void push_time(t_times *times, double time)
{
    times->arr = (double *)realloc(times->arr, (times->size + 1) * sizeof(double));
    times->arr[times->size++] = time;
}

void free_times(t_times *times)
{
    if (times->size)
        free(times->arr);
}