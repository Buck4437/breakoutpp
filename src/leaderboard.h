#include "record.h"

#include <string>
#include <vector>

#ifndef LEADERBOARD_H_
#define LEADERBOARD_H_

// leaderboard.txt holds the records of top-10 players
class Leaderboard {

    public:
        Leaderboard();
        int check_if_top_10(int points);
        void sort_leaderboard();
        void add_to_leaderboard(Record new_record);
        void revise_leaderboard();
        void push_leaderboard();
        std::vector<Record> get_records();

    private:
        std::vector<Record> record_list;
        std::vector<Record> pull_leaderboard();

        const std::string path = "data/leaderboard.txt";
};

#endif
