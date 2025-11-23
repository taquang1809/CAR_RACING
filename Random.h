#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <ctime>

// Ham sinh so ngau nhien trong khoang [min, max]
inline int random(int min, int max) {
    static std::mt19937 rng(static_cast<unsigned int>(time(nullptr))); // seed theo gio
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

#endif


