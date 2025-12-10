#include "point.h"

int main() {
    double x1, y1, z1, x2, y2, z2;
        
    printf("Type cord A: ");
    scanf("%lf %lf %lf", &x1, &y1, &z1);
    
    printf("Type cord B: ");
    scanf("%lf %lf %lf", &x2, &y2, &z2);
    
    Point* pointA = createPoint(x1, y1, z1);
    Point* pointB = createPoint(x2, y2, z2);
    
    printf("\n");
    printPoint(pointA, "A");
    printPoint(pointB, "B");
    
    double distance = calculateDistance(pointA, pointB);
    printf("\nDistance A - B: %.4f\n", distance);
    
    freePoint(pointA);
    freePoint(pointB);
    
    return 0;
}