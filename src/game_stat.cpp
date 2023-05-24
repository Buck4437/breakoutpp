#include "game_stat.h"
#include "game_stat_timer.h"
#include "general_utils.h"

#include <cmath>
#include <string>
#include <vector>

const int base_lives = 3;

// Initializes the game statistics of the game.
GameStat::GameStat() {
    total_score = 0;
    score_multi = 1;
    speed_multi = 0;
    missile = 0;
    lives_lost = 0;
    // Initially, the level counter is at zero. This counter is incremented every time a level is loaded.
    level = 0;
}

// Increase the score.
// score: The score to add (before scaling by the score multiplier).
void GameStat::add_base_score(int score) {
    total_score += score * score_multi;
}

// Increase the score multiplier by 1 for 300 frames. If another score multiplier power-up
// is collected before the timer runs out, the timer is reset and the bonus stacks additively.
void GameStat::add_score_multi() {
    ++score_multi;
    timer.score = 300;
}

// Increase the ball's speed for 200 frames.
void GameStat::add_speed() {
    ++speed_multi;
    apply_ball_speed_timer();
}

// Decrease the ball's speed for 200 frames.
void GameStat::sub_speed() {
    --speed_multi;
    apply_ball_speed_timer();
}

// Set the timer for ball speed power-ups to 200 frames.
// However, if the new power-up would cancel the effects of a previous one, the timer is set to 0.
void GameStat::apply_ball_speed_timer() {
    if (speed_multi != 0) {
        timer.speed = 200;
    } else {
        timer.speed = 0;
    }
}

// Increases the number of missiles.
void GameStat::add_missile() {
    ++missile;
}

// Decreases the number of missiles by 1, and prevents the player from firing another for 30 frames.
void GameStat::sub_missile() {
    if (can_fire_missile()) {
        --missile;
        timer.missile_cooldown = 30;
    }
}

// Decreases the number of lives by 1. In the code, this is implementing by increasing lives lost by 1.
// If the number of lives lost equals the total number of lives (3 + bonus lives), then the game ends.
void GameStat::sub_lives() {
    ++lives_lost;
}

// Increases the level counter by 1. Called when the player clears a level.
void GameStat::add_level() {
    ++level;
}

// Returns the current score.
int GameStat::get_score() {
    return total_score;
}

// Returns the current score multiplier.
int GameStat::get_score_multi() {
    return score_multi;
}

// Gets the ball's actual speed multiplier:
// +20% for every increase speed power-up.
// -20% for every decrease speed power-up, in the sense that -100% = half speed.
double GameStat::get_speed_multi() {
    if (speed_multi >= 0) {
        return 1 + speed_multi * 0.25;
    } else {
        return 1.0 / (1 - speed_multi * 0.25);
    }
}

// Returns the number of missiles.
int GameStat::get_missile() {
    return missile;
}

// Returns if the player can fire a missile at this moment:
// They should have at least 1 missile, and cannot have fired one in the last 30 frames.
bool GameStat::can_fire_missile() {
    return missile > 0 && timer.missile_cooldown <= 0;
}

// Returns lives remaining, which is the starting number of lives (3) + extra lives - lives lost.
int GameStat::get_lives() {
    return base_lives + get_extra_lives() - lives_lost;
}

// Returns the number of extra lives granted.
// Extra lives are granted at: 5k, 20k, 45k, 80k, ... points.
// A player would get about 2 to 3 extra lives if they complete the whole game.
int GameStat::get_extra_lives() {
    return std::floor(std::sqrt(get_score() / 5000));
}

// Returns if the player has any lives remaining. If not, they have lost the game.
bool GameStat::has_lives() {
    return get_lives() > 0;
}

// Returns the current level number.
int GameStat::get_level() {
    return level;
}

// Returns the timer used for keeping track of the duration of power-ups left.
GameStatTimer GameStat::get_timer() {
    return timer;
}

