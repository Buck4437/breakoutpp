#include <set>
#include <string>

#include "playing_field.h"
#include "rect.h"

#ifndef RECT_WALL_H_
#define RECT_WALL_H_

// Generic indestructable wall objects
class RectWall {
    public:
        RectWall(Rect rect, int clr0 = 0);

        void get_drawing_range(PlayingField pfield, int &r1, int &r2, int &c1, int &c2);
        void draw_to_pf(PlayingField pfield);
        void draw_fill(PlayingField pfield);
        void draw_frame(PlayingField pfield);

        void set_draw_pattern(int pattern);
        void set_filler(char filler);
        Rect get_rect();

    private:
        Rect rect;
        char filler = '#', clr0;
        int pattern = 0;
};

#endif