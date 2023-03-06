#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>


static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

void read_data(int* data, const char* filename, int NN){
    FILE *file;
	file=fopen(filename,"rb");
	fread(data,(2*NN*NN+1)*sizeof(int),1,file)!=0;
	fclose(file);
}

void save_data(int* data, const char* filename,int NN){
    FILE* fp = fopen(filename,"w"); //should be result.gal 
	if(fp == NULL)
    {
        printf("The file was not created.\n");
        exit(EXIT_FAILURE);
    }
	fwrite(data,((NN*NN*2+1)*sizeof(int)),1,fp);
	fclose(fp);

}

void shuffle(int* head, int idx, int n){
    for (int i = 0; i < n; ++i)
    {
        int i = rand() %idx;
        int j= rand() %idx;
        int tmp = head[i];
        head[i] = head[j];
        head[j] = tmp;
    }
}

void cpy_table(int* t1, int*t2, int NN){
    for (int i = 0; i < NN*NN; i++)
        t2[i]=t1[i];
}

void print_table(int* table, int NN)
{
    int i,j;
    for(i = 0; i < NN; i++){
        for(j=0; j < NN; j++)
            if(table[i*NN+j]==0){printf(" & ");}
            else printf("%d& ",table[i*NN+j]);
        printf("\n");}
}


int check(int* table, int N){
    int NN=N*N;
    for (int i = 0; i < NN; i++)
    {
        for (int j = 0; j < NN; j++)
        {
                if(table[i*NN+j]==0){
                    printf("Check failed! There is empty cell, %d %d\n", i, j);
                    return 0;}
                for (int k = j; k < NN; k++) // row column
                {
                    if (table[i*NN+j]==table[i*NN+k] && j!=k)
                        return 0;
                    if(table[j*NN+i]==table[k*NN+i] && j!=k)
                        return 0;
                }      
        }
    }
    //block validation
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                for (int l = k+1; l < N; l++)
                {
                    if (table[N*(i*N+k/N)+(j*N+k%N)]==table[N*(i*N+l/N)+(j*N+l%N)])
                    {return 0;}   
                }   
            }
        }   
    }   
    return 1;
}


int validate_table(int* table, int id, int N){
    int NN=N*N;
    int x = id/(NN);
    int y = id%(NN);
    for(int i=0;i<NN;i++){
            if(table[x*NN+y] == table[i*NN+y] && x != i)
            return 0;}
    
    for(int i=0;i<NN;i++){
            if(table[x*NN+y] == table[x*NN+i] && y != i)
            return 0;
            }

    for(int i=-(x%N); i<N-(x%N);i++){
        for (int j = -(y%N); j  < N-(y%N); j++)
        {
            if(table[x*NN+y] == table[(x+i)*NN+(y+j)] && (i!=0 || j!=0))
            return 0;
        }
    }
    return 1;
}

int nr_sol;
int solve_table(int* table, int N, int* head, int idx, int first){
        if (!first){
            if(idx==0){nr_sol++;
                if(nr_sol==1) return 1; 
                else return 0;}}
        else{if(idx==0)
                return 1;}
        int id=head[idx-1];
        for(int i=1; i<N*N+1;i++){
            table[id]=i;
            if (validate_table(table, id, N)){
                if(solve_table(table, N, head, idx-1, first))
                {
                    return 1;
                }
            }
        }
        table[id]=0;
        return 0;
}

void create_table(int* table, int NN, int* head, int* idx)
// TO DO 
{
    srand(time(0));
    int i,j,rnd;
    for(i = 0; i < NN; i++)
        for(j=0; j < NN; j++){
            rnd= rand()%100;
            if(rnd<7)//Not empty
            {table[i*NN+j] = rand() % (NN) + 1;}
            else{ // empty
                table[i*NN+j] = 0;
                head[(*idx)]=i*NN+j;
                (*idx)++;}}
}

