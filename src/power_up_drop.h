#include "paddle.h"
#include "playing_field.h"
#include "power_up.h"
#include "well.h"

#ifndef POWER_UP_DROP_H_
#define POWER_UP_DROP_H_

// Represents a power-up collectable that falls from the field.
class PowerUpDrop {
    public:
        PowerUpDrop(Vector2 pos, double move_speed, PowerUp powerup);
        void move();
        bool collide(Paddle paddle);
        bool outside_well(Well well);
        double next_y();
        void draw_pf(PlayingField pf);
        PowerUp powerup;

    private:
        Vector2 pos;
        double move_speed;
};

#endif