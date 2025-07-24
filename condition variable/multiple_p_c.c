#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#define B_SIZE 10
#define FG_BGREEN   "\033[92m"
#define FG_BYELLOW  "\033[93m"
#define BOLD        "\033[1m"
#define RESET       "\033[0m"
#define LOOP_MAX 40
#define PRODUCERS 4
#define CONSUMERS 4

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t b_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t b_empty = PTHREAD_COND_INITIALIZER;

bool output_delay;
int buffer[B_SIZE];
int in = 0, out = 0, count = 0, max_product = 0;

void* producer(void* arg) {
    int p_name = *((int*)arg);
    for (int i = 0;i < LOOP_MAX;i++) {
        pthread_mutex_lock(&lock);
        while (count == B_SIZE) {
            pthread_cond_wait(&b_empty, &lock);
            printf("\n");
        }
        buffer[in] = i;
        printf("%sThread[Producer %d]  |  Added %d in buffer[%d]  |  Buffer Value : %d%s\n", FG_BGREEN, p_name, i, in, buffer[in], RESET);
        in = (in + 1) % B_SIZE;
        count++;
        max_product++;
        pthread_cond_signal(&b_full);
        pthread_mutex_unlock(&lock);
        if (output_delay)
        {
            usleep(50000);
        }
    }
    return NULL;
}

void* consumer(void* arg) {
    int t_name = *((int*)arg);
    for (int i = 0;i < LOOP_MAX;i++) {
        pthread_mutex_lock(&lock);
        while (count == 0) {
            if (max_product >= PRODUCERS * LOOP_MAX) {
                pthread_cond_broadcast(&b_full);
                pthread_exit(NULL);
            }
            pthread_cond_wait(&b_full, &lock);
            printf("\n");
        }
        printf("%sThread[Consumer %d]  |  Total consumed : %d    |  Removed %d from buffer[%d]%s \n", FG_BYELLOW, t_name, i, buffer[out], out, RESET);
        out = (out + 1) % B_SIZE;
        count--;
        pthread_cond_signal(&b_empty);
        pthread_mutex_unlock(&lock);
        if (output_delay)
        {
            usleep(100000);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    //change this output_delay flag from false to true to get delay in output.
    output_delay = true;

    pthread_t p[PRODUCERS], c[CONSUMERS];
    int p_id[PRODUCERS];
    int c_id[CONSUMERS];
    for (int i = 0;i < 4;i++) {
        p_id[i] = i;
        c_id[i] = i;
        pthread_create(&p[i], NULL, producer, &p_id[i]);
        pthread_create(&c[i], NULL, consumer, &c_id[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(p[i], NULL);
        pthread_join(c[i], NULL);
    }

    printf("%s\n  EXECUTION DONE!..%s\n\n", BOLD, RESET);
    return 0;
}