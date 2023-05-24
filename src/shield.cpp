#include "shield.h"

// Returns the level of the shield, i.e. how many times it can be hit before it breaks.
int Shield::get_level() {
    return level;
}

// Increases the level of the shield, up to a maximum of 3 levels.
// Returns true if the upgrade can be applied, false otherwise.
bool Shield::upgrade() {
    if (level < 3) {
        ++level;
        return true;
    }
    return false;
}

// Decreases the level of the shield.
void Shield::destroy() {
    if (level > 0) {
        --level;
    }
}

// Returns if the shield is active at the moment.
bool Shield::is_intact() {
    return level > 0;
}

// Returns the character used to draw the shield.
// (The higher its level, the more lines it has and the stronger it looks.)
char Shield::get_filler() {
    switch (level) {
    case 0:
        return ' ';
    case 1:
        return '-';
    case 2:
        return '=';
    case 3:
        return '#';
    default:
        return '?';
    }
}
