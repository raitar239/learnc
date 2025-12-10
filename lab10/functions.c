#include "point.h"

Point* createPoint(double x, double y, double z) {
    Point* p = malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    p->z = z;
    return p;
}

void printPoint( Point* p,  char* name) {
    printf("Point %s: (%.2f, %.2f, %.2f)\n", name, p->x, p->y, p->z);
}

double calculateDistance( Point* p1,  Point* p2) {
    double dx = p2->x - p1->x;
    double dy = p2->y - p1->y;
    double dz = p2->z - p1->z;
    
    return sqrt(dx * dx + dy * dy + dz * dz);
}

void freePoint(Point* p) {
    free(p);
}