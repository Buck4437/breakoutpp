#include <string>
#include <vector>

#include "ncu.h"

#ifndef MENU_H_
#define MENU_H_

class Menu {

    public:
        const int rows = 40;
        const int cols = 120;
        const int start_row = 0;
        const int start_col = 0;
        int main_menu_choice = 0;

        void init_ui();
        void clear_ui(WINDOW *win);
        void update_choice(int ch, int option_count, int &choice);

        void draw_leaderboard_ui();
        void run_leaderboard_ui();

        void draw_credits_ui();
        void run_credits_ui();

        void draw_exit_ui(int choice);
        int run_exit_ui();

        void draw_pause_ui(int choice);
        int run_pause_ui();

        void print_page(int page_num);
        void draw_how_to_play_ui(int choice);
        void run_how_to_play_ui();

        void draw_main_menu_ui(int choice);
        int run_main_menu_ui();

    private:
        WINDOW *display_window;
        WINDOW *leaderboard_window;
        WINDOW *contributor_window;
        WINDOW *how_to_play_window;
        WINDOW *exit_window;
        WINDOW *pause_window;
};

#endif