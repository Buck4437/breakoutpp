#include <string>

#ifndef RECORD_H_
#define RECORD_H_

struct Record {
        int score;
        int time;
        std::string name;
        static Record get_new_record(int points, std::string username);
};

#endif