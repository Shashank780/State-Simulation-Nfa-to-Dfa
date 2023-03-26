#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

int newstates;
int initial[100][2][2]={0};
int arr[1000]={0};
int nextnewstate;
int states[1000][1000]={0};
int queue[1000];
int end=0;
int pointer=0;

bool exists();
void swap(int* , int* );
void bubbleSort();
int whicharr();
void middle(int,int, int*);            
void* traverse(void*);             

char w[500];
int n;

__thread int array[500];
pthread_mutex_t mut;

int maxthreads;
int currentthreads;

typedef struct{
    int para1,para2,para3,para4;
    int *para5;
} parameters;

void init(){
    int c=0;
    for(int i=1;i<=n*n;i++){
        int a=(i-1)/n;
        int b=(i-1)%n;
        int z=0;
        if(a-1>=0)initial[i-1][z++][1]=i-n;
        if(a+1<n)initial[i-1][z++][1]=i+n;
        z=0;
        if(b-1>=0)initial[i-1][z++][0]=i-1;
        if(b+1<n)initial[i-1][z++][0]=i+1;
    }
}


bool empty(){
    return end==pointer;
}

void add(int a){
    queue[end++]=a;
}

int rem(){
    return queue[pointer++];
}

void arycpy(int crr[]){
    for(int i=0;i<1000;i++){
        if(crr[i]==0)break;
        arr[i]=crr[i];
    }
}

void findnextstates(int a,int symbol){
    int d=0;
    int crr[1000]={0};
    memset(arr, 0, sizeof(arr));
    if(a<=n*n){
        crr[0]=initial[a-1][0][symbol];
        if(initial[a-1][1][symbol])crr[1]=initial[a-1][1][symbol];
    }
    else{
        int pos;
        for(int i=0;i<1000;i++)if(states[i][0]==a)pos=i;
        for(int i=1;i<1000;i++){
            if(states[pos][i]==0)break;
            int brr[1000]={0};
            brr[0]=initial[states[pos][i]-1][0][symbol];
            if(initial[states[pos][i]-1][1][symbol])brr[1]=initial[states[pos][i]-1][1][symbol];
            for(int j=0;j<1000;j++){
                if(brr[j]==0)break;
                crr[d++]=brr[j];
            }
        }
    }
    arycpy(crr);
}

