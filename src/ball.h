#include "paddle.h"
#include "playing_field.h"
#include "rect_block.h"
#include "rect_wall.h"
#include "well.h"

#ifndef BALL_H_
#define BALL_H_

// A round spherical object that is responsible for player's control.
// It moves in a 2D plane which allows breaking blocks to achieve the main objective of the game.
class Ball {
    public:
        // The maximum distance a ball can move before checking for collision.
        // Beyond this value, the ball may clip through walls.

        const double max_stepping = 0.1;

        Ball(Vector2 pos, Vector2 base_vel);
        void draw_pf(PlayingField pfield);
        double speed();

        Vector2 get_next_pos(double factor);
        void move_by_velocity(double factor);

        void if_collide_rebound(Well &well, double factor);
        void if_collide_rebound(Paddle paddle, double factor);
        void if_collide_rebound(RectBlock *rectwall, double factor);

        Vector2 vel();

        void set_speed_multi(double val);

        void move_to_paddle(Paddle paddle, double offset = 0);

        bool outside_well(Well well);

        // Positive = Faster, Negative = Slower
        double frame = 0;

    private:
        Vector2 pos, base_vel;
        // The purpose of new_vel_x and new_vel_y is to prevent rebouncing twice upon detection
        Vector2 new_vel = base_vel;

        double speed_multi = 1.0;

        void update_velocity();
};

#endif