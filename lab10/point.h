#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double x;
    double y;
    double z;
} Point;

Point* createPoint(double x, double y, double z);
void printPoint(Point* p, char* name);
double calculateDistance(Point* p1, Point* p2);
void freePoint(Point* p);