#include "ncu.h"

#include <list>
#include <string>

#ifndef NOTIFICATION_BAR_H
#define NOTIFICATION_BAR_H

class NotificationBar {

    public:
        NotificationBar(int w, int h);
        void set_display_window(int y_start);
        void display(std::string s);
        void tick();
        void update();
        void reset();

    private:
        int width, height, timer = 0;
        std::list<std::string> msg_queue;
        std::string msg;
        WINDOW *display_window = NULL;
        void set_message(std::string message);
        void show_next_message();
};

#endif