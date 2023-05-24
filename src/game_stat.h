#ifndef GAME_STAT_H_
#define GAME_STAT_H_

#include "game_stat_timer.h"
#include "ncu.h"

#include <string>

class GameStat {
    public:
        GameStat();

        // Increase the score based on the base score of a brick, multiplier is automatically applied.
        void add_base_score(int base_score);
        void add_score_multi();
        void add_speed();
        void sub_speed();
        void add_missile();
        void sub_missile();
        void sub_lives();
        void add_level();

        int get_score();
        int get_score_multi();
        double get_speed_multi();
        int get_missile();
        bool can_fire_missile();
        int get_lives();
        int get_extra_lives();
        bool has_lives();
        int get_level();

        GameStatTimer get_timer();
        void tick_timer();
        void check_depleted_power_ups();
        void reset_timer();

        void set_pity_timer();

        // Reset stats that are level dependent.
        void reset_lv_stats();
        // Reset every stats
        void reset_all_stats();

        void clear_window();

        void set_display_window(double max_x, double max_y);
        WINDOW *get_display_window();
        void draw_display_window();

    private:
        GameStatTimer timer;
        int total_score, score_multi, speed_multi, missile, lives_lost, level;
        double time_left;
        void apply_ball_speed_timer();

        std::string live_display();

        WINDOW *display_window;
};

#endif