#include "playing_field.h"
#include "rect_block.h"
#include "well.h"

#ifndef MISSILE_H_
#define MISSILE_H_

class Missile {
    public:
        Missile(Vector2 pos, double move_speed);
        void move();
        bool collide(RectBlock *bricks);
        bool hit_well(Well well);
        void draw_pf(PlayingField pf);

    private:
        Vector2 pos;
        double base_y, move_speed;
};

#endif