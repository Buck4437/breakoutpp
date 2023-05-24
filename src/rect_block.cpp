#include "rect_block.h"
#include "rect_wall.h"

#include <iostream>

// Breaks the brick. What actually happens is that the "broken" flag is set to true.
// The game will then later go through all brick objects and delete those marked as "broken".
void RectBlock::break_brick() {
    if (!unbreakable) {
        broken = true;
    }
}

// Creates a grid of bricks.
// bricks: A container to store the bricks that will be created by this function.
// subject_rect: The size of each brick.
// x_separation: The horizontal separation of each bricks' center.
// y_separation: The vertical separation of each bricks' center.
// col_count: The number of columns.
// row_count: The number of rows.
// clr0: The id of the color of the brick.
void RectBlock::create_rectblocks(std::set<RectBlock *> &bricks, Rect subject_rect, double x_separation, double y_separation, int col_count, int row_count, int clr0) {

    for (int i = 0; i < col_count; i++) {
        for (int j = 0; j < row_count; j++) {
            Rect new_rect = subject_rect.translate({x_separation * i, y_separation * j});
            RectBlock *brick = new RectBlock(new_rect, clr0);
            brick->points = 100;
            brick->unbreakable = false;
            brick->parent_set = &bricks;
            bricks.insert(brick);
        }
    }
}