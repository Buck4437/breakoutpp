#include "menu.h"
#include "leaderboard.h"
#include "ncu.h"

#include <fstream>
#include <iostream>

std::vector<std::string> exit_options = {"     Yes      ",
                                         "      No      "};

std::vector<std::string> pause_options = {"        Quit        ",
                                          "       Resume       "};

std::vector<std::string> menu_options = {" Start         ",
                                         " Leaderboard   ",
                                         " Credits       ",
                                         " How to play   ",
                                         " Exit          "};

// Initializes the UI so that it is ready for printing.
void Menu::init_ui() {

    start_color();
    display_window = newwin(rows, cols, start_row, start_col);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    keypad(display_window, TRUE);
    nodelay(display_window, TRUE);
    return;
}

// Refreshes the terminal to clean up characters from previous screens.
void Menu::clear_ui(WINDOW *win) {

    wrefresh(win);
    wmove(win, 0, 0);
}

// If the user pressed one of the arrow keys, change the selected choice.
// For example, if the user pressed the down key, "choice" is incremented by 1.
// This corresponds to selecting the next option in a menu screen.
void Menu::update_choice(int ch, int option_count, int &choice) {
    switch (ch) {
    case KEY_UP:
    case KEY_LEFT:
        choice--;
        if (choice < 0) {
            choice = option_count - 1;
        }
        break;
    case KEY_DOWN:
    case KEY_RIGHT:
        choice++;
        if (choice > option_count - 1) {
            choice = 0;
        }
        break;
    }
}

// Displays the credits UI.
void Menu::draw_leaderboard_ui() {

    Leaderboard lb;

    const int l_row = 30;
    const int l_col = 60;
    const int l_startRow = rows / 2 - 15;
    const int l_startCol = cols / 2 - 10;
    leaderboard_window = newwin(l_row, l_col, l_startRow, l_startCol);

    keypad(leaderboard_window, TRUE);
    clear_ui(leaderboard_window);
    box(leaderboard_window, 0, 0);

    std::vector<Record> record_lst = Leaderboard().get_records();

    std::string s = "----------- *** Leaderboard *** -----------";
    mvwprintw(leaderboard_window, 3, (l_col - s.length()) / 2, s.c_str());
    int place = 1;
    for (std::vector<Record>::iterator i = record_lst.begin(); i != record_lst.end(); ++i) {
        mvwprintw(leaderboard_window, 2 * place + 5, 10, std::to_string(place).c_str());
        mvwprintw(leaderboard_window, 2 * place + 5, 20, (i->name).c_str());
        mvwprintw(leaderboard_window, 2 * place + 5, 40, std::to_string(i->score).c_str());
        place++;
    }

    // for helping user to leave the window
    s = "Press ENTER to go back";
    mvwprintw(leaderboard_window, l_row - 2, l_col - s.length() - 2, s.c_str());

    wrefresh(leaderboard_window);

    record_lst.clear();
}

// Runs the leaderboard UI and listens to user input.
// When enter, escape or space is pressed, the UI closes.
void Menu::run_leaderboard_ui() {
    draw_leaderboard_ui();
    while (true) {
        int ch = wgetch(leaderboard_window);
        if (ch == 10 || ch == 27 || ch == ' ') {
            break;
        }
    }
    delwin(leaderboard_window);
}

// Displays the credits UI.
void Menu::draw_credits_ui() {

    const int c_row = 22;
    const int c_col = 60;
    const int c_startRow = (rows - c_row) / 2;
    const int c_startCol = (cols - c_col) / 2;
    contributor_window = newwin(c_row, c_col, c_startRow, c_startCol);

    keypad(contributor_window, TRUE);
    clear_ui(contributor_window);
    box(contributor_window, 0, 0);

    std::vector<std::string> contributor_lst = {"Buck4437",
                                                "ChatterCube",
                                                "minghinshi",
                                                "PeterOwO",
                                                "raZylAWY"};

    std::string s = "----------- === Contributors === -----------";
    mvwprintw(contributor_window, 3, (c_col - s.length()) / 2, s.c_str());
    int place = 1;
    for (int i = 0; i < contributor_lst.size(); ++i) {
        mvwprintw(contributor_window, 6 + 2 * i, (c_col - contributor_lst[i].length()) / 2, contributor_lst[i].c_str());
    }

    // for helping user to leave the window
    s = "Press ENTER to go back";
    mvwprintw(contributor_window, c_row - 2, c_col - s.length() - 2, s.c_str());

    wrefresh(contributor_window);

    contributor_lst.clear();
}

// Runs the credits UI and listens to user input.
// When enter, escape or space is pressed, the UI closes.
void Menu::run_credits_ui() {
    draw_credits_ui();
    while (true) {
        int ch = wgetch(contributor_window);
        if (ch == 10 || ch == 27 || ch == ' ') {
            break;
        }
    }
    delwin(contributor_window);
}

// Uses ncurses to print out a page, which is a .txt file you can find in the data folder.
// page_num: The page to be printed.
void Menu::print_page(int page_num) {
    std::ifstream fin;
    std::string file_name = "data/instructions" + std::to_string(page_num) + ".txt";
    fin.open(file_name);

    if (fin.fail()) {
        throw std::runtime_error("Cannot find " + file_name + ". Please check if the file exists.");
        exit(1);
    }

    std::string line;
    int line_num = 0;
    while (std::getline(fin, line)) {
        mvwprintw(how_to_play_window, line_num + 2, 5, line.c_str());
        line_num++;
    }

    fin.close();
}