int main(){
    
    maxthreads=20;
    currentthreads=1;
    FILE *pt;
    pt = fopen("input.txt","r");

    fscanf(pt,"%d", &n);
    fscanf(pt,"%s", w);
    fclose(pt);

    n=n+1;
    init();
    nextnewstate=n*n+1;
    int c=0;
    states[c++][0]=1;
    add(1);
    while(!empty()){
        int q=rem();
        findnextstates(q,0);
        bubbleSort();
        if(!exists()){
            add(nextnewstate);
            states[c++][0]=nextnewstate++;
            for(int i=0;i<1000;i++){
                if(arr[i]==0)break;
                states[c-1][i+1]=arr[i];
            }
        }
        findnextstates(q,1);
        bubbleSort();
        if(!exists()){
            add(nextnewstate);
            states[c++][0]=nextnewstate++;
            for(int i=0;i<1000;i++){
                if(arr[i]==0)break;
                states[c-1][i+1]=arr[i];
            }
        }
    }
    states[0][1]=1;
    newstates=nextnewstate-(n*n);
    
    //    for(int i=0;i<newstates;i++){
    //    for(int j=0;j<1000;j++){
    //        if(states[i][j]==0)break;
    //        printf("%d ",states[i][j]);
    //        printf("\n");
    //   }
    //}
    
    int ultra[newstates][2][2];
    for(int i=0;i<newstates;i++){
        ultra[i][0][0]=states[i][0];
        findnextstates(states[i][0],0);
        bubbleSort();
        ultra[i][1][0]=whicharr();
        ultra[i][0][1]=states[i][0];
        findnextstates(states[i][0],1);
        bubbleSort();
        ultra[i][1][1]=whicharr();
    }
    
    //for(int i=0;i<newstates;i++)printf("%d ",ultra[i][0][0]);
    //printf("\n");
    //    for(int i=0;i<newstates;i++)printf("%d ",ultra[i][1][0]);
    //printf("\n");
    //    for(int i=0;i<newstates;i++)printf("%d ",ultra[i][0][1]);
    //printf("\n");
    //    for(int i=0;i<newstates;i++)printf("%d ",ultra[i][1][1]);
    //printf("\n");
    
    int final[(2*newstates)+1][newstates];
    for(int i=0;i<=2*newstates;i++){
        for(int j=0;j<newstates;j++)final[i][j]=0;
    }
    
    for(int i=0;i<newstates;i++){
        bool fi=false;
        for(int j=1;j<1000;j++){
            if(states[i][j]==0)break;
            if(states[i][j]==n*n)fi=true;
        }
        if(fi)final[0][i]=1;
    }
    for(int i=1;i<=newstates;i++){
        if(ultra[i-1][1][0]==1)final[i][0]=1;
        else final[i][ultra[i-1][1][0]-n*n]=1;
    }
    for(int i=newstates+1;i<=2*newstates;i++){
        if(ultra[i-newstates-1][1][1]==1)final[i][0]=1;
        else final[i][ultra[i-newstates-1][1][1]-n*n]=1;
    }
    
    FILE *f4;
    f4 = fopen("2019B4A70956H_t2.txt","a");

    for(int i=0;i<=2*newstates;i++){
        for(int j=0;j<newstates;j++)fprintf(f4,"%d ",final[i][j]);
        fprintf(f4,"\n");
    }
    fclose(f4);

    n--;	
    pthread_t pth;
    parameters p;
    p.para1 = 0;
    p.para2 = 0;
    p.para3 = 0;
    p.para4 = 0;

    int array[100];
    p.para5 = array;

    pthread_create(&pth,NULL,&traverse,(void *)(&p));
    pthread_join(pth,NULL);

    FILE *ptr;
    ptr = fopen("output.txt","w");
    fprintf(ptr,"\n");
    fclose(ptr);

    return 0;
}

void* traverse(void* arg){

    parameters* args;
    pthread_t thval[2] = {0,0};
    args = (parameters*)arg;
    int x = args->para1;
    int y = args->para2;
    int c = args->para3;
    int ep = args->para4;
    int *point;
    point = args->para5;
    for(int i = 0; i<100; i++)  {
        array[i] = *(point+i);
    }
    char characterpoint = w[ep++];
    array[c] = (((n+1)*y) + x);
    c++;
    middle(0,c, array);
    if(characterpoint == '1')
    {
        if(y-1 >= 0 && y-1 <= n) {
            parameters p;
            p.para1 = x;
            p.para2 = y-1;
            p.para3 = c;
            p.para4 = ep;
            p.para5 = array;
            pthread_mutex_lock(&mut);
            if(currentthreads<maxthreads)  {
                currentthreads++;
                pthread_mutex_unlock(&mut);
                pthread_create(&thval[0],NULL,&traverse,(void *)(&p));
            }
            else  {
                pthread_mutex_unlock(&mut);
                traverse((void *)(&p));
                array[c] = 0;

            }
        }
        if(y+1 >= 0 && y+1 <= n) {
            parameters p;
            p.para1 = x;
            p.para2 = y+1;
            p.para3 = c;
            p.para4 = ep;
            p.para5 = array;
            pthread_mutex_lock(&mut);
            if(currentthreads<maxthreads)  {
                currentthreads++;
                pthread_mutex_unlock(&mut);
                pthread_create(&thval[1],NULL,&traverse,(void *)(&p));
            }
            else  {
                pthread_mutex_unlock(&mut);
                traverse((void *)(&p));
                array[c] = 0;

            }
        }
    }
    else if(characterpoint == '0')
    {
        if(x-1 >= 0 && x-1 <= n) {
            parameters p;
            p.para1 = x-1;
            p.para2 = y;
            p.para3 = c;
            p.para4 = ep;
            p.para5 = array;
            pthread_mutex_lock(&mut);
            if(currentthreads<maxthreads)  {
                currentthreads++;
                pthread_mutex_unlock(&mut);
                pthread_create(&thval[0],NULL,&traverse,(void *)(&p));
            }
            else  {
                pthread_mutex_unlock(&mut);
                traverse((void *)(&p));
                array[c] = 0;

            }
        }
        if(x+1 >= 0 && x+1 <= n) {
            parameters p;
            p.para1 = x+1;
            p.para2 = y;
            p.para3 = c;
            p.para4 = ep;
            p.para5 = array;
            pthread_mutex_lock(&mut);
            if(currentthreads<maxthreads)  {
                currentthreads++;
                pthread_mutex_unlock(&mut);
                pthread_create(&thval[1],NULL,&traverse,(void *)(&p));
            }
            else  {
                pthread_mutex_unlock(&mut);
                traverse((void *)(&p));
                array[c] = 0;

            }
        }
                
    }
    else if(characterpoint == '\0')
    {
        if(x == n && y == n )
            middle(1,c,array);
        else 
            middle(-1,c,array);
    }
    if(thval[0]!=0) {
        pthread_join(thval[0], NULL);
        currentthreads--;
    }
    if(thval[1]!=0)  {
    pthread_join(thval[1], NULL);
    currentthreads--;
    }
}

