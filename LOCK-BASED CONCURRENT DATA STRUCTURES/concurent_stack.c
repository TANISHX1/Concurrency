// concurrent implementation of stack using arrays
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_STACK 10
int stack[MAX_STACK];
int stack_ptr = -1;
pthread_mutex_t stack_mutex;
void push(int key)
    {
    pthread_mutex_lock(&stack_mutex);
    if (stack_ptr == MAX_STACK - 1) {
        puts("[Error] | Stack overflow - push failed");
        pthread_mutex_unlock(&stack_mutex);
        return;
        }
    stack[++stack_ptr] = key;
    printf("Pushed: %d, stack_ptr: %d\n", key, stack_ptr);
    pthread_mutex_unlock(&stack_mutex);
    }
int pop()
    {
    int value = -1;
    pthread_mutex_lock(&stack_mutex);
    if (stack_ptr < 0) {
        puts("[Error] | Stack underflow - pop failed");
        pthread_mutex_unlock(&stack_mutex);
        return value;
        }
    value = stack[stack_ptr];
    stack[stack_ptr--] = '\0';
    printf("Popped: %d, stack_ptr: %d\n", value, stack_ptr);
    pthread_mutex_unlock(&stack_mutex);
    return value;
    }
void* thread_push_pop(void* arg)
    {
    for (int i = 0; i < 3; i++) {
        push(rand() % 100);
        }
    for (int i = 0; i < 2; i++) {
        pop();
        }
    return NULL;
    }
int main()
    {
    pthread_mutex_init(&stack_mutex, NULL);
    puts("--- Initial Push Operations ---");
    for (int i = 0; i < MAX_STACK; i++) {
        push((i + 1) * 8);
        }
    puts("\n--- Stack Content After Initial Pushes ---");
    pthread_mutex_lock(&stack_mutex);
    for (int i = 0; i <= stack_ptr; i++) {
        printf("Stack[%d] | value : %d\n", stack_ptr - i, stack[stack_ptr - i]);
        }
    pthread_mutex_unlock(&stack_mutex);
    puts("\n--- Pop Operations ---");
    for (int i = 0; i < 5; i++) {
        pop();
        }
    puts("\n--- Stack Content After Pops ---");
    pthread_mutex_lock(&stack_mutex);
    for (int i = 0; i <= stack_ptr; i++) {
        printf("Stack[%d] | value : %d\n", stack_ptr - i, stack[stack_ptr - i]);
        }
    pthread_mutex_unlock(&stack_mutex);
    puts("\n--- Demonstrating Concurrent Access with Threads ---");
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_push_pop, NULL);
    pthread_create(&thread2, NULL, thread_push_pop, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    puts("\n--- Stack Content After Concurrent Operations ---");
    pthread_mutex_lock(&stack_mutex);
    if (stack_ptr >= 0) {
        for (int i = 0; i <= stack_ptr; i++) {
            printf("Stack[%d] | value : %d\n", stack_ptr - i, stack[stack_ptr - i]);
            }
        }
    else {
        puts("Stack is empty.");
        }
    pthread_mutex_unlock(&stack_mutex);
    pthread_mutex_destroy(&stack_mutex);
    return 0;
    }
