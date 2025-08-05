/*multiple producer - multiple consumer using semaphore ,
 concurrently producer produces and consumer consumes
 */
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX 1
#define count 5
#define FG_BBLUE    "\033[94m"
#define FG_BBLACK   "\033[90m"
#define FG_BRED     "\033[91m"
#define FG_BGREEN   "\033[92m"
#define FG_BYELLOW  "\033[93m"
#define RESET       "\033[0m"
#define BOLD        "\033[1m"


sem_t full, empty, glock;

int buffer[MAX];
int in;
int out;
short int terminate_value = 0;

void* producer(void* arg)
    {
    int t_id = *((int*)arg);
    for (int i = 0;i < 10;i++) {
        sem_wait(&full);
        sem_wait(&glock);
        buffer[in] = i;
        printf("[producer %d] | added %d \n", t_id, buffer[in]);
        in = (in + 1) % MAX;
        sem_post(&glock);
        sem_post(&empty);
        }
    sem_wait(&glock);
    terminate_value++;
    sem_post(&glock);
    return NULL;
    }

void* consumer(void* arg)
    {
    int t_no = *((int*)arg);
    char* t_color;
    switch (t_no) {
        case 0:
            t_color = FG_BBLUE;
            break;
        case 1:
            t_color = FG_BGREEN;
            break;
        case 2:
            t_color = FG_BRED;
            break;
        case 3:
            t_color = FG_BYELLOW;
            break;
        default:
            t_color = FG_BBLACK;
            break;

        }
    while (1) {
         if (terminate_value >= count) {
                sem_post(&empty);
                break;
            }
        sem_wait(&empty);
        sem_wait(&glock);
        out = (out + 1) % MAX;
        sem_post(&glock);
        sem_post(&full);
            printf("%s[Consumer %d] | Removed %s %d\n", t_color, t_no, RESET, buffer[out]);

           
        }
    return NULL;
    }

void init()
    {
    sem_init(&full, 0, MAX);
    sem_init(&empty, 0, 0);
    sem_init(&glock, 0, 1);
    in = 0;
    out = 0;
    }

int main()
    {
    init();
    pthread_t t_c[count], t_p[count];
    int t_id_c[count], t_id_p[count];
    for (int i = 0;i < count;i++) {
        t_id_c[i] = i;
        t_id_p[i] = i;
        pthread_create(&t_p[i], NULL, producer, &t_id_p[i]);
        pthread_create(&t_c[i], NULL, consumer, &t_id_c[i]);
        }
    for (int i = 0;i < count;i++) {
        pthread_join(t_p[i], NULL);
        pthread_join(t_c[i], NULL);
        }
    printf("Done\n");
    }