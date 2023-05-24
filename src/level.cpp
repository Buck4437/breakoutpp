#include "level.h"
#include "ball.h"
#include "game_stat.h"
#include "loot_table.h"
#include "math_utils.h"
#include "menu.h"
#include "missile.h"
#include "ncu.h"
#include "notification_bar.h"
#include "power_up_drop.h"
#include "power_up_list.h"
#include "rect_wall.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <time.h>

// Creates a new level, which is a set of bricks to break. Clear all the bricks to complete a level.
// subject_rect: The size of a brick, defined by two of its opposite corners.
// x_separation: Horizontal separation between the centres of bricks.
// y_separation: Vertical separation between the centres of bricks.
// x_repeat: The number of rows of bricks.
// y_repeat: The number of columns of bricks.
Level::Level(Rect subject_rect, double x_separation, double y_separation, int x_repeat, int y_repeat) {

    RectBlock::create_rectblocks(bricks, subject_rect, x_separation, y_separation, x_repeat, y_repeat);

    construct_loot_table();

    init_ball();

    srand(time(0));
}

// Initializes the list of power-ups.
// When a dropping power-up is created, a power-up will be drawn from this list.
void Level::construct_loot_table() {
    loot_table = new LootTable();

    std::map<PowerUp, int> pity_map;
    pity_map[PowerUpList::MULTIBALL] = 1;
    pity_map[PowerUpList::MISSILE] = 1;

    pity_table = new LootTable(pity_map);
}

// Removes both loot tables from memory.
void Level::destruct_loot_table() {
    delete loot_table;
    delete pity_table;
}

// Creates a new level, but with no bricks created at first.
Level::Level() {

    std::set<RectBlock *> bricks = {};

    construct_loot_table();

    init_ball();

    srand(time(0));
}

// Connects the level to a GameStat, making the GameStat reusable across levels.
// game_stat: The GameStat to connect to.
void Level::bind_stat(GameStat &game_stat) {
    game_stat_ptr = &game_stat;
}

// Connects the level to a PlayingField, making the PlayingField reusable across levels.
// pf: the PLayingField to connect to.
void Level::bind_playing_field(PlayingField &pf) {
    pf_ptr = &pf;
}

void Level::bind_notification_bar(NotificationBar &bar) {
    bar_ptr = &bar;
}

// Before the player launches the ball, make the ball swing from left to right.
// When the player presses Space, launch the ball.
void Level::launch_ball() {
    int ch = 'a';
    double period = 90;
    do {
        period += 6.28;
        if (period > 360) {
            period -= 360;
        }
        double offset = std::sin(from_deg(period)) * 0.9;

        ch = wgetch(pf_ptr->get_display_window());

        if (ch == 'p' || ch == 'P') {
            if (pause_menu() == 0) {
                return;
            }
        }

        paddle.move_by_input(ch, well);
        for (Ball *ball : balls) {
            ball->move_to_paddle(paddle, offset);
        }

        render_screen();

        // Wait for a frame
        napms(33);
    } while (ch != ' ');
}

// Simulates one frame of the game.
void Level::run_loop() {

    int init_extra_lifes = game_stat_ptr->get_extra_lives();

    // Move
    int ch = wgetch(pf_ptr->get_display_window());
    paddle.move_by_input(ch, well);

    if ((ch == 'c' || ch == 'C') && game_stat_ptr->can_fire_missile()) {
        game_stat_ptr->sub_missile();
        launch_missile();
    }

    if (ch == 'p' || ch == 'P') {
        pause_menu();
    }

    std::set<Ball *> removed_ball;
    for (Ball *ball : balls) {
        ball->set_speed_multi(game_stat_ptr->get_speed_multi());
        move_ball(*ball);

        if (ball->outside_well(well)) {
            removed_ball.insert(ball);
        }
    }

    for (Ball *ball : removed_ball) {
        delete_ball(ball);
    }

    // Check collision of missiles
    handle_missile();

    // Check collision with power-up drops
    handle_power_ups();
    tick_power_up_timers();
    render_screen();

    if (init_extra_lifes < game_stat_ptr->get_extra_lives()) {
        bar_ptr->display("Score milestone reached! +1 life");
    }

    bar_ptr->tick();
}