// Ticks each timer for 1 frame.
void GameStat::tick_timer() {
    if (timer.score > 0) {
        --timer.score;
    }

    if (timer.speed > 0) {
        --timer.speed;
    }

    if (timer.speed == 0 && speed_multi != 0) {
        speed_multi = 0;
    }

    if (timer.missile_cooldown > 0) {
        --timer.missile_cooldown;
    }

    if (timer.pity > 0) {
        --timer.pity;
    }
}

// If the timer hits 0 for a power-up, reset that power-up.
void GameStat::check_depleted_power_ups() {
    if (timer.score == 0 && score_multi != 1) {
        score_multi = 1;
    }
    if (timer.speed == 0 && speed_multi != 0) {
        speed_multi = 0;
    }
}

// Resets all the timers, including the score multiplier, the ball speed modifier,
// the missile firing cooldown and the pity power-up drop timer.
void GameStat::reset_timer() {
    timer.score = 0;
    timer.speed = 0;
    timer.missile_cooldown = 0;
    timer.pity = -1;
}

// Sets the pity timer to 500 frames.
// After 500 frames, a random power-up drops from the top of the playing field.
void GameStat::set_pity_timer() {
    timer.pity = 500;
}

// Resets all the power-up statistics, including the score multiplier,
// the ball speed modifier and the missile count.
void GameStat::reset_lv_stats() {
    score_multi = 1;
    speed_multi = 0;
    missile = 0;
}

// Resets every statistic, including all the power-up statistics, as well as
// the number of lives, the score and the level.
void GameStat::reset_all_stats() {
    reset_lv_stats();
    lives_lost = 0;
    total_score = 0;
    level = 0;
}

// Returns the string used to display the number of lives.
// The string shows the current number of lives, and the number of bonus lives gained.
std::string GameStat::live_display() {
    return std::to_string(get_lives()) + " (" + std::to_string(get_extra_lives()) + " bonus)";
}

// Prints out the game stats to the side of the screen,
// including the score, multiplier, and power-up timers.
void GameStat::draw_display_window() {
    wattron(display_window, A_NORMAL);
    werase(display_window);
    wborder(display_window, '|', '|', '-', '-', '+', '+', '+', '+');
    int max_y, max_x;
    getmaxyx(display_window, max_y, max_x);

    wmove(display_window, 0, 2);
    wprintw(display_window, "[LEVEL %i]", get_level());

    wmove(display_window, 1, 1);
    wprintw(display_window, "BALLS");

    wmove(display_window, 2, 1);
    wprintw(display_window, "%s", live_display().c_str());

    wmove(display_window, 1, max_x / 2 - 2);
    wprintw(display_window, "SCORE");

    wmove(display_window, 2, max_x / 2 - std::to_string(get_score()).size() / 2);
    wprintw(display_window, "%i", get_score());

    wmove(display_window, 3, max_x / 2 - 1);

    if (get_timer().score % (300 / (get_timer().score + 1)) == 1 || get_timer().score > 100) {
        wprintw(display_window, "x%i", get_score_multi());
    }

    wmove(display_window, 1, max_x - 18);
    wprintw(display_window, "MISSILE (Press C)");

    wmove(display_window, 2, max_x - std::to_string(get_missile()).size() - 1);
    wprintw(display_window, "%i", get_missile());

    wmove(display_window, 4, max_x / 2 - 10);

    if (get_timer().speed > 0) {
        wmove(display_window, 4, max_x / 2 - 17);
        wprintw(display_window, "SPEED [                    ]");
        wmove(display_window, 4, max_x / 2 - 10);
        wprintw(display_window, "%s", strmulti(".", get_timer().speed / 10).c_str());
    }
    wrefresh(display_window);
}

// Hides the game statistics window.
void GameStat::clear_window() {
    werase(display_window);
    wrefresh(display_window);
}

// Creates a new display window for showing game stats.
void GameStat::set_display_window(double max_x, double max_y) {
    // display_window = newwin(40, 60, 1, max_x + 1);
    display_window = newwin(5, max_x - 1, 1, 1);
}

// Returns the display window for showing game stats.
WINDOW *GameStat::get_display_window() {
    return display_window;
}
