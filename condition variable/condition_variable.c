#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;

int  buffer[5];
bool valid[5] ={0,0,0,0,0};

void *producer(void *arg){
    for (int i=0; i<5;i++){
         sleep(1);
        pthread_mutex_lock(&lock);
        buffer[i] =i;
        valid[i]=true;
        printf("Thread [Producer] |  Buffer[%d] : %d\n",i,buffer[i]);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *consumer(void *arg){
    for(int i=0;i<5;i++){
        pthread_mutex_lock(&lock);
        while(!valid[i]){
            printf("Waiting for signal \n");
            pthread_cond_wait(&cond, &lock);
        }
        printf("Thread [consumer] |  Buffer[%d] : %d\n",i,buffer[i]);
        pthread_mutex_unlock(&lock);
    }
}

int main(){
    pthread_t t1,t2;
    pthread_create(&t1,NULL,producer,NULL);
    pthread_create(&t2,NULL,consumer,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}