#include "loot_table.h"
#include "power_up.h"
#include "power_up_list.h"

#include <cmath>
#include <random>
#include <stdexcept>

// Constructs a default loot table, used for randomizing power-ups.
LootTable::LootTable() {
    std::map<PowerUp, int> loot_map;
    loot_map[PowerUpList::PAD_EXPAND] = 5;
    loot_map[PowerUpList::PAD_REDUCE] = 5;
    loot_map[PowerUpList::FAST_BALL] = 5;
    loot_map[PowerUpList::SLOW_BALL] = 5;
    loot_map[PowerUpList::MULTIBALL] = 5;
    loot_map[PowerUpList::MULTIPLIER] = 5;
    loot_map[PowerUpList::SHIELD] = 5;
    loot_map[PowerUpList::MISSILE] = 5;

    init(loot_map);
}

// Constructs a loot table using a map that stores power-up weights.
// loot_table: The power-up weights.
// e.g. If loot_table is {PowerUpList::PAD_EXPAND: 3, PowerUpList::PAD_REDUCE: 1},
// then pad expand and pad reduce will be drawn 75% and 25% of the time respectively.
LootTable::LootTable(std::map<PowerUp, int> loot_table) {
    init(loot_table);
}

// Initializes the loot table by setting RNG and calculating the total weight.
// loot_table: The power-up weights.
void LootTable::init(std::map<PowerUp, int> loot_table) {
    LootTable::loot_table = loot_table;
    engine = std::mt19937(rd());

    if (loot_table.size() == 0) {
        throw std::invalid_argument("Loot Table is empty");
    }

    int sum = 0;
    for (std::map<PowerUp, int>::iterator i = loot_table.begin(); i != loot_table.end(); i++) {
        int weight = (*i).second;
        if (weight <= 0) {
            throw std::invalid_argument("Weight should be positive");
        }
        sum += weight;
    }
    total_weight = sum;
}

// Draws a random power-up from the loot table and returns it.
PowerUp LootTable::draw_power_up() {
    int r = std::abs((int)engine());
    int val = r % total_weight + 1;

    int cum = 0;
    for (std::map<PowerUp, int>::iterator i = loot_table.begin(); i != loot_table.end(); i++) {
        cum += (*i).second;
        if (cum >= val) {
            return (*i).first;
        }
    }
    // Fallback
    throw std::invalid_argument("Something bad happened. " + std::to_string(r) + " " + std::to_string(val));
    return (*loot_table.begin()).first;
}