// Displays everything inside the Level to the main screen (PlayingField).
void Level::render_screen() {
    PlayingField pf = *pf_ptr;

    // Clear screen before printing
    werase(pf.get_display_window());
    wrefresh(pf.get_display_window());

    game_stat_ptr->draw_display_window();

    // Draw the paddle
    paddle.draw_pf(pf);

    for (RectWall *i : bricks) {
        i->draw_to_pf(pf);
    }

    // Draw the missiles
    for (Missile *m : missiles) {
        m->draw_pf(pf);
    }

    // Draw the power-up drops
    for (PowerUpDrop *p : power_up_drops) {
        p->draw_pf(pf);
    }

    // Draw the well
    well.draw(pf);

    for (Ball *ball : balls) {
        // Draw the ball
        ball->draw_pf(pf);
    }

    wrefresh(pf.get_display_window());
}

// Simulates the movement of a Ball in one frame, making it bounce and destroying bricks as needed.
// ball: The ball to simulate.
void Level::move_ball(Ball &ball) {

    double segments = ball.speed() / ball.max_stepping;

    for (int i = 0; i < segments; i++) {
        double travelFactor = std::min(segments - i, 1.0) / segments;
        ball.move_by_velocity(travelFactor);

        // Collision checking
        // Check collision for the wall
        ball.if_collide_rebound(well, travelFactor);

        // Check collision for the paddle
        ball.if_collide_rebound(paddle, travelFactor);

        // Check collision for all edges in the game field
        for (RectBlock *i : bricks) {
            if (!(i->broken)) {
                // Rebound and mark as broken
                ball.if_collide_rebound(i, travelFactor);
            }
        }

        remove_broken_block();
    }
}

// Removes all the bricks that have been broken by the Ball.
// Drop a power-up for every 5 bricks broken (by default). This value may be changed by the .bl file.
void Level::remove_broken_block() {
    std::vector<RectBlock *> broken_blocks;

    // Select broken tile
    for (auto i = bricks.begin(); i != bricks.end(); ++i) {
        RectBlock *brick = (*i);
        if (brick->broken) {
            broken_blocks.push_back(brick);
        }
    }

    // Remove from list
    for (RectBlock *brick : broken_blocks) {
        // Spawn power-ups
        if ((broke_count - offset) >= 0) {
            if ((broke_count - offset) % drop_freq == 0) {
                PowerUp pu = loot_table->draw_power_up();
                Vector2 pos = brick->get_rect().center().add_x(0.5);
                PowerUpDrop *pud = new PowerUpDrop(pos, 0.1, pu);
                add_power_up_drop(pud);
            }
        }

        ++broke_count;

        game_stat_ptr->add_base_score(brick->points);
        delete_brick(brick);
    }
}

// Simulates the falling power-ups in the playing field for one frame.
void Level::handle_power_ups() {
    std::vector<PowerUpDrop *> removed;
    for (auto &pud : power_up_drops) {
        if (pud->collide(paddle)) {
            removed.push_back(pud);
            // Extra functions for handling power-up collections
            use_power_up(pud->powerup.id);
        } else {
            pud->move();
            if (pud->outside_well(well)) {
                removed.push_back(pud);
            }
        }
    }

    for (PowerUpDrop *pud : removed) {
        delete_power_up_drop(pud);
    }
}

// Ticks the timers of active power-ups for one frame.
void Level::tick_power_up_timers() {
    game_stat_ptr->tick_timer();
    game_stat_ptr->check_depleted_power_ups();

    if (is_pity_activated()) {
        if (game_stat_ptr->get_timer().pity <= 0) {
            PowerUp pu = pity_table->draw_power_up();

            Rect box = well.get_inner_box();
            Vector2 pos = box.top_center();

            PowerUpDrop *pud = new PowerUpDrop(pos, 0.1, pu);
            add_power_up_drop(pud);

            game_stat_ptr->set_pity_timer();
            bar_ptr->display("Pity power-up spawned");
        }
    }
}

