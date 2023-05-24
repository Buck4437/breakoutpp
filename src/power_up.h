#include <string>

#ifndef POWER_UP_H_
#define POWER_UP_H_

struct PowerUp {
    public:
        int id;
        std::string symbol;
        std::string abbr;
        bool operator<(const PowerUp &a) const {
            return id < a.id;
        };
};

#endif