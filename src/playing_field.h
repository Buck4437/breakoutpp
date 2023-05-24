#include "ncu.h"
#include "vector2.h"

#ifndef PLAYING_FIELD_H_
#define PLAYING_FIELD_H_

// Container for the region for physics/game simulation. Includes an ncurses window.
class PlayingField {
    private:
        WINDOW *display_window;
        Vector2 size;

    public:
        int max_x, max_y;

        PlayingField(Vector2 size);
        void set_display_window();
        int col_x(double x);
        int row_y(double y);
        WINDOW *get_display_window();
};

// row,"r" and col,"c" are the coordinates used in drawing to ncurses window
// r=0, c=0, starts at the top-left corner of the window
// the x_image, y_image can stretch the screen in the factors

// x-coordinates and y-coordinates are used in the physics calculation and game logic
// the origin starts from the center of the screen
// row_y(y) and col_x(x) converts x and y coords to row and col for wprintw in ncurses window

#endif