// Returns if the pity system is activated.
// While it is activated, a free power-up will drop from the top of the screen every 500 frames.
bool Level::is_pity_activated() {
    if (pity_threshold >= 100) {
        return false;
    } else if (pity_threshold <= 0) {
        return true;
    }
    return (!all_bricks_destroyed()) && broke_count > (get_remaining_breakable_blocks() * (100.0 / (100 - pity_threshold) - 1)); // 80%+ cleared
}

// Decides what happens when the paddle collects a dropping power-up.
// id: The id of the collected power-up.
void Level::use_power_up(int id) {

    if (id == PowerUpList::PAD_EXPAND.id) {
        bool success = paddle.buff(well);
        if (success) {
            bar_ptr->display("<== Pad Expand ==>");
        } else {
            bar_ptr->display("Maximum pad width reached, no effect.");
        }
        return;
    }

    if (id == PowerUpList::PAD_REDUCE.id) {
        bool success = paddle.nerf();
        if (success) {
            bar_ptr->display(">== Pad Shrink ==<");
        } else {
            bar_ptr->display("Mininum pad width reached, no effect.");
        }
        return;
    }

    if (id == PowerUpList::FAST_BALL.id) {
        game_stat_ptr->add_speed();
        double speed = game_stat_ptr->get_speed_multi();

        std::stringstream ss;
        ss << ">>> Ball Speed Up (x" << std::fixed << std::setprecision(2) << speed << ")";
        bar_ptr->display(ss.str());

        return;
    }

    if (id == PowerUpList::SLOW_BALL.id) {
        game_stat_ptr->sub_speed();
        double speed = game_stat_ptr->get_speed_multi();

        std::stringstream ss;
        ss << "<<< Ball Slow Down (x" << std::fixed << std::setprecision(2) << speed << ")";
        bar_ptr->display(ss.str());
        return;
    }

    if (id == PowerUpList::MULTIBALL.id) {
        Rect phb = paddle.get_paddle_hitbox();
        Vector2 pos = phb.bottom_center().add_y(0.2);
        // Base velocity of two balls, can be changed later.
        // The asymmetry of the balls is intentional.
        Ball *left = new Ball(pos, {-0.3, 0.5});
        Ball *right = new Ball(pos, {0.5, 0.3});
        add_ball(left);
        add_ball(right);

        bar_ptr->display("Multiball");
        return;
    }

    if (id == PowerUpList::MULTIPLIER.id) {
        game_stat_ptr->add_score_multi();

        bar_ptr->display("Score multiplier +1");
        return;
    }

    if (id == PowerUpList::SHIELD.id) {
        bool success = well.shield.upgrade();
        if (success) {
            bar_ptr->display("Shield Upgrade Level " + std::to_string(well.shield.get_level()));
        } else {
            bar_ptr->display("Shield max level reached, no effect.");
        }

        return;
    }

    if (id == PowerUpList::MISSILE.id) {
        game_stat_ptr->add_missile();
        bar_ptr->display("Missile +1 (Press C to launch)");
    }
}

// Simulates the fired missiles for one frame.
void Level::handle_missile() {
    std::vector<Missile *> removed;
    for (auto &missile : missiles) {
        bool broken = false;
        for (RectBlock *brick : bricks) {
            if (!(brick->broken)) {
                if (missile->collide(brick)) {
                    brick->break_brick();
                    removed.push_back(missile);
                    broken = true;
                    break;
                }
            }
        }
        if (!broken) {
            missile->move();
            if (missile->hit_well(well)) {
                removed.push_back(missile);
            }
        }
    }

    remove_broken_block();

    for (Missile *missile : removed) {
        delete_missile(missile);
    }
}

// Fires a missile. The missile spawns at the top-centre of the paddle.
void Level::launch_missile() {
    Rect hitbox = paddle.get_paddle_hitbox();
    Missile *missile_mid = new Missile(hitbox.top_center(), 0.8);
    Missile *missile_left = new Missile(hitbox.top_left(), 0.8);
    Missile *missile_right = new Missile(hitbox.top_right(), 0.8);
    add_missile(missile_left);
    add_missile(missile_mid);
    add_missile(missile_right);
}

