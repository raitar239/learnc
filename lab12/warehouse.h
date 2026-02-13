#ifndef WAREHOUSE_H
#define WAREHOUSE_H
#define N 5
#include <sys/time.h>

typedef struct {
    int article;
    char name[30];
    int quantity;
    float price;
} Product;

void fillArray(Product arr[], int size);
void printArray(Product arr[], int size, char* title);
void bubbleSort(Product arr[], int size);
void measureSort(int size); 
#endif