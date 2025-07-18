/* scalable counter (data structure)
-- such approuch is used in high-concurrency environment
   - OS kernal
   - high end servers
   - performance-critical things
consist of global counter and local counter
+ global lock(glock) && local locks(llock)
 if threshold value is high then, better perfomance ,but less accuracy (applicable in os kernals)
 if threshold value is less then ,loss performane ,but better or more accurate.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#define CPU_NUM 4

typedef struct _counter
{
    int global;
    pthread_mutex_t glock;
    int lvalue[CPU_NUM];
    pthread_mutex_t llock[CPU_NUM];
    int threshold;
} _counter;

void initilization(_counter *data, int threshold)
{
    data->global = 0;
    pthread_mutex_init(&data->glock, NULL);
    for (int i = 0; i < CPU_NUM; i++)
    {
        data->lvalue[i] = 0;
        pthread_mutex_init(&data->llock, NULL);
    }
    data->threshold = threshold;
}

void update(_counter *data, int thread_id)
{
    int cpu = thread_id % CPU_NUM;
    pthread_mutex_lock(&data->llock[cpu]);
    if (data->lvalue[cpu] >= data->threshold)
    {
        pthread_mutex_lock(&data->glock);
        data->global += data->lvalue[cpu];
        pthread_mutex_unlock(&data->glock);
        data->lvalue[cpu] = 0;
    }
    else
    {
        data->lvalue[cpu] += 1;
    }
    pthread_mutex_unlock(&data->llock[cpu]);
}

int get(_counter *data )
{

    pthread_mutex_lock(&data->glock);
    int value = data->global;
    pthread_mutex_unlock(&data->glock);
    return value;
}
_counter data_t;

void *mythread(void *arg)
{
    int thread_id = *((int *)arg);
    for (int i = 0; i < 1e5; i++)
    {
        update(&data_t, thread_id);
    }
    return NULL;
}

int main()
{
    initilization(&data_t, 100);
    pthread_t threads[10];
    int tid[10];
    for (int i = 0; i < 10; i++)
    {
        tid[i] = i;
        pthread_create(&threads[i], NULL, mythread, &tid[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("Counter [global]       :%d\n", get(&data_t));

}