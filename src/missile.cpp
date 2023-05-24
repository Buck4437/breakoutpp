#include "missile.h"
#include "playing_field.h"
#include "rect.h"
#include "rect_block.h"

#include <string>

// Constructs a missile object.
// x: The x-coordinate of its position.
// y: The y-coordinate of its position.
// move_speed: The amount of distance that the missile moves upwards per frame.
Missile::Missile(Vector2 pos, double move_speed) {
    Missile::pos = pos;
    Missile::base_y = pos.y;
    Missile::move_speed = move_speed;
}

// Simulates the missile and moves it upwards.
void Missile::move() {
    pos.y += move_speed;
}

// Returns if the missile has collided with a brick.
// brick: The brick to check collision against.
bool Missile::collide(RectBlock *brick) {
    return brick->get_rect().contains_point(pos);
}

// Returns if the missile has hit the top of the well (playing field).
// well: The well to check collision against.
bool Missile::hit_well(Well well) {
    return !well.get_inner_box().contains_point(pos);
}

// Displays the missile.
// pf: The playing field to draw the missile on.
void Missile::draw_pf(PlayingField pf) {
    std::string art[3] = {"^", "|", "^"};
    int base_r = pf.row_y(base_y);
    int r = pf.row_y(pos.y);
    int c_start = pf.col_x(pos.x);
    for (int i = 0; i < 3; i++) {
        if (r + i >= base_r) {
            break;
        }
        wmove(pf.get_display_window(), r + i, c_start);
        wprintw(pf.get_display_window(), art[i].c_str());
    }
}
