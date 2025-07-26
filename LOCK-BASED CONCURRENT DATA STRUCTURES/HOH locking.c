/*implementation of Hand over hand locking
-this code can stuck sometimes(due to potential deadlock)*/
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#define FG_BBLUE    "\033[94m"
#define FG_BBLACK   "\033[90m"
#define FG_BRED     "\033[91m"
#define FG_BGREEN   "\033[92m"
#define FG_BYELLOW  "\033[93m"
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define CPU_NUM 12
#define SEARCH__ 10
#define MAX_NODES 20

pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;

typedef struct node {
    int node_no;
    int value;
    pthread_mutex_t lock;
    struct node* next;
    } node;

//gloable variable 
int counter = 0;

typedef struct list {
    node* head;
    }list;

// initilises the list 
void init_list(list* headlist) {
    headlist->head = NULL;
    }

// creates node and link with list

void add_node(list* l) {
    node* new_node = (node*)malloc(sizeof(node));
    pthread_mutex_init(&new_node->lock, NULL);
    new_node->value = rand() % 200;
    pthread_mutex_lock(&glock);
    new_node->node_no = counter++;
    new_node->next = l->head;
    l->head = new_node;
    pthread_mutex_unlock(&glock);
    }

// search value in linked list ( implementation of hand in hand locking )

node* search(list* l, int f_value) {
    node* current = l->head;
    node* next = current->next;
    node* temp;
    if (current == NULL) {
        printf("List is Empty\n");
        exit(0);
        }
    
    pthread_mutex_lock(&current->lock);
    while (next != NULL) {
        pthread_mutex_lock(&next->lock);
        if (current->value == f_value) {
            pthread_mutex_unlock(&current->lock);
            pthread_mutex_unlock(&next->lock);
            return current;
            }
        temp = current;
        current = next;
            next = current->next;
            pthread_mutex_unlock(&temp->lock);
        }
    if (current->value == f_value) {
        pthread_mutex_unlock(&current->lock);
        return current;
        }
    pthread_mutex_unlock(&current->lock);
    return NULL;
    }

//global list (shared data )
list headlist;

// creates list 
void* t_node(void* arg) {
    for (int i = 0;i < MAX_NODES;i++) {
        add_node(&headlist);
        }
    return NULL;
    }

//searches the value
void* t_search(void* arg) {
    int t_id = *((int*)arg);
    char* t_color;
    switch (t_id) {
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
    for (int i = 0;i < MAX_NODES;i++) {
        node* found = search(&headlist, rand() % 200);
        if (!found) {
            printf("Not Found \n");
            }
        else {
            printf("\n%s~[Found]\n Node info \n node no. : %d\n node value : %d%s\n\n", t_color, found->node_no, found->value, RESET);
            }
        }
    }

int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    srand(time(NULL));
    init_list(&headlist);
    int count = 4;
    pthread_t t[count];
    int t_id[count];

    //threads creation (linked list creation) 
    for (int i = 0;i < count;i++) {
        t_id[i] = i;
        pthread_create(&t[i], NULL, t_node, NULL);
        }
    for (int i = 0;i < count;i++) {
        pthread_join(t[i], NULL);
        }

    //threads creation for search 
    for (int i = 0;i < count;i++) {
        pthread_create(&t[i], NULL, t_search, &t_id[i]);
        }
    for (int i = 0;i < count;i++) {
        pthread_join(t[i], NULL);
        }

    //sys info 
    gettimeofday(&end, NULL);
    long Exe_time = (end.tv_sec - start.tv_sec) * 1e6;
    Exe_time += (end.tv_usec - start.tv_usec);
    printf("%sTotal Execution Time : %ldus\n",BOLD, Exe_time);
    printf("Total CPU core used  : %d \n", sysconf(_SC_NPROCESSORS_ONLN));
    printf("Done !%s\n",RESET);
    return 0;
    }