#include "notification_bar.h"
#include "ncu.h"

#include <algorithm>
#include <cmath>
#include <string>

// Constructs a notification bar with the given dimensions.
// w: The width of the bar.
// h: The height of the bar.
NotificationBar::NotificationBar(int w, int h) {
    width = w;
    height = h;
}

// Generates an ncurses window for the notification bar to use.
// y_start: The vertical position of the top-left corner of the bar.
// 0 means the 1st line (the top), 1 means the 2nd line, etc.
void NotificationBar::set_display_window(int y_start) {
    display_window = newwin(height, width, y_start, 1);
}

// Displays a piece of text. If other text is being displayed,
// the notification bar puts it in a queue and displays it later.
void NotificationBar::display(std::string s) {
    if (s != "") {
        msg_queue.push_back(s);
    }
}

// Updates the notification bar to display the current message.
// Text to display will be centered in the bar.
void NotificationBar::update() {
    werase(display_window);
    if (msg != "") {
        int mid_x = width / 2, mid_y = height / 2;
        wmove(display_window, mid_y, mid_x - msg.length() / 2);
        wprintw(display_window, msg.c_str());
    }
    wrefresh(display_window);
}

// Sets the current message to show.
// message: The current message to show.
void NotificationBar::set_message(std::string message) {
    msg = message;
    update();
}

// If a message is queued, show the next message. Otherwise, show nothing.
// By default, each message is shown for 50 frames, which will be decreased if many messages are queued.
void NotificationBar::show_next_message() {
    if (msg_queue.size() > 0) {
        set_message(msg_queue.front());
        msg_queue.pop_front();
        timer = std::max(15.0, 50 - std::pow(msg_queue.size(), 3));
    } else {
        set_message("");
    }
}

// Ticks the bar's timer, which controls for how many frames a message should be displayed.
// If the timer reaches 0, the next message is displayed and the timer is reset.
void NotificationBar::tick() {
    if (timer > 0) {
        --timer;
    }
    if (timer <= 0) {
        show_next_message();
    }
}

// Resets the notification bar, which clears its message queue.
void NotificationBar::reset() {
    timer = 0;
    set_message("");
    while (msg_queue.size() > 0) {
        msg_queue.pop_front();
    }
}
