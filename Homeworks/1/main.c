
#include <stdio.h>        // printf
#include "circle.h"    // getArea, getCircumference

// A function that computes/displays the area and circumference
// of a circle
void compute(int radius) {
    printf("Area of circle with radius %d: %f\n", 
           radius, getArea(radius));
    printf("Circumference of circle with radius %d: %f\n", 
           radius, getCircumference(radius));
}

int main(void) {
    compute(1);
    compute(5);
    compute(10);

    return 0;
}
