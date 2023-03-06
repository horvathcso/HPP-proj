#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

void create_table(int** table, int N)
// TO DO 
{

    printf("HERE");
    int i,j;
    for(i = 0; i < N*N; i++)
        for(j=0; j < N*N; j++)
            table[i][j] = rand() % N*N;
}

int main() {
    printf("HERE");
    int N=3;
    int** table=(int**)malloc(N*N*N*N*sizeof(int));
    create_table(table,N);

    printf("HERE");
    //print_table(table,N);

    free(table);
}