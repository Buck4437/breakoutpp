#ifndef SHIELD_H_
#define SHIELD_H_

class Shield {

    public:
        int get_level();
        bool upgrade();
        void destroy();
        bool is_intact();
        char get_filler();

    private:
        int level = 0;
        double x1, x2, y;
};

#endif