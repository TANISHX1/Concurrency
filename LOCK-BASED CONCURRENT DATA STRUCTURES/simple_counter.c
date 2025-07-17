/*single lock counter(data structure)
 -accurate
 -poor scaling in multithreading
*/
#include <stdio.h>
#include <pthread.h>
typedef struct __counter_t
{
    int
        value;
    pthread_mutex_t lock;
} counter_t;
void init(counter_t *c)
{
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}
void increment(counter_t *c)
{
    pthread_mutex_lock(&c->lock);
    
        c->value++;
    pthread_mutex_unlock(&c->lock);
}
void decrement(counter_t *c)
{
    pthread_mutex_lock(&c->lock);
    c->value--;
    pthread_mutex_unlock(&c->lock);
}
int get(counter_t *c)
{
    pthread_mutex_lock(&c->lock);
    int rc = c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}

void *mythread(void *arg)
{
    counter_t *entry = (counter_t *)arg;
    for (int i = 0; i < 1e5; i++)
    {
        increment(entry);
        decrement(entry);
    }
}
int main()
{ 
    counter_t entry;
    init(&entry);
    pthread_t t1, t2, t3, t4, t5;
    printf("%d\n", entry.value);
    pthread_create(&t1, NULL, mythread, &entry);
    pthread_create(&t2, NULL, mythread, &entry);
    pthread_create(&t3, NULL, mythread, &entry);
    pthread_create(&t4, NULL, mythread, &entry);
    pthread_create(&t5, NULL, mythread, &entry);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    printf("%d\n", entry.value);
    return 0;
}