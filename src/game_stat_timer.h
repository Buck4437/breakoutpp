#ifndef GAME_STAT_TIMER_H_
#define GAME_STAT_TIMER_H_
#include "ncu.h"

struct GameStatTimer {
    public:
        int score = 0;
        int speed = 0;
        int missile_cooldown = 0;
        // -1: The timer is disabled.
        // 0+: The timer has been activated.
        int pity = -1;
};

#endif