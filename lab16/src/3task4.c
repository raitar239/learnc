#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* t_func(void* arg) {
    char** words = (char**)arg;
    for (int i=0; i<4; i++) {
        printf("[Child thread %ld] %s\n", pthread_self()%1000, words[i]);
        sleep(1);   // add sleep
    } 
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

    // sleep before death
    printf("[Main] Wait 2 seconds before canceling threads ...\n");
    sleep(2);

    // add cancel threads
    for (int i=0; i<4; i++) {
        printf("[Main] Cancel thread %ld\n", tid[i]%1000);
        pthread_cancel(tid[i]);
    }

    for (int i=0; i<4; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
