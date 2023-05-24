#include "paddle.h"
#include "math_utils.h"
#include "playing_field.h"
#include "rect.h"
#include "rect_wall.h"
#include "well.h"

#include <cmath>
#include <iostream>

// Constructs a new paddle at a given location with a given size.
// pos: The paddle's position.
// base_length: The starting length of the paddle.
// thickness: The thickness of the paddle.
Paddle::Paddle(Vector2 pos, double base_length, double thickness) {
    Paddle::pos = pos;
    Paddle::base_length = base_length;
    Paddle::thickness = thickness;
    Paddle::leftmost_deflect_angle = from_deg(-80.0);
    Paddle::rightmost_deflect_angle = from_deg(80.0);
}

// Returns the current length of the paddle, including the effect from paddle power-ups.
double Paddle::length() {
    return base_length + 2 * buffs;
}

// Displays the paddle.
// pfield: The playing field to draw the paddle on.
void Paddle::draw_pf(PlayingField pfield) {
    int c1 = pfield.col_x(pos.x - length() / 2);
    int c2 = pfield.col_x(pos.x + length() / 2);
    int r1 = pfield.row_y(pos.y);

    for (int c = c1; c <= c2; c++) {
        wmove(pfield.get_display_window(), r1, c);
        if (c == c1) {
            wprintw(pfield.get_display_window(), "[");
        }
        if (c == c2) {
            wprintw(pfield.get_display_window(), "]");
        }
        if (c != c1 && c != c2) {
            wprintw(pfield.get_display_window(), "-");
        }
    }
}

// Returns the hitbox of the paddle as a Rect.
Rect Paddle::get_paddle_hitbox() {
    return Rect::get_rect(pos, {length(), thickness});
}

// Returns the paddle's hitbox for the ball, which is slightly larger than its true hitbox.
// This is to implement a grace mechanism to make the game easier and feel better.
Rect Paddle::get_ball_hitbox() {
    Rect hitbox = get_paddle_hitbox();
    return {hitbox.pos1.add({-1, 0}), hitbox.pos2.add({1, 0.5})};
}

// Returns whether the given position is within the paddle's "ball hitbox".
// arg_x: The x-coordinate of the position.
// arg_y: The y-coordinate of the position.
bool Paddle::can_hit_ball(Vector2 ball_pos) {
    return get_ball_hitbox().contains_point(ball_pos);
}

// Receives input from the keyboard to move the paddle.
// ch: The key pressed, translated to an int as determined by ncurses.
// well: The well in the playing field. Used to prevent the paddle from clipping into the wall.
void Paddle::move_by_input(int ch, Well well) {
    double offset = 0;
    if (ch == KEY_LEFT || ch == KEY_UP) {
        offset = -1.0;
    }
    if (ch == KEY_RIGHT || ch == KEY_DOWN) {
        offset = 1.0;
    }

    Rect new_hitbox = get_paddle_hitbox().translate({offset, 0});
    if (well.get_inner_box().contains_rect(new_hitbox)) {
        pos.x += offset;
    }
}

// Get the ball's deflection angle depending on where the ball hit the paddle.
// Balls that hit the left of the paddle bounce to the left and vice versa.
// The closer to the edge that the ball hits, the further the ball will be deflected to the side.
double Paddle::get_deflection_angle(double arg_x) {
    Rect ball_hitbox = get_ball_hitbox();
    double ratio = (arg_x - ball_hitbox.pos1.x) / (ball_hitbox.pos2.x - ball_hitbox.pos1.x);
    return lerp(leftmost_deflect_angle, rightmost_deflect_angle, ratio);
}

// Increases the width of the paddle. The buff can be applied at most twice.
// Returns true if the buff can be applied, false if it fails to be applied.
// well: The well in the playing field.
// Used to make sure the size of the paddle won't exceed the size of the well.
bool Paddle::buff(Well well) {
    if (buffs < 2) {
        ++buffs;
        Rect hitbox = get_paddle_hitbox();
        Rect well_hitbox = well.get_inner_box();

        if (hitbox.pos1.x < well_hitbox.pos1.x) {
            pos.x += 1;
        }
        if (hitbox.pos2.x > well_hitbox.pos2.x) {
            pos.x -= 1;
        }

        // If it still hits the other side then it's probably a bad idea to buff it, so we need to undo the buff
        if (hitbox.pos1.x < well_hitbox.pos1.x) {
            --buffs;
        } else {
            return true;
        }
    }
    return false;
}

// Decreases the width of the paddle. The nerf can be applied at most once. 
// If the paddle is too short, the nerf will do nothing.
// Returns true if the buff can be applied, false if it fails to be applied.
bool Paddle::nerf() {
    if (length() >= 7 && buffs > -1) {
        --buffs;
        return true;
    }
    return false;
}

// Resets the width of the paddle.
// well: The well in playing field.
// Used to make sure the size of the paddle won't exceed the size of the well.
void Paddle::reset_width(Well well) {
    if (buffs < 0) {
        while (buffs != 0) {
            buff(well);
        }
    } else if (buffs > 0) {
        buffs = 0;
    }
}

// Returns the paddle's position.
Vector2 Paddle::get_pos() { return pos; }
