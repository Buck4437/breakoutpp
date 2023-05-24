#include "well.h"
#include "playing_field.h"
#include "rect.h"
#include "rect_wall.h"
#include "shield.h"

#include <vector>

const double margin = 0.2;

// Constructs a well, which is the sides of the playing field.
// rect: The rectangle to build the well object from.
Well::Well(Rect rect) {
    initialize_walls(rect);
    inner_box = {rect.bottom_left().add_x(margin), rect.top_right().add_x(-margin)};
    shield_box = {rect.bottom_left().add_y(-margin), rect.bottom_right().add_y(margin)};
}

// Initializes the top, left and right walls.
// rect: The rectangle to build the well object from.
void Well::initialize_walls(Rect rect) {
    Rect wall_rects[3] = {
        {rect.top_left().add_y(-margin), rect.top_right().add_y(margin)},      // Top edge
        {rect.bottom_left().add_x(-margin), rect.top_left().add_x(margin)},    // Left edge
        {rect.bottom_right().add_x(-margin), rect.top_right().add_x(margin)}}; // Right edge
    for (Rect wall_rect : wall_rects) {
        RectWall wall = RectWall(wall_rect);
        wall.set_draw_pattern(1);
        wall.set_filler('x');
        walls.push_back(wall);
    }
}

// Returns the inner box, i.e. the box used for calculating collisions.
Rect Well::get_inner_box() {
    return inner_box;
}

// This extra bit of margin is added to prevent false detection of balls,
// when it can be deflected by the shield.
bool Well::is_below_well(double y) {
    return y < get_inner_box().pos1.y - 0.5;
}

// Displays the well on the screen.
// pf: The playing field to draw on.
void Well::draw(PlayingField pf) {
    for (RectWall wall : walls) {
        wall.draw_to_pf(pf);
    }

    RectWall shield_wall(shield_box);
    shield_wall.set_draw_pattern(1);
    shield_wall.set_filler(shield.get_filler());
    shield_wall.draw_to_pf(pf);
    wrefresh(pf.get_display_window());
}

// Returns the walls that form the well (one on the top, one on the left and one of the right).
std::vector<RectWall> Well::get_walls() {
    return walls;
}