#include <stdlib.h>
#include <time.h>
#include "warehouse.h"

int main() {
    srand(time(NULL));

    Product small[N];
    fillArray(small, N);
    
    printArray(small, N, "BEFORE SORT (N=5)"); 
    bubbleSort(small, N);
    printArray(small, N, "AFTER SORT (N=5)");

    measureSort(100);
    measureSort(10000);
    measureSort(100000);

    return 0;
}