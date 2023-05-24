#include "power_up.h"
#include <map>
#include <random>

#ifndef LOOT_TABLE_H_
#define LOOT_TABLE_H_

class LootTable {

    public:
        LootTable();
        LootTable(std::map<PowerUp, int> loot_table);

        PowerUp draw_power_up();

    private:
        void init(std::map<PowerUp, int> loot_table);
        std::map<PowerUp, int> loot_table;
        std::random_device rd;
        std::mt19937 engine;

        int draw_num();
        int total_weight;
};

#endif