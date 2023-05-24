#include "math_utils.h"

#include <cmath>

#define M_PI 3.14159265358979323846

// Converts an angle from degrees to radians.
// deg: The angle in degrees.
double from_deg(double deg) {
    return deg * (M_PI / 180.0);
}

// Converts an angle from radians to degrees.
// rad: The angle in radians.
double to_deg(double rad) {
    return rad * (180.0 / M_PI);
}

// Performs linear interpolation between two numbers and returns the result.
// a: The starting point.
// b: The ending point.
// t: The ratio of the distance between a and the output, to the distance between a and b.
// Example: lerp(5, 15, 0.3) returns 8 because if a point on the number line starts from 5
// and moves 30% of the way towards 15, it would land on 8.
double lerp(double a, double b, double t) {
    return a + t * (b - a);
}

// Returns the average of 2 numbers.
double avg(double a, double b) {
    return (a + b) / 2;
}