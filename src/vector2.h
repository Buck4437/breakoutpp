#ifndef VECTOR2_H_
#define VECTOR2_H_

struct Vector2 {
        double x, y;

        double magnitude();
        bool on_top_right_of(Vector2 other);

        Vector2 add(Vector2 other);
        Vector2 add_x(double x);
        Vector2 add_y(double y);
        Vector2 scale(double scalar);

        Vector2 horizontal_flip();
        Vector2 vertical_flip();
        Vector2 flip();

        static Vector2 midpoint(Vector2 pos1, Vector2 pos2);
};

#endif