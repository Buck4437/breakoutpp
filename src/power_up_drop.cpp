#include "power_up_drop.h"
#include "playing_field.h"
#include "rect.h"

// Constructs (spawns) a dropping power-up on the playing field.
// pos: The spawning position.
// move_speed: The power-up's dropping speed.
// powerup: The type of power-up that this dropping power-up represents.
PowerUpDrop::PowerUpDrop(Vector2 pos, double move_speed, PowerUp powerup) {
    PowerUpDrop::pos = pos;
    PowerUpDrop::move_speed = move_speed;
    PowerUpDrop::powerup = powerup;
}

// Simulates the dropping power-up for 1 frame, causing it to move downwards.
// (This is getting redundant at this point, but C++ inheritance is a mess, so...)
void PowerUpDrop::move() {
    pos.y = next_y();
}

// Returns if the power-up has collided with the paddle.
// paddle: The paddle to check collision against.
bool PowerUpDrop::collide(Paddle paddle) {
    Rect hitbox = paddle.get_ball_hitbox();
    return hitbox.contains_point(pos);
}

// Returns if the power-up has fallen outside of the playing field.
// Used for detecting if it should be destroyed.
// well: The well of the playing field.
bool PowerUpDrop::outside_well(Well well) {
    return well.is_below_well(pos.y);
}

// Returns the position of the power-up on the next frame.
double PowerUpDrop::next_y() {
    return pos.y - move_speed;
}

// Displays the power-up.
// pf: The playing field to draw the power-up on.
void PowerUpDrop::draw_pf(PlayingField pf) {
    int r = pf.row_y(pos.y);
    int c_start = pf.col_x(pos.x - 1);
    wmove(pf.get_display_window(), r, c_start);
    if (powerup.id == 2 || powerup.id == 3) {
        wattron(pf.get_display_window(), COLOR_PAIR(8));
    } else {
        wattron(pf.get_display_window(), COLOR_PAIR(16));
    }
    wprintw(pf.get_display_window(), powerup.symbol.c_str());
    if (powerup.id == 2 || powerup.id == 3) {
        wattroff(pf.get_display_window(), COLOR_PAIR(8));
    } else {
        wattron(pf.get_display_window(), COLOR_PAIR(16));
    }
    wattroff(pf.get_display_window(), COLOR_PAIR(8));
    wattroff(pf.get_display_window(), COLOR_PAIR(16));
}
