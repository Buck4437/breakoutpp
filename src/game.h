#include "ball.h"
#include "game_stat.h"
#include "leaderboard.h"
#include "level.h"
#include "notification_bar.h"
#include "paddle.h"
#include "playing_field.h"
#include "power_up.h"
#include "power_up_drop.h"
#include "rect_wall.h"
#include "well.h"

#ifndef GAME_H_
#define GAME_H_

// Part of the game
class Game {
    private:
        PlayingField pf = PlayingField({32.0, 32.0});
        WINDOW *info_screen;
        Level *cur_lv;
        NotificationBar bar = NotificationBar(64, 1);
        GameStat game_stat;
        Leaderboard lb;
        Record rc;

        void initialize_screen();

        void print_stats(bool all_completed);
        void add_record_to_leaderboard();
        std::string get_player_name(int rank);

    public:
        void run_game(std::vector<std::string> filenames);
        void initialize_level(std::string level_file);
        void run_level(std::string level_file);
        void run_round();
        bool round_ended();
        bool level_ended();
        bool game_ended();
};

#endif