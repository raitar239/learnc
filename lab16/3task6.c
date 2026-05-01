#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* sleep_sort(void* arg) {
    int value = *(int*)arg;

    usleep(value * 10000); // usleep takes microseconds, so * 10k us
    printf("%d ", value);
    fflush(stdout);      // elements are displayed immediately
    return NULL;
}

int main() {
    int arr[] = {15, 3, 8, 20, 1, 5, 12, 2};
    int n = sizeof(arr) / sizeof(arr[0]);
    pthread_t tid[n];

    printf("Original array: ");
    for (int i=0; i<n; i++) printf("%d ", arr[i]);

    printf("\nSorted array:   ");
    for (int i=0; i<n; i++) {
        pthread_create(&tid[i], NULL, sleep_sort, &arr[i]); // pass address arr[i]!
    }

    for (int i=0; i<n; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("\n");
    return 0;
}