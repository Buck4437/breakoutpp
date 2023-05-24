#include "rect.h"
#include "math_utils.h"

// Creates a rect given a center and the rect's size (static function).
// center: The center coordinates of the rect.
// size: The width and height of the rect.
Rect Rect::get_rect(Vector2 center, Vector2 size) {
    double x1 = center.x - size.x / 2;
    double x2 = center.x + size.x / 2;
    double y1 = center.y - size.y / 2;
    double y2 = center.y + size.y / 2;
    return {x1, y1, x2, y2};
}

// Returns if a point is contained within this rect.
// pos: The point to check.
bool Rect::contains_point(Vector2 pos) {
    return pos.on_top_right_of(pos1) && pos2.on_top_right_of(pos);
}

// Returns if another rect is contained entirely by this rect.
// other: The rect to check.
bool Rect::contains_rect(Rect other) {
    return other.pos1.on_top_right_of(pos1) && pos2.on_top_right_of(other.pos2);
}

// Returns the top-left coordinates of the rect.
Vector2 Rect::top_left() {
    return {pos1.x, pos2.y};
}

// Returns the top-center coordinates of the rect.
Vector2 Rect::top_center() {
    return {avg(pos1.x, pos2.x), pos2.y};
}

// Returns the top-right coordinates of the rect. (Same as pos2.)
Vector2 Rect::top_right() {
    return pos2;
}

// Returns the center coordinates of the rect.
Vector2 Rect::center() {
    return {avg(pos1.x, pos2.x), avg(pos1.y, pos2.y)};
}

// Returns the bottom-left coordinates of the rect. (Same as pos1.)
Vector2 Rect::bottom_left() {
    return pos1;
}

// Returns the bottom-center coordinates of the rect.
Vector2 Rect::bottom_center() {
    return {avg(pos1.x, pos2.x), pos1.y};
}

// Returns the bottom-right coordinates of the rect.
Vector2 Rect::bottom_right() {
    return {pos2.x, pos1.y};
}

// Moves this rect and returns a new rect at the new position.
// vector: The direction and distance to move.
Rect Rect::translate(Vector2 vector) {
    return {pos1.add(vector), pos2.add(vector)};
}