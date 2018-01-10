#define M_PI           3.14159265358979323846

#include "circle.h"

float getArea(int radius) {
    return M_PI * radius * radius;
}

float getCircumference(int radius) {
    return 2 * M_PI * radius;
}
