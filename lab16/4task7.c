#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t c_mutex = PTHREAD_MUTEX_INITIALIZER;      // child
pthread_mutex_t p_mutex = PTHREAD_MUTEX_INITIALIZER;      // parent

void *t_func(void *arg) {
    char **words = (char **)arg;
    for (int i = 0; i < 4; i++) {
        pthread_mutex_lock(&c_mutex);       // waiting for permission
        printf("[Child thread %ld] %s\n", pthread_self() % 1000, words[i]);
        sleep(1);
        pthread_mutex_unlock(&p_mutex);     // pass to parent
    }
    return NULL;
}

int main(void) {
    pthread_t tid;
    char *child_words[] = {"One", "Two", "Three", "Four"};
    char *parent_words[] = {"Morning", "Day", "Evening", "Night"};
    
    pthread_mutex_lock(&c_mutex);       // first block child
    pthread_create(&tid, NULL, t_func, child_words);

    for (int i = 0; i < 4; i++) {
        pthread_mutex_lock(&p_mutex);       // waiting for permission
        printf("[Parent thread %ld] %s\n", pthread_self()%1000, parent_words[i]);
        sleep(1);
        pthread_mutex_unlock(&c_mutex);     // pass to child
    }
    pthread_join(tid, NULL);

    // cleaning
    pthread_mutex_destroy(&p_mutex);
    pthread_mutex_destroy(&c_mutex);

    return 0;
}