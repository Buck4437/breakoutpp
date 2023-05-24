#include "game.h"
#include "menu.h"

#include <fstream>
#include <iostream>
#include <string>

Game game;
Menu sample;
std::vector<std::string> filenames;
std::string level_index = "data/index.txt";

// Initializes the colors in ncurses so that they can be displayed.
void init_color_pairs() {
    // For making colors consistent across platforms
    if (has_colors() && COLORS >= 8) {
        int const clr_no[] = {COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE};
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                init_pair(i * 8 + j, clr_no[i], clr_no[j]);
            }
        }
    } else {
        int const clr_no[] = {COLOR_BLACK, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE};
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                init_pair(i * COLORS + j, clr_no[i], clr_no[j]);
            }
        }
    }
}

// Runs the main menu screen. Returns false if the player quit the game, and true otherwise.
bool run_menu() {
    sample.init_ui();
    int choice = sample.run_main_menu_ui();

    switch (choice) {

    // -1 is returned when player pressed 'q' to force quit the program in main menu
    case -1:
        return false;

    case 0:
        game.run_game(filenames);
        break;

    case 1:
        sample.run_leaderboard_ui();
        break;

    case 2:
        sample.run_credits_ui();
        break;

    case 3:
        sample.run_how_to_play_ui();
        break;

    case 4:
        if (sample.run_exit_ui() == 0) {
            return false;
        }
        break;
    }

    return true;
}

// The main method to run the game.
int main() {

    initscr();
    resize_term(50, 200);

    start_color();
    init_color_pairs();

    std::ifstream fin;

    fin.open(level_index);
    if (!fin.fail()) {
        std::string file_name;
        while (fin >> file_name) {
            if (file_name != "") {
                filenames.push_back(file_name);
            }
        }

        while (run_menu()) {
            continue;
        }
        endwin();
    } else {
        endwin();
        throw std::runtime_error("Failed to open " + level_index + ". Please check if it is missing.");
    }

}
