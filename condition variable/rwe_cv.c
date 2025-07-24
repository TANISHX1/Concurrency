
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define LOOP_MAX 100
#define B_SIZE 10
#define FG_BBLUE    "\033[94m"
#define FG_BBLACK   "\033[90m"
#define FG_BRED     "\033[91m"
#define FG_BGREEN   "\033[92m"
#define FG_BYELLOW  "\033[93m"
#define BOLD        "\033[1m"
#define RESET       "\033[0m"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t b_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t b_full = PTHREAD_COND_INITIALIZER;
int buffer[B_SIZE];
int in = 0, out = 0, count = 0;
int p_i = 0;

void* producer(void* arg) {
    for (;p_i < LOOP_MAX;p_i++) {
        pthread_mutex_lock(&lock);
        while (count == B_SIZE) {
            pthread_cond_wait(&b_empty, &lock);
            printf("\n");
        }
        buffer[in] = p_i;
        printf("Thread[Producer]  |  Added %d in Buffer[%d]  |  Value : %d\n", p_i, in, buffer[in]);
        in = (in + 1) % B_SIZE;
        count++;
        pthread_cond_signal(&b_full);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* consumer(void* arg) {
    int  t_name = *((int*)arg);
    char* t_color;
    switch (t_name)
    {
    case 65:
        t_color = FG_BBLUE;
        break;
    case 66:
        t_color = FG_BGREEN;
        break;
    case 67:
        t_color = FG_BRED;
        break;
    case 68:
        t_color = FG_BYELLOW;
        break;

    default:
        t_color = FG_BBLACK;
        break;
    };

    for (int i = 0;i < LOOP_MAX;i++) {
        pthread_mutex_lock(&lock);
        while (count == 0) {
            if (p_i >= LOOP_MAX) {
                printf("\n\nThread[%sconsumer : %c%s]  | consumed : %d%c\t",t_color,(char)t_name,RESET,i,'%');
                pthread_cond_broadcast(&b_full);
                pthread_mutex_unlock(&lock);
                
                pthread_exit(NULL);
            }
            pthread_cond_wait(&b_full, &lock);
            printf("\n");
        }
        printf("%c\n", (char)(t_name));
        printf("%sThread[Consumer]  |  Removed %d From Buffer[%d]  |  value : %d %s\n", t_color, i, out, buffer[out], RESET);
        out = (out + 1) % B_SIZE;
        count--;
        pthread_cond_signal(&b_empty);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t[5];
    int t_id[4];
    pthread_create(&t[0], NULL, producer, NULL);
    for (int i = 1;i < 5;i++) {
        t_id[i - 1] = 64 + i;
        pthread_create(&t[i], NULL, consumer, &t_id[i - 1]);
    }
    for (int i = 0;i < 5;i++) {

        pthread_join(t[i], NULL);
    }
    printf("\n%s EXECTION DONE !%s\n",BOLD,RESET);
    return 0;
}
