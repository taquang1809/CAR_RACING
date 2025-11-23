#ifndef BUFF_H
#define BUFF_H

#include "DoiTuongGame.h"
#include "Xe.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Buff : public DoiTuongGame {
private:
    int loai;       // 0 = Shield, 1 = Double Score, 2 = Slow Opponent
    SDL_Texture* texBuff;
    SDL_Renderer* renderer;

    static SDL_Texture* s_texShield;
    static SDL_Texture* s_texDouble;
    static SDL_Texture* s_texSlow;
    static bool s_texturesLoaded;

    static SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            SDL_Log("Khong load dc anh buff: %s | Loai: %s", path.c_str(), IMG_GetError());
            return nullptr;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!tex) {
            SDL_Log("Kh?ng t?o ???c texture t? %s | SDL_Error: %s", path.c_str(), SDL_GetError());
        }
        return tex;
    }

    static void loadSharedTextures(SDL_Renderer* renderer) {
        if (s_texturesLoaded) return;
        s_texturesLoaded = true;

        s_texShield = loadTexture(renderer, "assets/khien.png");
        s_texDouble = loadTexture(renderer, "assets/double.png");
        s_texSlow   = loadTexture(renderer, "assets/slow.png");
    }

public:
    Buff(int x, int y, int w, int h, int tocDo, int loaiBuff, SDL_Renderer* renderer)
        : DoiTuongGame(x, y, w, h, tocDo),
          loai(loaiBuff),
          texBuff(nullptr),
          renderer(renderer)
    {
        loadSharedTextures(renderer);

        if (loai == 0) {
            texBuff = s_texShield;
        } else if (loai == 1) {
            texBuff = s_texDouble;
        } else {
            texBuff = s_texSlow;
        }

        if (!texBuff) {
            SDL_Log("? Kh?ng th?? ga?n texture Buff (loai %d)", loai);
        }
    }

    void capNhat() override {
        x -= tocDo;
    }

    void ve(SDL_Renderer* renderer) override {
        if (!texBuff) return;
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderCopy(renderer, texBuff, NULL, &rect);
    }
	
    void datLai(int xMoi = 0, int yMoi = 0, int loaiMoi = 0, int tocDoMoi = 5) {
        // Dat lai vi tri va loai buff
        x = xMoi;
        y = yMoi;
        loai = loaiMoi;
        tocDo = tocDoMoi;

        // Gan lai texture tu loai moi
        if (loai == 0) {
            texBuff = s_texShield;
        } else if (loai == 1) {
            texBuff = s_texDouble;
        } else {
            texBuff = s_texSlow;
        }
    }

	
    int getLoai() const { return loai; }
    bool daRaNgoai() const { return x + w < 0; }

    bool vaChamVoiXe(Xe& xe) {
        SDL_Rect buffRect = {x, y, w, h};
        SDL_Rect xeRect = {xe.getX(), xe.getY(), xe.getW(), xe.getH()};
        return SDL_HasIntersection(&buffRect, &xeRect);
    }

    static void freeSharedTextures() {
        if (s_texShield) { SDL_DestroyTexture(s_texShield); s_texShield = nullptr; }
        if (s_texDouble) { SDL_DestroyTexture(s_texDouble); s_texDouble = nullptr; }
        if (s_texSlow)   { SDL_DestroyTexture(s_texSlow);   s_texSlow = nullptr; }
        s_texturesLoaded = false;
    }

    ~Buff() {}
};

SDL_Texture* Buff::s_texShield = nullptr;
SDL_Texture* Buff::s_texDouble = nullptr;
SDL_Texture* Buff::s_texSlow   = nullptr;
bool Buff::s_texturesLoaded = false;

#endif