int main(int argc, char* argv[]) {

    // read args
    if(argc != 3) {
        printf("Please give 2 argument: N (if table side length is N*N), test_file (binary starting data for problem size N).\n");
        return -1;
    }
    int N = atoi(argv[1]);
    int NN=N*N;
	const char *filename=argv[2];
    
    // Init data
    
    int* data=(int*)calloc((2*NN*NN+1),sizeof(double));
    read_data(data, filename, NN);    
    int* table=data;
    int* head=data+NN*NN;
    int idx=head[NN*NN];

/* 
idx=0;
for (int i = 0; i < NN*NN; i++)
    {
        if(table[i]==0){idx++;}
    }
    
    idx=0;
    create_table(table, NN, head, &idx);
    
    for (int i = 0; i < NN*NN; i++)
    {
        head[i]=i;
    }
    idx=NN*NN;*/
    
    //static int arr[] = {16,23,7,0,0,24,0,4,0,0,0,10,0,0,0,1,0,18,0,0,8,21,14,0,17,0,0,20,0,0,19,15,16,0,0,0,0,0,5,24,4,0,2,14,23,0,0,18,0,7,9,2,12,0,0,0,0,0,20,11,13,0,0,7,0,0,0,0,0,6,0,0,10,25,1,4,0,0,0,19,0,0,0,14,0,8,0,0,23,21,10,0,9,7,17,0,0,0,0,0,18,0,0,0,0,0,1,17,10,0,11,15,19,0,0,12,0,20,0,0,0,13,0,0,0,0,7,1,3,0,0,12,0,0,0,0,0,16,0,0,8,20,11,0,0,0,0,0,9,21,0,6,0,10,0,0,2,21,18,0,12,19,23,0,0,0,0,0,24,16,1,0,0,14,0,8,20,0,18,16,11,0,0,24,0,9,0,0,0,3,0,0,0,22,0,12,0,0,10,4,0,0,0,0,0,1,0,0,9,22,4,0,0,0,0,0,17,23,2,0,24,8,13,0,0,15,21,0,17,9,8,0,0,0,0,0,18,7,2,0,0,1,0,0,0,0,0,19,0,0,0,4,0,16,0,0,0,14,0,0,0,22,0,10,0,0,11,17,8,0,21,24,9,0,0,0,10,11,22,0,0,0,0,0,21,24,3,0,17,1,7,0,0,18,0,5,0,0,0,14,0,0,0,0,17,10,4,0,0,20,0,0,0,0,0,25,0,0,9,5,16,0,0,0,0,25,0,0,0,6,0,16,0,0,19,14,13,0,8,9,23,0,0,0,0,0,12,4,18,0,0,0,23,21,20,0,7,18,13,0,0,4,0,6,0,0,0,3,0,0,0,17,0,19,0,0,0,10,0,0,0,0,0,17,0,0,7,14,12,0,0,0,0,0,4,25,16,0,22,19,0,0,14,11,13,0,10,19,12,0,0,0,0,0,16,18,15,0,0,7,0,0,0,0,0,5,16,0,0,24,0,14,0,0,0,17,0,0,0,11,0,19,0,0,1,6,10,0,4,18,0,18,0,0,3,21,11,0,0,0,0,0,6,13,22,0,25,24,10,0,0,5,0,23,0,12,19,0,0,0,0,0,8,2,23,0,0,9,0,0,0,0,0,6,0,0,7,15,11,0,0,0,0,9,0,0,0,12,0,7,0,0,10,24,14,0,5,19,1,0,0,0,0,0,13,0,0,0,0,0,22,23,24,0,14,21,12,0,0,17,0,9,0,0,0,10,0,0,0,3,23,25,18,0,0,4,0,0,0,0,0,9,0,0,20,6,24,0,0,0,0,0,12,1,16,14,0,19,0,0,15,3,1,0,9,7,5,0,0,0,0,0,8,11,12,0,0,17,0,0,11,0,16,5,1,0,0,13,0,8,0,0,0,25,0,0,0,10,0,14,0,0,24,2,23};
    //static int arr[]= {0,1,5,0,1,0,2,1,0,1,4,1,2,0,0,0,0,0,0,0,0,6,3,1,6,1,2,0,8,4,1,4,1,5,1,0,2,0,0,0,1,4,0,9,7,1,1,3,1,5,0,0,0,0,0,0,0,0,0,4,1,3,2,1,2,0,0,0,0,6,0,0,0,0,1,5,0,0,0,0,0,0,1,4,1,1,1,7,3,5,1,0,0,0,8,0,1,2,3,1,6,0,0,2,4,0,0,0,1,4,7,1,3,0,0,5,1,5,1,1,0,5,0,0,0,0,0,0,9,4,0,0,6,0,0,0,0,0,0,1,3,0,1,6,5,1,5,0,0,1,2,0,0,0,0,0,0,0,0,9,0,1,1,2,0,8,3,1,0,1,1,0,1,5,0,2,1,2,0,1,1,0,0,1,4,3,5,4,0,0,0,0,9,0,6,3,0,4,0,0,1,3,0,0,1,1,9,1,0,1,2,1,6,2,0,0,1,0,9,0,0,0,0,0,0,1,2,0,8,0,6,7,1,2,8,0,0,1,6,0,0,1,0,0,1,3,0,0,0,5,0,0,5,0,0,0,3,0,4,6,0,1,1,5,0,0,0,0,0,0,9,1,6,0,1,4,0,1,1,0,0,2,0,0,0,1,0,8,0,1,4,0,0,0,1,3,9,0,4,1,2,1,1,8,0,0,2,0};
    //idx=0;
    /*
    for (int i = 0; i < NN*NN; i++)
    {
        if(arr[i]==0){head[idx]=i,idx++;}
        table[i]=arr[i];
        printf("%d\n", arr[i]);
    }
    print_table(table, NN);
    printf("%d\n",idx);

    

    //head[0]=2*NN+2;
    //head[NN*2+2]=0;
    shuffle(head,idx,5);
    

   print_table(table,NN);
   printf("\n%d\n",idx);
   print_table(head,NN);
*/
    // Run test
    int* table_cpy=(int*)malloc(NN*NN*sizeof(int));
    double start=get_wall_seconds();
        for (int i = 0; i < 10; i++)
        {
            
            nr_sol=0;
            cpy_table(table,table_cpy,NN);
            solve_table(table_cpy, N, head, idx, 0);  
        }
    double end=get_wall_seconds();
   

    if (check(table_cpy, N))
    {
        printf("%lf\n",(end-start));
    }
    else{
        print_table(table_cpy,NN);
        printf("Generation failed");
        printf("%lf\n",(end-start));
    }

    data[NN*NN*2]=idx;
    save_data(data,filename,NN);   

    free(table_cpy);
    free(data);
}