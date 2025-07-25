#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define FG_BBLUE    "\033[94m"
#define FG_BBLACK   "\033[90m"
#define FG_BRED     "\033[91m"
#define FG_BGREEN   "\033[92m"
#define FG_BYELLOW  "\033[93m"
#define BOLD        "\033[1m"
#define RESET       "\033[0m"

volatile int counter = 0;
int random_num;
sem_t s;

void* mythread(void* arg) {
    int t_name = *((int*)arg);
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
    }
    // printf("Thread: Begin\n");
    for (int i = 0;i < random_num;i++) {
        sem_wait(&s);
        printf("\nlock aquired by [%sthread %c%s]\n",t_color, t_name,RESET);
        counter++;
        sem_post(&s);
        }
    return NULL;
    }

int main() {
    srand(time(NULL));
    random_num = rand() % 101;

    printf("%sMain : Begin\n%s",BOLD,RESET);
    sem_init(&s, 0, 1);
    pthread_t t[4];
    int t_id[4];
    for (int i = 0;i < 4;i++) {
        t_id[i] = 65+i;
        pthread_create(&t[i], NULL, mythread, &t_id[i]);
        }
    for (int i = 0;i < 4;i++) {
        pthread_join(t[i], NULL);
        }
    printf("Counter :%d\n", counter);
    printf("%s\nMain : End\n%s",BOLD,RESET);

    return 0;
    }