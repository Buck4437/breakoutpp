#include "playing_field.h"
#include "rect.h"
#include "rect_wall.h"
#include "shield.h"

#include <vector>

#ifndef WELL_H_
#define WELL_H_

// A structure containing 3 RectWall, forms the outermost border of the game.
class Well {
    public:
        Well(Rect rect);
        std::vector<RectWall> get_walls();
        Rect get_inner_box();
        bool is_below_well(double y);
        void draw(PlayingField pf);
        Shield shield;

    private:
        // A vector that stores all three walls in the well.
        // It is in the order of top, left and right.
        std::vector<RectWall> walls;
        Rect shield_box;
        Rect inner_box;

        void initialize_walls(Rect rect);
};

#endif