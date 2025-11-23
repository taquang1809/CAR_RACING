#ifndef DOITUONGGAME_H
#define DOITUONGGAME_H

#include <SDL2/SDL.h>

class DoiTuongGame {
protected:
    int x, y, w, h, tocDo;

public:
    DoiTuongGame(int x, int y, int w, int h, int tocDo)
        : x(x), y(y), w(w), h(h), tocDo(tocDo) {}
	DoiTuongGame() : x(0), y(0), w(0), h(0), tocDo(0) {} 
    virtual ~DoiTuongGame() {}

    virtual void capNhat() = 0;
    virtual void ve(SDL_Renderer* renderer) = 0;
};

#endif
