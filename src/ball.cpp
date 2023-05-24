#include "ball.h"
#include "ncu.h"
#include "paddle.h"
#include "playing_field.h"
#include "rect_block.h"
#include "rect_wall.h"
#include "well.h"

#include <cmath>
#include <iostream>

// Creates a Ball with specified position and velocity.
// pos: The initial ball position.
// base_vel: The initial ball velocity.
Ball::Ball(Vector2 pos, Vector2 base_vel) {
    Ball::pos = pos;
    Ball::base_vel = base_vel;
    new_vel = base_vel;
}

// Sets the ball's speed multiplier, making it move faster or slower.
// val: The new speed multiplier.
void Ball::set_speed_multi(double val) {
    speed_multi = val;
}

// Returns the velocity of the ball, affected by its speed multiplier.
Vector2 Ball::vel() {
    return base_vel.scale(speed_multi);
}

// Draw the ball on a PlayingField.
// pfield: The PlayingField that the ball is drawn on.
void Ball::draw_pf(PlayingField pfield) {
    int r = pfield.row_y(pos.y);
    int c = pfield.col_x(pos.x);
    wmove(pfield.get_display_window(), r, c);
    wprintw(pfield.get_display_window(), "o");
    return;
}

// Returns the magnitude of the ball's velocity, i.e. speed.
double Ball::speed() {
    return vel().magnitude();
}

// Applies the new ball's velocity (due to wall collisions)
// to its actual velocity used in calculations.
void Ball::update_velocity() {
    base_vel = new_vel;
}

// Returns the position of the ball on the next frame.
Vector2 Ball::get_next_pos(double factor) {
    return pos.add(vel().scale(factor));
}

// Simulates the ball's movement in a frame.
// factor: Scaling factor for the change in position.
void Ball::move_by_velocity(double factor) {
    update_velocity();
    pos = get_next_pos(factor);
}

// Checks if the ball hits the well, and make it bounce if it does.
// If the shield is on, also checks if the ball collides with the shield.
// If it does, bounce the ball up and consume the shield.
// well: The well to check collision against.
// factor: Scaling factor for the change in position.
void Ball::if_collide_rebound(Well &well, double factor) {
    Rect inner_box = well.get_inner_box();
    Vector2 new_pos = get_next_pos(factor);

    if (new_pos.x < inner_box.pos1.x) {
        new_vel.x = std::abs(base_vel.x);
    } else if (new_pos.x > inner_box.pos2.x) {
        new_vel.x = -std::abs(base_vel.x);
    }

    if (new_pos.y > inner_box.pos2.y) {
        new_vel.y = -std::abs(base_vel.y);
    }

    // When the ball tries to escape the wall via the bottom
    if (new_pos.y < inner_box.pos1.y && base_vel.y < 0) {
        if (well.shield.is_intact()) {
            well.shield.destroy();
            new_vel.y = std::abs(base_vel.y);
        }
    }
}

// Checks if the ball hits the paddle. If it does, reflect the ball based on where it is hit.
// paddle: The paddle to check collision against.
// factor: Scaling factor for the change in position.
void Ball::if_collide_rebound(Paddle paddle, double factor) {
    if (vel().y > 0) {
        return;
    }
    if (paddle.can_hit_ball(get_next_pos(factor))) {
        double deflection_angle = paddle.get_deflection_angle(pos.x);

        double mag_vel = base_vel.magnitude();
        new_vel.x = std::sin(deflection_angle) * mag_vel;
        new_vel.y = std::cos(deflection_angle) * mag_vel;
    }
}

// Checks if the ball hits the brick, and make it bounce if it does.
// If the ball (somehow) hits the corner of the brick, it will be reflected diagonally.
// brick: The brick to check collision against.
// factor: Scaling factor for the change in position.
void Ball::if_collide_rebound(RectBlock *brick, double factor) {
    Vector2 next_pos = get_next_pos(factor);
    if (brick->get_rect().contains_point({next_pos.x, pos.y})) {
        new_vel = base_vel.horizontal_flip();
        brick->break_brick();
    } else if (brick->get_rect().contains_point({pos.x, next_pos.y})) {
        new_vel = base_vel.vertical_flip();
        brick->break_brick();
    } else if (brick->get_rect().contains_point(next_pos)) {
        new_vel = base_vel.flip();
        brick->break_brick();
    }
}

// Returns if the ball is below the screen (defined as the bottom of the well).
// well: The well to use for checking if the ball is below the well.
bool Ball::outside_well(Well well) {
    return well.is_below_well(pos.y);
}

// Moves the ball to the top-centre of the paddle.
// paddle: The paddle used to calculate the ball's new position.
// offset: Where the ball should be placed. -1 is the left and 1 is the right.
void Ball::move_to_paddle(Paddle paddle, double offset) {
    double len = paddle.length();
    pos = paddle.get_pos().add({len * offset * 0.5, 1});
}