// Displays the how-to-play UI.
// page_num: The page that should be displayed (0-3). There are 4 pages in total.
void Menu::draw_how_to_play_ui(int page_num) {

    const int h_row = 29;
    const int h_col = 59;
    const int h_startRow = (rows - h_row) / 2;
    const int h_startCol = (cols - h_col) / 2;
    how_to_play_window = newwin(h_row, h_col, h_startRow, h_startCol);

    keypad(how_to_play_window, TRUE);
    clear_ui(how_to_play_window);
    box(how_to_play_window, 0, 0);
    print_page(page_num);
    wrefresh(how_to_play_window);
}

// Runs the how-to-play UI and listens to user input.
// When the user presses enter or escape, ends the UI.
// When the user presses left/right, change the plage that is being displayed.
void Menu::run_how_to_play_ui() {
    int choice = 0;
    while (true) {
        draw_how_to_play_ui(choice);
        int ch = wgetch(how_to_play_window);
        if (ch == 10 || ch == 27) {
            delwin(how_to_play_window);
            break;
        }
        update_choice(ch, 4, choice);
    }
}

// Displays the exit confirmation UI on the screen.
// choice: The currently selected choice. (0 = 1st choice, 1 = 2nd choice, etc.)
void Menu::draw_exit_ui(int choice) {

    const int e_row = 10;
    const int e_col = 40;
    const int e_startRow = (rows - e_row) / 2;
    const int e_startCol = (cols - e_col) / 2;
    exit_window = newwin(e_row, e_col, e_startRow, e_startCol);

    keypad(exit_window, TRUE);
    clear_ui(exit_window);
    box(exit_window, 0, 0);

    std::string s1 = "QUIT";
    mvwprintw(exit_window, 0.2 * e_row, (e_col - s1.length()) / 2, s1.c_str());
    s1 = "Are you sure you want to quit?";
    mvwprintw(exit_window, 0.5 * e_row, (e_col - s1.length()) / 2, s1.c_str());

    int option_size = exit_options.size();
    for (int i = 0; i < option_size; ++i) {
        wattron(exit_window, COLOR_PAIR(((choice == i) ? 1 : 2)));
        mvwprintw(exit_window, e_row - 2, 3 + i * 0.5 * e_col, exit_options[i].c_str());
    }

    wrefresh(exit_window);
}

// Runs the exit UI and listens to user input.
// If the user presses escape, exit the menu and returns option 1 ("No").
// If the user presses enter, returns the option that the player chose (0 = "Yes", 1 = "No").
int Menu::run_exit_ui() {
    int ch = 0;
    int choice = 1;
    while (ch != 'q') {
        draw_exit_ui(choice);
        ch = wgetch(exit_window);
        if ((ch == 10 && choice == 1) || ch == 27) {
            delwin(exit_window);
            return 1;
        } else if ((ch == 10 && choice == 0)) {
            break;
        } else {
            update_choice(ch, exit_options.size(), choice);
        }
    }
    return 0;
}

// Displays the pause UI on the screen.
// choice: The currently selected choice. (0 = 1st choice, 1 = 2nd choice, etc.)
void Menu::draw_pause_ui(int choice) {

    const int p_row = 10;
    const int p_col = 50;
    const int p_startRow = (rows - p_row) / 3;
    const int p_startCol = (cols - p_col) / 10;
    pause_window = newwin(p_row, p_col, p_startRow, p_startCol);

    keypad(pause_window, TRUE);
    clear_ui(pause_window);
    box(pause_window, 0, 0);

    std::string s1 = "PAUSED";
    mvwprintw(pause_window, 0.2 * p_row, (p_col - s1.length()) / 2, s1.c_str());

    int option_size = pause_options.size();
    for (int i = 0; i < option_size; ++i) {
        wattron(pause_window, COLOR_PAIR(((choice == i) ? 1 : 2)));
        mvwprintw(pause_window, p_row - 2, 2 + i * 0.5 * p_col, pause_options[i].c_str());
    }

    wrefresh(pause_window);
}

// Runs the pause menu UI and listens to user input.
// If the user presses escape, exit the menu and returns option 1 ("No").
// If the user presses enter, returns the option that the player chose (0 = "Yes", 1 = "No").
int Menu::run_pause_ui() {
    int ch = 0;
    int choice = 1;
    while (true) {
        draw_pause_ui(choice);
        ch = wgetch(pause_window);
        if ((ch == 10 && choice == 1) || ch == 27) {
            delwin(pause_window);
            return 1;
        } else if (ch == 10 && choice == 0) {
            return 0;
        } else {
            update_choice(ch, pause_options.size(), choice);
        }
    }
}

// Draws the main menu UI.
// choice: The currently selected choice. (0 = 1st choice, 1 = 2nd choice, etc.)
void Menu::draw_main_menu_ui(int choice) {
    clear_ui(display_window);
    wmove(display_window, 4, 10);
    wattron(display_window, COLOR_PAIR(1));
    waddstr(display_window, "Breakout++");

    const int pos_y = 15;
    const int pos_x = 10;
    int option_size = menu_options.size();

    for (int i = 0; i < option_size; ++i) {
        wattron(display_window, COLOR_PAIR(((choice == i) ? 1 : 2)));
        mvwprintw(display_window, i + pos_y, pos_x, menu_options[i].c_str());
    }

    wrefresh(display_window);
}

// Runs the main menu UI and listens to user input.
// When the user presses enter, returns the option that the player chose
// (0 = first option in the list, 1 = second option, etc.)
int Menu::run_main_menu_ui() {
    int ch = 0;
    noecho();
    while (true) {
        draw_main_menu_ui(main_menu_choice);
        ch = wgetch(display_window);
        // press 'q' to quit
        if (ch == 'q' || ch == 'Q') {
            return -1;
        } else if (ch == 10) {
            break;
        } else {
            update_choice(ch, menu_options.size(), main_menu_choice);
        }
        napms(33);
    }
    delwin(display_window);
    return main_menu_choice;
}
