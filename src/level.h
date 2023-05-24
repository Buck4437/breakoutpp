#include "ball.h"
#include "game_stat.h"
#include "loot_table.h"
#include "missile.h"
#include "notification_bar.h"
#include "playing_field.h"
#include "power_up_drop.h"
#include "rect.h"
#include "rect_block.h"
#include "rect_wall.h"

#include <fstream>
#include <set>
#include <string>

#ifndef LEVEL_H_
#define LEVEL_H_

class Level {
    private:
        GameStat *game_stat_ptr = NULL;
        PlayingField *pf_ptr = NULL;
        NotificationBar *bar_ptr = NULL;

        bool is_quitted = false;

        // Path for the next level
        std::string next_level = "/";

        int broke_count = 0, drop_freq = 5, offset = 2, pity_threshold = 80;

        Well well = Well({-15, -15, 15, 15});
        Paddle paddle = Paddle({0.0, -10.0}, 7.0, 1.0);

        std::set<RectBlock *> bricks;
        std::set<Ball *> balls;
        std::set<PowerUpDrop *> power_up_drops;
        std::set<Missile *> missiles;

        std::vector<PowerUp> power_up_list;
        LootTable *loot_table = NULL;
        LootTable *pity_table = NULL;

        std::ifstream fin;

        void construct_loot_table();
        void destruct_loot_table();
        void move_ball(Ball &ball);
        void remove_broken_block();

        void handle_power_ups();

        void tick_power_up_timers();
        bool is_pity_activated();
        void use_power_up(int id);

        void handle_missile();
        void launch_missile();

        void add_ball(Ball *ball);
        void add_brick(RectBlock *object);
        void add_power_up_drop(PowerUpDrop *pud);
        void add_missile(Missile *missile);

        void init_ball();

        void delete_ball(Ball *ball);
        void delete_brick(RectBlock *object);
        void delete_power_up_drop(PowerUpDrop *pud);
        void delete_missile(Missile *missile);

        int get_remaining_breakable_blocks();

        // level loader

        void read_command();
        bool read_loot_table_command(std::map<PowerUp, int> &loot_map);
        bool read_pity_table_command(std::map<PowerUp, int> &pity_map);

        void construct_object();
        void construct_brick(bool unbreakable);
        void construct_grid_of_bricks();

        void set_drop_frequency();
        void set_drop_offset();
        void set_pity_threshold();
        void set_power_up_weight(std::map<PowerUp, int> &weights, PowerUp power_up);

        void construct_loot_table_from_file();
        void construct_pity_table_from_file();

    public:
        Level(Rect subject_rect, double x_separation, double y_separation, int x_repeat, int y_repeat);
        Level();

        void bind_stat(GameStat &game_stat);
        void bind_playing_field(PlayingField &pf);
        void bind_notification_bar(NotificationBar &bar);
        int load_level_by_file(std::string filename);

        void render_screen();
        void launch_ball();
        void run_loop();

        bool all_bricks_destroyed();
        bool has_ball();

        void reset_level();
        void destroy_objects();

        void set_quit_status(bool status);
        bool quitted();
        int pause_menu();
};

#endif