void middle(int flag, int count, int *array){
  
    if(flag==0){
        FILE *f3;
        f3 = fopen("2019B4A70956H_t1.txt","a");

        int i = 0;
        fprintf(f3, "Exploring path:");
        for(int i = 0; i<count; i++)
        {
            fprintf(f3, "%d     ", *(array+i));
        }
        fprintf(f3, "\n");
        fclose(f3);
        return;
    }
    else if(flag==-1){
        FILE *f2;
        f2 = fopen("2019B4A70956H_t1.txt","a");

        int i = 0;
        fprintf(f2, "Failed at path:");
        for(int i = 0; i<count; i++)
        {
            fprintf(f2, "%d     ", *(array+i));
        }
        fprintf(f2, "\n");
        fclose(f2);
        return;
    }
    else if(flag==1){
        FILE *f1;
        f1 = fopen("2019B4A70956H_t1.txt","a");

        fprintf(f1, "Accepted! Found path:");
        for(int i = 0; i<count; i++)
        {
            fprintf(f1, "%d     ", *(array+i));
        }
        fprintf(f1,"\n");
        fclose(f1);
        exit(0); 
    }
}

bool exists(){
    bool ex=false;
    for(int i=0;i<1000;i++){
        ex=true;
        if(states[i][0]==0)break;
        int c=0;
        for(int j=1;j<1000;j++){
            if(states[i][j]==0){
                if(arr[c]!=0)ex=false;
                break;
            }
            if(arr[c++]==states[i][j])continue;
            ex=false;
            break;
        }
        if(ex==true)return true;
    }
    return false;
}

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort()
{
    int n=0;
    for(int i=0;i<1000;i++){
        if(arr[i]==0)break;
        n++;
    }
    int i, j;
    for (i = 0; i < n-1; i++)
 
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
    int l=1,prev=arr[0];
    for(int k=1;k<n;k++){
        if(arr[k]==prev)continue;
        arr[l++]=arr[k];
        prev=arr[k];
    }
    arr[l]=0;
}

int whicharr(){
    for(int i=0;i<newstates;i++){
        for(int j=1;j<1000;j++){
            if(states[i][j]==0){
                if(arr[j-1]==0)return states[i][0];
                break;
            }
            if(states[i][j]!=arr[j-1])break;
        }
    }
    return 0;
}



