#include "rect_wall.h"

#ifndef RECT_BLOCK_H_
#include <set>
#define RECT_BLOCK_H_

class RectBlock : public RectWall {
    public:
        using RectWall::RectWall;

        static void create_rectblocks(std::set<RectBlock *> &bricks, Rect subject_rect, double x_separation, double y_separation, int x_repeat, int y_repeat, int clr0 = 32);

        void break_brick();
        bool broken = false;
        bool unbreakable = false;

        // Points given when broken such block
        double points;
        std::set<RectBlock *> *parent_set;

        // To be implemented
        int appearance_id;
};

#endif
