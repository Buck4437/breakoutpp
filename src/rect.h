#include "vector2.h"

#ifndef RECT_H_
#define RECT_H_

// Container for any arbitrary rectangle.
// Order: x1, y1, x2, y2
// Constraints: x1 <= x2, y1 <= y2.
struct Rect {
        Vector2 pos1, pos2;

        static Rect get_rect(Vector2 center, Vector2 size);

        bool contains_point(Vector2 pos);
        bool contains_rect(Rect other);

        Vector2 top_left();
        Vector2 top_center();
        Vector2 top_right();
        Vector2 center();
        Vector2 bottom_left();
        Vector2 bottom_center();
        Vector2 bottom_right();

        Rect translate(Vector2 vector);
};

#endif