#include "playing_field.h"
#include "rect.h"
#include "well.h"

#ifndef PADDLE_H_
#define PADDLE_H_

class Paddle {
    public:
        Paddle(Vector2 pos, double base_length, double thickness);
        double length();

        void draw_pf(PlayingField pfield);
        Rect get_paddle_hitbox();
        Rect get_ball_hitbox();
        bool can_hit_ball(Vector2 ball_pos);
        void move_by_input(int ch, Well well);
        double get_deflection_angle(double arg_x);
        Vector2 get_pos();

        bool buff(Well well);
        bool nerf();
        void reset_width(Well well);

    private:
        Vector2 pos;
        double base_length, thickness;
        double leftmost_deflect_angle;
        double rightmost_deflect_angle;

        // Positive => Bigger, Negative => Smaller
        double buffs = 0;
};

#endif