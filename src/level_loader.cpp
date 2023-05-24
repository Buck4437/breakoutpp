#include "level.h"
#include "loot_table.h"
#include "power_up_list.h"
#include "rect.h"
#include "rect_block.h"

#include <fstream>
#include <map>
#include <stdexcept>
#include <string>

// Returns the power-up object corresponding to a 3-letter abbreviation.
// Refer to power_up_list.cpp for the abbreviations.
// abbr: The 3-letter abbreviation.
PowerUp abbr_to_power_up(std::string abbr) {
    if (abbr == PowerUpList::PAD_EXPAND.abbr) {
        return PowerUpList::PAD_EXPAND;
    }
    if (abbr == PowerUpList::PAD_REDUCE.abbr) {
        return PowerUpList::PAD_REDUCE;
    }
    if (abbr == PowerUpList::FAST_BALL.abbr) {
        return PowerUpList::FAST_BALL;
    }
    if (abbr == PowerUpList::SLOW_BALL.abbr) {
        return PowerUpList::SLOW_BALL;
    }
    if (abbr == PowerUpList::MULTIBALL.abbr) {
        return PowerUpList::MULTIBALL;
    }
    if (abbr == PowerUpList::MULTIPLIER.abbr) {
        return PowerUpList::MULTIPLIER;
    }
    if (abbr == PowerUpList::SHIELD.abbr) {
        return PowerUpList::SHIELD;
    }
    if (abbr == PowerUpList::MISSILE.abbr) {
        return PowerUpList::MISSILE;
    }

    throw std::runtime_error("Unrecognized power up id: " + abbr);
}

// Reads the next word in the ifstream and performs actions if it is one of the following keywords:
// "cst": The function checks for the next argument and decides what to construct in the level.
// "loots": The level loader starts modifying the power-up drops from bricks.
// "pity": The level loader starts modifying the power-up drops from the pity system.
void Level::read_command() {
    std::string cmd;
    fin >> cmd;
    if (cmd == "cst") {
        construct_object();
    } else if (cmd == "loots") {
        construct_loot_table_from_file();
    } else if (cmd == "pity") {
        construct_pity_table_from_file();
    }
}

// Reads the next word in the ifstream and performs actions if it is one of the following keywords:
// "fin": Finish modifying the power-up drops from bricks.
// "freq": Adjust the frequency of power-up drops.
// "offset": Adjust the offset of power-up drops. See set_drop_offset() for details.
// a power-up abbreviation: Adjust the weight of the power-up in the loot table.
//
// loot_map: A map that records the weights of power-ups from bricks. Modified by this function.
bool Level::read_loot_table_command(std::map<PowerUp, int> &loot_map) {
    std::string cmd;
    fin >> cmd;
    if (cmd == "fin") {
        return false;
    } else if (cmd == "freq") {
        set_drop_frequency();
    } else if (cmd == "offset") {
        set_drop_offset();
    } else {
        set_power_up_weight(loot_map, abbr_to_power_up(cmd));
    }
    return true;
}

// Reads the next word in the ifstream and performs actions if it is one of the following keywords:
// "fin": Finish modifying the power-up drops from the pity system.
// "threshold": Change the threshold of bricks to clear before free power-ups appear.
// a power-up abbreviation: Adjust the weight of the power-up in the loot table.
//
// pity_map: A map that records the weights of power-ups from the pity system. Modified by this function.
bool Level::read_pity_table_command(std::map<PowerUp, int> &pity_map) {
    std::string cmd;
    fin >> cmd;
    if (cmd == "fin") {
        return false;
    } else if (cmd == "threshold") {
        set_pity_threshold();
    } else {
        set_power_up_weight(pity_map, abbr_to_power_up(cmd));
    }
    return true;
}

// Constructs an object in the level. The ifstream reads the next word and decides what to construct:
// "rbk": Constructs a brick. This requires 5 arguments.
// "wal": Constructs a wall. This requires 5 arguments.
// "lvl": Constructs a grid of bricks. This requires 9 arguments.
void Level::construct_object() {
    std::string cmd;
    fin >> cmd;
    if (cmd == "rbk") {
        construct_brick(false);
    } else if (cmd == "wal") {
        construct_brick(true);
    } else if (cmd == "lvl") {
        construct_grid_of_bricks();
    }
}

