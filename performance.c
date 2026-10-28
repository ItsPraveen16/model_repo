#include <time.h>

struct timespec start, end;

clock_gettime(CLOCK_MONOTONIC, &start);

/* Send request and receive server response here */

clock_gettime(CLOCK_MONOTONIC, &end);

double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;

printf("Round-trip time: %.3f ms\n", elapsed_ms);   