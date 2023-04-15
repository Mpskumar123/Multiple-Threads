#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX 100

pthread_mutex_t lock;
pthread_cond_t cond;

int regular_queue[MAX];
int new_queue[MAX]; 
int regular_front = -1;
int regular_rear = -1;
int new_front = -1;
int new_rear = -1;
int regular_count = 0;
int new_count = 0;
int total_time = 0;
int total_queries = 0;

void *regular_customer(void *arg)
{
    int time = rand() % 10 + 1;
    int id = *(int *)arg;
    pthread_mutex_lock(&lock);
    while (regular_count == MAX)
    {
        printf("Regular Queue is full\n");
        pthread_cond_wait(&cond, &lock);
    }
    regular_rear = (regular_rear + 1) % MAX;
    regular_queue[regular_rear] = id;
    regular_count++;
    printf("Regular Customer %d has arrived\n", id);
    pthread_mutex_unlock(&lock);
    sleep(time);
    pthread_mutex_lock(&lock);
    regular_front = (regular_front + 1) % MAX;
    regular_count--;
    printf("Regular Customer %d has left\n", id);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    total_time += time;
    total_queries++;
}

void *new_customer(void *arg)
{
    int time = rand() % 10 + 1;
    int id = *(int *)arg;
    pthread_mutex_lock(&lock);
    while (new_count == MAX)
    {
        printf("New Queue is full\n");
        pthread_cond_wait(&cond, &lock);
    }
    new_rear = (new_rear + 1) % MAX;
    new_queue[new_rear] = id;
    new_count++;
    printf("New Customer %d has arrived\n", id);
    pthread_mutex_unlock(&lock);
    sleep(time);
    pthread_mutex_lock(&lock);
    new_front = (new_front + 1) % MAX;
    new_count--;
    printf("New Customer %d has left\n", id);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    total_time += time;
    total_queries++;
}

int main()
{
   
    int n;
    printf("Enter the number of customers: ");
    scanf("%d", &n);
    int i;
    int id[n];
    for (i = 0; i < n; i++)
    {
        id[i] = i + 1;
    }
    pthread_t tid[n];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    for (i = 0; i < n; i++)
    {
        int r = rand() % 2;
        if (r == 0)
        {
            pthread_create(&tid[i], NULL, regular_customer, &id[i]);
        }
        else
        {
            pthread_create(&tid[i], NULL, new_customer, &id[i]);
        }
    }
    for (i = 0; i < n; i++)
    {
        pthread_join(tid[i], NULL);
    }
    printf("Total time spent on handling queries: %d\n", total_time);
    printf("Average query time: %f\n", (float)total_time / total_queries);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}
