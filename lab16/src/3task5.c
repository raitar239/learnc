#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void cleanup_handler(void* arg) {
    long tid_c = (long)arg;
    printf("[Cleanup] Thread %ld is canceling now!\n", tid_c);
}

void* t_func(void* arg) {
    char** words = (char**)arg;
    long tid_c = pthread_self() % 1000;

    // registering the handler. two argument need for function
    pthread_cleanup_push(cleanup_handler, (void*)tid_c);

    for (int i=0; i<4; i++) {
        printf("[Child thread %ld] %s\n", pthread_self()%1000, words[i]);
        sleep(1);
    } 

    // remove the handler
    pthread_cleanup_pop(0);
    return NULL;
}

int main() {
    pthread_t tid[4];
    char* words[4][4] = {
        {"One", "Two", "Three", "Four"},
        {"Red", "Green", "Blue", "Brown"},
        {"Apple", "Banana", "Cherry", "Kiwi"},
        {"Dog", "Cat", "Cow", "Pig"}
    };

    for (int i=0; i<4; i++){
        pthread_create(&tid[i], NULL, t_func, words[i]);
    }

    printf("[Main] Wait 2 seconds before canceling threads ...\n");
    sleep(2);

    for (int i=0; i<4; i++) {
        printf("[Main] Requesting cancel for thread %ld\n", tid[i]%1000);
        pthread_cancel(tid[i]);
    }

    for (int i=0; i<4; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