// Constructs a brick. The ifstream reads the next 5 arguments, which are:
// x-coordinate of the brick's left side, y-coordinate of the brick's bottom,
// x-coordinate of the brick's right side, y-coordinate of the brick's top, the brick's color's id.
// unbreakable: If true, constructs a wall instead, which is unbreakble.
void Level::construct_brick(bool unbreakable) {
    double arg_x1, arg_y1, arg_x2, arg_y2;
    int arg_clr0;
    fin >> arg_x1 >> arg_y1 >> arg_x2 >> arg_y2 >> arg_clr0;
    RectBlock *newBrick = new RectBlock({arg_x1, arg_y1, arg_x2, arg_y2}, arg_clr0);
    if (unbreakable) {
        newBrick->set_draw_pattern(1);
        newBrick->unbreakable = true;
    }
    add_brick(newBrick);
}

// Constructs a grid of bricks. The ifstream reads the next 9 arguments.
//
// The first 4 arguments define the top-left brick in the grid:
// x-coordinate of the brick's left side, y-coordinate of the brick's bottom,
// x-coordinate of the brick's right side, y-coordinate of the brick's top.
//
// The next 5 arguments apply to all bricks in the grid:
// horizontal distance between centres of bricks, vertical distance between centres of bricks,
// number of rows in the grid, number of columns in the grid, the bricks' color's id.
void Level::construct_grid_of_bricks() {
    double arg_x1, arg_y1, arg_x2, arg_y2;
    int arg_clr0;
    double arg_x_sep, arg_y_sep;
    int arg_x_repeat, arg_y_repeat;

    fin >> arg_x1 >> arg_y1 >> arg_x2 >> arg_y2 >> arg_x_sep >> arg_y_sep >> arg_x_repeat >> arg_y_repeat >> arg_clr0;

    RectBlock::create_rectblocks(bricks, {arg_x1, arg_y1, arg_x2, arg_y2}, arg_x_sep, arg_y_sep, arg_x_repeat, arg_y_repeat, arg_clr0);
}

// Reads the next number in the ifstream and sets the power-up drop frequency to it.
// The default is 5, which means every 5 bricks drop a power-up.
// The frequency must be positive. The function throws a runtime error if this is not the case.
void Level::set_drop_frequency() {
    fin >> drop_freq;
    if (drop_freq <= 0) {
        throw std::runtime_error("Power-up drop frequency must be positive");
    }
}

// Reads the next number in the ifstream and sets the power-up drop offset to it.
// The default is 2, which means the first power-up is from the 3rd block.
// Setting the offset to n makes the first power-up appear from the (n+1)th block.
void Level::set_drop_offset() {
    fin >> offset;
}

// Reads the next number in the ifstream and sets the pity threshold to it.
// The default is 80, which means after 80% of the bricks are broken, free power-ups start dropping.
void Level::set_pity_threshold() {
    fin >> pity_threshold;
}

// Reads the next number in the ifstream and sets the weight of a power-up to it.
// The probability of a drop being picked is its weight divided by the total weight in the table.
// weights: A map that records the weights of power-ups. Modified by this function.
// power_up: The power-up to change the weight for.
void Level::set_power_up_weight(std::map<PowerUp, int> &weights, PowerUp power_up) {
    fin >> weights[power_up];
}

// Sets the brick power-up drop loot table to a custom one defined in the .bl file.
// The ifstream starts interpreting commands for the table until it reads "fin".
void Level::construct_loot_table_from_file() {
    std::map<PowerUp, int> loot_map;
    while (read_loot_table_command(loot_map)) {
        continue;
    }
    if (loot_map.size() >= 1) {
        if (loot_table != NULL) {
            delete loot_table;
        }
        loot_table = new LootTable(loot_map);
    }
}

// Sets the free power-up drop loot table to a custom one defined in the .bl file.
// The ifstream starts interpreting commands for the table until it reads "fin".
void Level::construct_pity_table_from_file() {
    std::map<PowerUp, int> pity_map;
    while (read_pity_table_command(pity_map)) {
        continue;
    }
    if (pity_map.size() >= 1) {
        if (pity_table != NULL) {
            delete pity_table;
        }
        pity_table = new LootTable(pity_map);
    }
}

// Loads a level from a .bl file. Returns 0 if the level is successfully loaded, 1 otherwise.
// The .bl file is read using an ifstream, and works similarly to shell scripts.
// The ifstream looks for command names and their arguments, until EOF is reached.
// filename: The address of the file to be opened.
int Level::load_level_by_file(std::string filename) {

    if (game_stat_ptr == NULL || pf_ptr == NULL) {
        return 1;
    }

    fin.open(filename);

    while (!fin.eof()) {
        read_command();
    }

    fin.close();
    return 0;
}