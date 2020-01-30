#include <stdio.h>
#include <lib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char* argv) {
    clock_t start, end;
    double dtime;
    int i;

    start = clock();
    for(i = 0; i < 3000 ; i++)
        sqrt(i);
    end = clock();
    dtime = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("\tPID: %d\nTime: %f s\n", getpid(), dtime); 
    return 0;
}
