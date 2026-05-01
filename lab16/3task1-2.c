#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* t_func(void* arg) {
    for (int i=1; i<=5; i++) {
        printf("[Child thread, %ld] stroke %d \n", pthread_self()%1000, i);
    }
    return NULL;
}
int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, t_func, NULL);
    pthread_join(tid, NULL);    // 2 task
    for (int i=1; i<=5; i++) {
        printf("[Parent thread, %ld] stroke %d \n", pthread_self()%1000, i);
    }
    return 0;
}
