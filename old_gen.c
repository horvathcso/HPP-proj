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

void create_table(int* table, int NN, int* head, int* idx)
// TO DO 
{
    int i,j,rnd;
    for(i = 0; i < NN; i++)
        for(j=0; j < NN; j++){
            rnd= rand()%100;
            if(rnd<5)//Not empty
            {table[i*NN+j] = rand() % (NN) + 1;}
            else{ // empty
                table[i*NN+j] = 0;
                head[(*idx+1)]=i*NN+j;
                (*idx)++;}}
}

void print_table(int* table, int NN)
{
    int i,j;
    for(i = 0; i < NN; i++){
        for(j=0; j < NN; j++)
            printf("%d ",table[i*NN+j]);
        printf("\n");}
}

void write_table(int* table, int NN){
    FILE *fp = fopen("table.txt", "w");
    if (fp) {
        int i,j;
        for(i = 0; i < NN; i++){
            for(j=0; j < NN; j++)
                if(j!=NN-1)
                fprintf(fp, "%d \t",table[i*NN+j]);
                else 
                fprintf(fp, "%d",table[i*NN+j]);
            fprintf(fp,"\n");}
        fclose(fp);
    }
}

int validate_table(int* table, int id, int N){
    int NN=N*N; int x = id/(NN); int y = id%(NN);
    for(int i=0;i<NN;i++){
            if(table[x*NN+y] == table[i*NN+y] && x != i)
            return 0;}
    for(int i=0;i<NN;i++){
            if(table[x*NN+y] == table[x*NN+i] && y != i)
            return 0;
            }
    for(int i=-(x%N); i<N-(x%N);i++){
        for (int j = -(y%N); j  < N-(y%N); j++){
            if(table[x*NN+y] == table[(x+i)*NN+(y+j)] && (i!=0 || j!=0))
            return 0;}}
    return 1;}

int start_validation(int* table, int N){
    int NN=N*N;
    for (int i = 0; i < NN; i++)
    {
        for (int j = 0; j < NN; j++)
        {
                for (int k = j; k < NN; k++) // row column
                {
                    if (table[i*NN+j]!=0 && table[i*NN+j]==table[i*NN+k] && j!=k)
                        return 0;
                    if(table[j*NN+i] !=0 && table[j*NN+i]==table[k*NN+i] && j!=k)
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
                    if (table[N*(i*N+k/N)+(j*N+k%N)]!=0 && table[N*(i*N+k/N)+(j*N+k%N)]==table[N*(i*N+l/N)+(j*N+l%N)])
                    {
                        return 0;
                    }
                    
                }
                
            }
            
            /* code */
        }
        
    }
    
    return 1;
}

int nr_sol;
int highest_idx;
int generate_table(int* table, int N, int* head, int idx){
        if(idx==0){if(nr_sol==1){print_table(table,N*N); nr_sol++; return 1;} else{nr_sol++; print_table(table,N*N);
            printf("\n");return 0;}}
        int id=head[idx];
        for(int i=1; i<N*N+1;i++){
            table[id]=i;
            if (validate_table(table, id, N)){
                if(generate_table(table, N, head, idx-1))
                {
                    if(idx < highest_idx && nr_sol==2)
                        {table[head[idx]]=0;printf("%d ",idx);}
                    return 1;
                }
            }
            if(nr_sol==1 && highest_idx<idx){
                highest_idx=idx;
            }
        }
        table[id]=0;
        return 0;
}

int double_solution(int* table, int N, int* head, int idx){
        if(idx==0){if(nr_sol==1){  return 1;} else{nr_sol++; 
            printf("\n");return 0;}}
        int id=head[idx];
        for(int i=1; i<N*N+1;i++){
            table[id]=i;
            if (validate_table(table, id, N)){
                if(double_solution(table, N, head, idx-1))
                {
                    return 1;
                }
            }
        }
        table[id]=0;
        return 0;
}

int solve_table(int* table, int N, int* head, int idx){
        if(idx==0){return 1;}
        int id=head[idx];
        for(int i=1; i<N*N+1;i++){
            table[id]=i;
            if (validate_table(table, id, N))
                if(solve_table(table, N, head, idx-1))
                    return 1;}
        table[id]=0;
        return 0;}

void shuffle(int* old, int* new, int n){
    for (int i = 1; i <= n; i++) //copy
    {
        new[i]=old[i];
    }
    srand(time(0));
    for (int i = 1; i < n; ++i)
    {
    int j = rand() % (n-i) + i+1;
    int tmp = new[i];
    new[i] = new[j];
    new[j] = tmp;
    }
    
}

int main(int argc, char* argv[]) {
    srand(time(0));

    if(argc != 3) {
        printf("Please give 2 argument: N (number of elements to sort), save_table (bool).\n");
        return -1;
    }
    int N = atoi(argv[1]);
    int NN=N*N;
	const int savetable = atoi(argv[2]);

    //Create table and table operations
    int* table=(int*)malloc(NN*NN*sizeof(int));
    int* head=(int*)malloc(NN*NN*sizeof(int));
    int idx=0;
    create_table(table,NN, head, &idx);
    if(savetable){write_table(table,NN);}
    printf("\n");
    
    // generate table
    // guarantee good starting table 
    double s=get_wall_seconds();
    while ((! start_validation(table, N)))
    {
        idx=0;
        create_table(table,NN,head, &idx);
    }
    nr_sol=0;
    if (double_solution(table,N,head,idx))
    {
        printf("Starting generating function\n");
        highest_idx=0;
        int* head_shuffle = (int*)malloc((idx+1)*sizeof(int));
        shuffle(head,head_shuffle,idx);
        generate_table(table, N,head_shuffle, idx);
        idx=highest_idx-1;
        if(nr_sol==2){
            nr_sol=1; int id=1;
            while (nr_sol==1)
        {
        printf("New loop started, hid: %d\n", highest_idx);
        for (int i = 0; i < id; i++)
        {
            table[head_shuffle[i+1]]=0;
        }
        highest_idx=0;
        nr_sol=0;
        generate_table(table, N,head_shuffle, idx);
        printf("\n%d, %d\n",table[head_shuffle[highest_idx]],head_shuffle[highest_idx]);
        print_table(table,NN);
        }
        }
        
    }
    else{
        printf("A generated table\n");
        print_table(table,NN);
    }
    double e=get_wall_seconds();
        printf("\nGeneration took %lf\n", (e-s));
        printf("\n");
    
    
    


    int res=-1; nr_sol=0;
    // run function
    double start =get_wall_seconds();
    if(start_validation(table, N))
        res=solve_table(table, N, head, idx);
    else
    printf("Start validation failed \n");
    double end =get_wall_seconds();


    if (res)
        print_table(table,NN);
    else
        printf("no soliution");
    
    printf("\n res: %d", res);

    printf("\n nr_sol: %d", nr_sol);

    printf("\nThe funtion took: %lfs\n", (end-start));
    free(head);
    free(table);
}