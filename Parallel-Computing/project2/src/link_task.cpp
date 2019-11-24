#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

#define N 5
#define FS 38
#define NMAX 10
#define NUM_THREADS 2
struct node {
    int data;
    int fibdata;
    struct node* next;
};
struct node* init_list(struct node * p);
int fib(int n);
void processwork(struct node * p);

int fib(int n){
    int x, y;
    if (n < 2){
        return n;
    }else{
        x = fib(n - 1);
        y = fib(n - 2);
        return x + y;
    }
}

void processwork(struct node* p){
    p->fibdata = fib(p->data);
}

struct node* init_list(struct node * p){
    struct node* head = NULL;
    struct node* temp = NULL;

    head = (node*)malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for(int i = 0; i < N; i++){
        temp = (node*)malloc(sizeof(struct node));
        p->next = temp;
        p = temp;
        p->data = FS + i + 1;
        p->fibdata = i + 1;
    }
    p->next = NULL;
    return head;
}


int main(){
    double start_time, end_time;
    printf("Process linked list\n");
    printf("Each linked list node will be processed by function processwork() \n");
	omp_set_num_threads(NUM_THREADS);

    struct node *p = NULL;
    struct node *temp = NULL;
    struct node *head = NULL;
    struct node* parr[NMAX];
    int count = 0;

    p = init_list(p);
    head = p;

    start_time = omp_get_wtime();

    while(p!=NULL){
        processwork(p);
        p = p->next;
    }
    
    end_time = omp_get_wtime();
    
    printf("serial Compute Time: %f seconds\n", end_time - start_time);

    start_time = omp_get_wtime();
    
#pragma omp parallel
{
#pragma omp single
{
    printf(" %d threads \n", omp_get_num_threads());
    p = head;
    while(p){
#pragma omp task firstprivate(p)
{
        processwork(p);
}
        p = p->next;
    }
}
}    
    end_time = omp_get_wtime();
    p = head;
    while(p != NULL){
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);
    printf("Compute Time: %f seconds\n", end_time - start_time);
    return 0;
}