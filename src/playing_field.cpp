#include "playing_field.h"
#include "ncu.h"

#include <cmath>

const double kImageX = 2.0;
const double kImageY = 1.0;

// Constructs a playing field with the given dimensions.
// width: The width of the playing field.
// height: The height of the playing field.
PlayingField::PlayingField(Vector2 size) {
    PlayingField::size = size;
}

// Sets up a window in ncurses. This needs to be done after ncurses has been initialized.
void PlayingField::set_display_window() {
    max_x = (int)ceil(size.x * kImageX);
    max_y = (int)ceil(size.y * kImageY);
    display_window = newwin(max_y, max_x, 6, 0);
}

// When the game is being simulated, the positions, velocities, etc. are being stored as doubles.
// This means that our simulation is more precise than simulating everything on a grid.
// However, we are still confined to a terminal with limited resolution, with a small width and height.

// The following 2 functions map the positions in the simulation to the positions of characters
// that you actually see on the screen. The center of the screen is (0, 0) in the simulation.

// Converts an x-coordinate in the simulation to a column in the terminal.
// x: The x-coordinate.
int PlayingField::col_x(double x) {
    return (int)(round((size.x / 2 + x) * kImageX));
}

// Converts a y-coordinate in the simulation to a row in the terminal.
// y: The y-coordinate.
int PlayingField::row_y(double y) {
    return (int)(round((size.y / 2 - y) * kImageY));
}

// Returns the display window as a pointer.
WINDOW *PlayingField::get_display_window() {
    return display_window;
}