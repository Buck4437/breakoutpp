#include "vector2.h"
#include "math_utils.h"

#include <math.h>

// Returns the magnitude of the vector.
double Vector2::magnitude() {
    return std::hypot(x, y);
}

// Returns whether this point is on the top-right region of another point.
// More precisely, it checks if the x-coord of this point >= x-coord of the other point
// and if y-coord of this point >= y-coord of the other point.
// other: The other point.
bool Vector2::on_top_right_of(Vector2 other) {
    return x >= other.x && y >= other.y;
}

// Adds 2 vectors and returns the result.
// other: The other vector to add to this vector.
Vector2 Vector2::add(Vector2 other) {
    return {x + other.x, y + other.y};
}

// Translates the point horizontally and returns the new point.
// x: The horizontal distance to move (positive -> right, negative -> left).
Vector2 Vector2::add_x(double x) {
    return {this->x + x, y};
}

// Translates the point vertically and returns the new point.
// y: The vertical distance to move (positive -> up, negative -> down).
Vector2 Vector2::add_y(double y) {
    return {x, this->y + y};
}

// Multiplies the vector and return the result.
// scalar: The scaling factor.
Vector2 Vector2::scale(double scalar) {
    return {x * scalar, y * scalar};
}

// Flips the vector horizontally (along the y-axis) and returns the result.
Vector2 Vector2::horizontal_flip() {
    return {-x, y};
}

// Flips the vector vertically (along the x-axis) and returns the result.
Vector2 Vector2::vertical_flip() {
    return {x, -y};
}

// Flips the vector diagonally / Rotate the point by 180 degrees about the origin and returns the result.
Vector2 Vector2::flip() {
    return {-x, -y};
}

// Returns the mid-point of two points (static method).
// a: The first point.
// b: The second point.
Vector2 Vector2::midpoint(Vector2 a, Vector2 b) {
    return {avg(a.x, b.x), avg(a.y, b.y)};
}