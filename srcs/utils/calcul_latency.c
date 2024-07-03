#include "inc.h"

double calcul_latency(struct timeval start_time, struct timeval end_time)
{
    double time_elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
    time_elapsed += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

    return time_elapsed;
}