#include "rect_wall.h"
#include "ncu.h"
#include "playing_field.h"
#include "rect.h"

#include <iostream>
#include <string>

// Constructs a wall. The wall can be a brick, or can be an actual, indestructable wall.
// rect: The rectangle that the wall occupies.
// clr0: The color code of this wall.
RectWall::RectWall(Rect rect, int clr0) {
    RectWall::rect = rect;
    RectWall::clr0 = clr0;
}

// Sets the pattern that will be used when drawing the RectWall on the screen.
// pattern: The pattern to use. 0 = frame, 1 = fill.
void RectWall::set_draw_pattern(int pattern){
    RectWall::pattern = pattern;
}

// Sets the filler character that will be used when drawing the RectWall with the fill pattern.
// filler: The character used to fill.
void RectWall::set_filler(char filler){
    RectWall::filler = filler;
}

// Returns the range of cells on the screen to draw on (starting and ending rows and columns).
// pfield: The playing field to draw on.
// &r1: The starting row.
// &r2: The ending row.
// &c1: The starting column.
// &c2: The ending column.
// All 4 pass-by-reference variables are outputs of the function.
void RectWall::get_drawing_range(PlayingField pfield, int &r1, int &r2, int &c1, int &c2){
    r1 = pfield.row_y(rect.pos2.y);
    r2 = pfield.row_y(rect.pos1.y);
    c1 = pfield.col_x(rect.pos1.x);
    c2 = pfield.col_x(rect.pos2.x);
}

// Draws on the screen with the fill pattern.
// This fills a rectangle on the screen with the filler character.
// pfield: The playing field to draw on.
void RectWall::draw_fill(PlayingField pfield){
    int r1, r2, c1, c2;
    get_drawing_range(pfield, r1, r2, c1, c2);

    for (int r = r1; r <= r2; r++) {
        for (int c = c1; c <= c2; c++) {
            wattrset(pfield.get_display_window(), COLOR_PAIR(clr0));
            wmove(pfield.get_display_window(), r, c);
            wprintw(pfield.get_display_window(), "%c", filler);
        }
    }
}

// Draws on the screen with the frame pattern.
// This draws only the corners (with '+') and the edges (with '-' and '|').
// pfield: The playing field to draw on.
void RectWall::draw_frame(PlayingField pfield){
    int r1, r2, c1, c2;
    get_drawing_range(pfield, r1, r2, c1, c2);

    wattrset(pfield.get_display_window(), COLOR_PAIR(clr0));
    wmove(pfield.get_display_window(), r1, c1 + 1);
    whline(pfield.get_display_window(), '-', c2 - c1);
    wmove(pfield.get_display_window(), r2, c1 + 1);
    whline(pfield.get_display_window(), '-', c2 - c1);
    wmove(pfield.get_display_window(), r1 + 1, c1);
    wvline(pfield.get_display_window(), '|', r2 - r1);
    wmove(pfield.get_display_window(), r1 + 1, c2);
    wvline(pfield.get_display_window(), '|', r2 - r1);

    mvwaddstr(pfield.get_display_window(), r1, c1, "+");
    mvwaddstr(pfield.get_display_window(), r1, c2, "+");
    mvwaddstr(pfield.get_display_window(), r2, c1, "+");
    mvwaddstr(pfield.get_display_window(), r2, c2, "+");
}

// Draws this RectWall on the screen.
// pfield: The playing field to draw on.
void RectWall::draw_to_pf(PlayingField pfield) {
    wattron(pfield.get_display_window(), 0);
    if (pattern == 0) draw_frame(pfield);
    if (pattern == 1) draw_fill(pfield);
    wattrset(pfield.get_display_window(), 0);
    //wattroff(pfield.get_display_window(), 0);
}

// Returns the rect that this wall occupies.
Rect RectWall::get_rect() { return rect; }