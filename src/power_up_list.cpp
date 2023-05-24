#include "power_up_list.h"

// Here is a list of power-ups that the game has.
// The format is as follows: {id, in-game icon, short form used in level files}.
const PowerUp PowerUpList::PAD_EXPAND = {1, "<->", "xpd"};
const PowerUp PowerUpList::PAD_REDUCE = {2, ">-<", "rdc"};
const PowerUp PowerUpList::FAST_BALL = {3, "o>>", "fst"};
const PowerUp PowerUpList::SLOW_BALL = {4, "o<<", "slw"};
const PowerUp PowerUpList::MULTIBALL = {5, "ooo", "mlb"};
const PowerUp PowerUpList::MULTIPLIER = {6, "x 2", "mlt"};
const PowerUp PowerUpList::SHIELD = {7, "###", "shd"};
const PowerUp PowerUpList::MISSILE = {8, ">=>", "msl"};