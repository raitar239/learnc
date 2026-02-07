#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define N 5

typedef struct {
    int article;
    char name[30];
    int quantity;
    float price;
} Product;

void fillArray(Product arr[], int size) {
    const char* names[] = {"Milk", "Bread", "Sugar", "Salt", "Rice", "Tea", "Coffie", "Cookies", "Chockolate", "Water"};
    for (int i = 0; i < size; i++) {
        arr[i].article = 1000 + rand() % 9000;                    // 1000-9999
        strcpy(arr[i].name, names[rand() % 10]);                  // random name
        arr[i].quantity = 1 + rand() % 100;                       // 1-100
        arr[i].price = (10.0 + rand() % 990) / 10.0 + .09;        // 1.09 - 100.09
    }
}

void printArray(Product arr[], int size, char* title) {
    printf("\n%31s\n", title);
    printf("=====================================================\n");
    printf("| %-8s | %-12s | %-10s | %-10s |\n", "Article", "Name", "Quantity", "Price");
    printf("=====================================================\n");
    for (int i = 0; i < size; i++) {
        printf("| %-8d | %-12s | %-10d | %-10.2f |\n", arr[i].article, arr[i].name, arr[i].quantity, arr[i].price);
    };
    printf("=====================================================\n");
}

void bubbleSort(Product arr[], int size) {
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-i-1; j++) {
            if (arr[j].quantity > arr[j+1].quantity) {
                Product temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main() {
    srand(time(NULL));

    Product warehouse[N];
    fillArray(warehouse, N);

    printArray(warehouse, N, "BEFORE SORT");
    bubbleSort(warehouse, N);
    printArray(warehouse, N, "AFTER SORT");

    return 0;

}