// Returns the remaining number of bricks.
int Level::get_remaining_breakable_blocks() {
    int count = 0;
    for (RectBlock *brick : bricks) {
        if (!brick->unbreakable) {
            ++count;
        }
    }
    return count;
}

// Returns if all the bricks on the field has been destroyed, i.e. there are 0 bricks left.
// Used to decide if the player should advance to the next level.
bool Level::all_bricks_destroyed() {
    return get_remaining_breakable_blocks() <= 0;
}

// Returns if there is at least still 1 ball on the field.
// Used to decide if the player should lose a life.
bool Level::has_ball() {
    return balls.size() > 0;
}

// Destroys all dynamic objects when a level ends.
void Level::destroy_objects() {
    destruct_loot_table();

    std::set<Ball *> balls_cp = balls;
    std::set<RectBlock *> bricks_cp = bricks;
    std::set<PowerUpDrop *> pud_cp = power_up_drops;
    std::set<Missile *> missiles_cp = missiles;

    for (Ball *ball : balls_cp) {
        delete_ball(ball);
    }

    for (RectBlock *brick : bricks_cp) {
        delete_brick(brick);
    }

    for (PowerUpDrop *pud : pud_cp) {
        delete_power_up_drop(pud);
    }

    for (Missile *missile : missiles_cp) {
        delete_missile(missile);
    }
}

// Adds a ball to the playing field.
// ball: The ball to add.
void Level::add_ball(Ball *ball) {
    balls.insert(ball);
}

// Adds a brick to the playing field.
// brick: The brick to add.
void Level::add_brick(RectBlock *brick) {
    bricks.insert(brick);
    brick->parent_set = &bricks;
}

// Adds a dropping power-up to the playing field.
// pud: The dropping power-up to add.
void Level::add_power_up_drop(PowerUpDrop *pud) {
    power_up_drops.insert(pud);
}

// Adds a missile to the playing field.
// missile: The missile to add.
void Level::add_missile(Missile *missile) {
    missiles.insert(missile);
}

// Removes a ball from the playing field.
// ball: The ball to remove.
void Level::delete_ball(Ball *ball) {
    balls.erase(ball);
    delete ball;
}

// Removes a brick from the playing field.
// brick: The brick to remove.
void Level::delete_brick(RectBlock *brick) {
    bricks.erase(brick);
    delete brick;
}

// Removes a dropping power-up from the playing field.
// pud: The dropping power-up to remove.
void Level::delete_power_up_drop(PowerUpDrop *pud) {
    power_up_drops.erase(pud);
    delete pud;
}

// Removes a missile from the playing field.
// missile: The missile to remove.
void Level::delete_missile(Missile *missile) {
    missiles.erase(missile);
    delete missile;
}

// Creates a new ball at the top-centre of the paddle that initially moves downward.
void Level::init_ball() {
    if (balls.empty()) {
        Ball *ball = new Ball({0.0, 0.0}, {0.0, -0.6});
        balls.insert(ball);
    }
}

// Resets the level, which:
// - Clears all the balls, dropping power-ups and missiles on the field.
// - Resets the game statistics to initial values.
// - Resets the size of the paddle.
// - Generates a new ball.
void Level::reset_level() {

    for (Ball *ball : balls) {
        delete ball;
    }
    balls.clear();

    for (PowerUpDrop *pud : power_up_drops) {
        delete pud;
    }
    power_up_drops.clear();

    for (Missile *missile : missiles) {
        delete missile;
    }
    missiles.clear();

    game_stat_ptr->reset_lv_stats();
    paddle.reset_width(well);

    init_ball();
}

// Set the "is_quitted" variable.
// If set to "true", the Game object will detect that and end the game immediately.
// status: The value to set "is_quitted" to.
void Level::set_quit_status(bool status) {
    is_quitted = status;
}

// Returns whether or not the player has quit the game.
bool Level::quitted() {
    return is_quitted;
}

// Pauses the game and opens up the pause menu.
// Returns the option that the player chose (0 = exit game, 1 = resume).
int Level::pause_menu() {
    Menu sample;

    int choice = sample.run_pause_ui();

    if (choice == 0) {
        set_quit_status(true);
    }
    return choice;
}