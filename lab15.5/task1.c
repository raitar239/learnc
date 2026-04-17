#include <stdio.h>
#include <stdlib.h>

void init(int **arr, int n) {           // передаем указатель на указатель
    *arr = malloc(n * sizeof(int));     // память выделяем для указателя 
    for (long i = 0; i < n; ++i) { 
        (*arr)[i] = i;                  // кладем в указатель
    }
}

int main() {
    int *arr = NULL;
    int n = 10;

    init(&arr, n);                      // передаем адрес
    for (int i = 0; i < n; ++i) {
        printf("%d\n", arr[i]);
    }
    free(arr);
    return 0;
}
