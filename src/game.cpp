#include "game.h"
#include "ball.h"
#include "game_stat.h"
#include "leaderboard.h"
#include "level.h"
#include "ncu.h"
#include "paddle.h"
#include "playing_field.h"
#include "power_up.h"
#include "power_up_drop.h"
#include "power_up_list.h"
#include "rect.h"
#include "rect_block.h"
#include "rect_wall.h"
#include "well.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <string>

// Readies the screen for drawing characters in the terminal.
void Game::initialize_screen() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    setlocale(LC_ALL, "en_US.UTF-8");
    // timeout(-1);

    pf.set_display_window();
    game_stat.set_display_window(pf.max_x, pf.max_y);
    bar.set_display_window(pf.max_y + 6);
    nodelay(pf.get_display_window(), true);
    keypad(pf.get_display_window(), true);
}

// Loads a level from a file and initializes it.
// level_file: The address of the level file to load.
void Game::initialize_level(std::string level_file) {
    cur_lv = new Level();
    cur_lv->bind_stat(game_stat);
    cur_lv->bind_playing_field(pf);
    cur_lv->bind_notification_bar(bar);
    cur_lv->load_level_by_file(level_file);
    cur_lv->render_screen();
    cur_lv->set_quit_status(false);
}

// Returns if the current round has ended.
// A round starts when a player launches their ball and ends when they lose all balls on the playing field.
bool Game::round_ended() {
    return !cur_lv->has_ball() || level_ended();
}

// Returns if the current level has ended.
bool Game::level_ended() {
    return cur_lv->all_bricks_destroyed() || game_ended();
}

// Returns if the current game has ended.
bool Game::game_ended() {
    return !game_stat.has_lives() || cur_lv->quitted();
}

// Runs a round, starting from launching the ball to losing all balls on the field.
void Game::run_round() {
    bar.display("Press SPACE to launch the ball.");
    bar.tick();

    cur_lv->launch_ball();
    bar.reset();
    do {
        cur_lv->run_loop();
        // Wait for a frame
        napms(33);
    } while (!round_ended());

    if (!cur_lv->has_ball()) {
        game_stat.sub_lives();
    }

    if (game_stat.has_lives()) {
        cur_lv->reset_level();
        game_stat.reset_lv_stats();
        game_stat.reset_timer();
        bar.reset();
    }
}

// Runs a level, starting with all the bricks and ending when there are no bricks left.
// level_file: The address of the level file to load.
void Game::run_level(std::string level_file) {
    initialize_level(level_file);
    game_stat.add_level();

    do {
        run_round();
    } while (!level_ended());

    cur_lv->render_screen();
    cur_lv->destroy_objects();
    delete cur_lv;

    game_stat.reset_lv_stats();
    game_stat.reset_timer();
    bar.reset();
}

// Runs a game, starting from level 1 and ending when the player loses all lives or clears all levels.
// filenames: The addresses of the level files to load sequentially.
void Game::run_game(std::vector<std::string> filenames) {
    initialize_screen();

    bool all_completed = true;
    for (auto filename : filenames) {
        run_level(filename);
        if (game_ended()) {
            all_completed = false;
            break;
        }
    }

    // Print game over screen
    print_stats(all_completed);
    while (true) {
        int ch = wgetch(pf.get_display_window());
        if (ch == 10 || ch == 27 || ch == ' ') {
            break;
        }
        napms(33);
    }
    add_record_to_leaderboard();

    // Reset stats
    game_stat.reset_all_stats();
    game_stat.reset_timer();
}

// Prints the game over screen.
// all_completed: Whether the player cleared every level. If so, a congratulatory message will be shown.
void Game::print_stats(bool all_completed) {
    game_stat.clear_window();
    WINDOW *pf_win = pf.get_display_window();

    werase(pf_win);

    std::vector<std::string> strings = {
        all_completed ? "Game Complete!" : "Game Over",
        "",
        "Highest Level: " + std::to_string(game_stat.get_level()),
        "Score: " + std::to_string(game_stat.get_score()),
        "",
        "Press ENTER to continue"};

    int y_begin = strings.size() / 2;
    for (int i = 0; i < strings.size(); i++) {
        std::string str = strings.at(i);
        wmove(pf_win, y_begin + pf.max_y / 2 + i * 2, pf.max_x / 2 - str.length() / 2);
        wprintw(pf_win, str.c_str());
    }
    wrefresh(pf_win);
}

// If the player makes it to the top 10, asks for the player's name and adds this run to the leaderboard.
void Game::add_record_to_leaderboard() {
    Record new_record;
    int rank = lb.check_if_top_10(game_stat.get_score());

    if (rank != -1) {
        std::string name = get_player_name(rank);
        if (name.length() == 0 || std::all_of(name.begin(), name.end(), [](char c) { return std::isspace(c); })) {
            new_record = rc.get_new_record(game_stat.get_score(), "Guest");
        } else {
            new_record = rc.get_new_record(game_stat.get_score(), name);
        }
        lb.add_to_leaderboard(new_record);
        lb.revise_leaderboard();
        lb.push_leaderboard();
    }
    return;
}

// Displays a screen for the player to type their name, and then returns it.
// rank: The rank that the player achieved on the leaderboard.
std::string Game::get_player_name(int rank) {
    WINDOW *pf_win = pf.get_display_window();
    werase(pf_win);
    keypad(pf_win, TRUE);

    std::vector<std::string> strings = {
        "Congratulations",
        "",
        "You made it to rank " + std::to_string(rank),
        "What name would you like to be remembered by",
        "on the Hall of Fame?",
        "",
        "Please input your name (max 15 characters)",
        "Use up/ down arrow key to delete character",
        "Press Enter when you are finished."};

    int y_begin = strings.size() / 2;
    for (int i = 0; i < strings.size(); i++) {
        std::string str = strings.at(i);
        wmove(pf_win, y_begin + pf.max_y / 3 + i * 2, pf.max_x / 2 - str.length() / 2);
        wprintw(pf_win, str.c_str());
    }

    std::string name = "";
    int pos = name.length();
    echo();
    curs_set(1);
    int ch;

    while ((ch = wgetch(pf_win)) != '\n') {
        switch (ch) {
        case 127:
        case KEY_DOWN:
        case KEY_UP:
        case KEY_BACKSPACE:
            if (pos > 0) {
                name.erase(pos - 1, 1);
                pos--;
            }
            break;
        default:
            if (isprint(ch) && name.length() < 15) {
                name.insert(pos, 1, ch);
                pos++;
            }
            break;
        }

        werase(pf_win);
        for (int i = 0; i < strings.size(); i++) {
            std::string str = strings.at(i);
            wmove(pf_win, y_begin + pf.max_y / 3 + i * 2, pf.max_x / 2 - str.length() / 2);
            wprintw(pf_win, str.c_str());
        }
        wmove(pf_win, 25, 20);
        wprintw(pf_win, "Your name: %s", name.c_str());
        wrefresh(pf_win);
        napms(33);
    }
    curs_set(0);
    noecho();

    return name;
}
