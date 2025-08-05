/*custom semaphore
- temaphore [in tem_wait ] first checks the condition s.value <=0 , then decrements the value
- temaphore [in tem_post ] first increments the value then signals the waiting threads */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define FG_BBLUE    "\033[94m"
#define FG_BBLACK   "\033[90m"
#define FG_BRED     "\033[91m"
#define FG_BGREEN   "\033[92m"
#define FG_BYELLOW  "\033[93m"
#define RESET       "\033[0m"
#define BOLD        "\033[1m"

typedef struct _tem_t {
    int value;
    pthread_cond_t cond;
    pthread_mutex_t lock;
    }_tem_t;

void tem_init(_tem_t* t, int value)
    {
    t->value = value;
    pthread_mutex_init(&t->lock, NULL);
    pthread_cond_init(&t->cond, NULL);
    }

void tem_wait(_tem_t* t)
    {
    pthread_mutex_lock(&t->lock);
    while (t->value <= 0) {
        pthread_cond_wait(&t->cond, &t->lock);
    }
    t->value--;
    pthread_mutex_unlock(&t->lock);
    }

void tem_post(_tem_t* t)
    {
    pthread_mutex_lock(&t->lock);
    t->value++;
    pthread_cond_signal(&t->cond);
    pthread_mutex_unlock(&t->lock);
    }

_tem_t custom_l;
int counter = 0;

void* mythread(void* arg)
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
    for (int i = 0; i < 50; i++) {
        tem_wait(&custom_l);
        counter++;
        tem_post(&custom_l);
        printf("%scounter Updated by thread[%d]%s : %d\n",t_color, i,RESET, counter);
        }
    return NULL;
    }
int main()
    {
    int count = 5;
    pthread_t t[count];
    int t_no[count];
    tem_init(&custom_l, 1);
    printf("Main begin\n");
    for (int i = 0; i < count; i++) {
        t_no[i] = i;
        pthread_create(&t[i], NULL, mythread, &t_no[i]);
        }
    for (int i = 0;i < count;i++) {
        pthread_join(t[i], NULL);
        }
    printf("\n Counter [final] :%d\n", counter);
    printf("Main End\n");
    return 0;
    }