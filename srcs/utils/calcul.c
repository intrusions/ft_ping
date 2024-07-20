#include "inc.h"

double calcul_latency(timeval start_time, timeval end_time)
{
    double time_elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
    time_elapsed += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

    return time_elapsed;
}

void calcul_statistics(t_times *times, double *min, double *max, double *avg, double *stddev)
{
    double sum = 0.0;
    double sum_sq_diff = 0.0;
    
    *avg = 0.0;
    *stddev = 0.0;
    *min = times->arr[0];
    *max = times->arr[0];

    for (uint16_t i = 0; i < times->size; i++) {

        sum += times->arr[i];
        
        if (times->arr[i] < *min) {
            *min = times->arr[i];
        } else if (times->arr[i] > *max) {
            *max = times->arr[i];
        }
    }

    *avg = sum / times->size;

    for (uint16_t i = 0; i < times->size; i++) {
        double diff = times->arr[i] - *avg;
        sum_sq_diff += diff * diff;
    }

    *stddev = sqrt(sum_sq_diff / times->size);
}
