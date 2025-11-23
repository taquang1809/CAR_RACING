#ifndef VATCAN_H
#define VATCAN_H

#include "DoiTuongGame.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class VatCan : public DoiTuongGame {
private:
    int loai;   // 0 = Xe, 1 = Rao, 2 = Container
    bool daXoa;
    SDL_Texture* texVatCan;
    SDL_Renderer* renderer;
    std::string duongDanAnh;

    static SDL_Texture* s_texXe;
    static SDL_Texture* s_texRao;
    static SDL_Texture* s_texContainer;
    static bool s_texturesLoaded;

    static SDL_Texture* loadTextureFromImage(SDL_Renderer* renderer, const std::string& path) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            SDL_Log("? Khong load duoc anh %s: %s", path.c_str(), IMG_GetError());
            return nullptr;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return tex;
    }

    static void loadSharedTextures(SDL_Renderer* renderer) {
        if (s_texturesLoaded) return;
        s_texturesLoaded = true;

        s_texXe        = loadTextureFromImage(renderer, "assets/xetrang.png");    // loai 0
        s_texRao       = loadTextureFromImage(renderer, "assets/ho.png");        // loai 1
        s_texContainer = loadTextureFromImage(renderer, "assets/containerp.png"); // loai 2
    }

public:
    VatCan(int x, int y, int w, int h, int tocDo, int loaiVatCan, SDL_Renderer* renderer)
        : DoiTuongGame(x, y, w, h, tocDo),
          loai(loaiVatCan),
          daXoa(false),
          texVatCan(nullptr),
          renderer(renderer)
    {
        loadSharedTextures(renderer);

        if (loai == 0) {
            texVatCan = s_texXe;
            duongDanAnh = "assets/xetrang.png";
        } else if (loai == 1) {
            texVatCan = s_texRao;
            duongDanAnh = "assets/ho.png";
        } else {
            texVatCan = s_texContainer;
            duongDanAnh = "assets/containerp.png";
        }

        if (!texVatCan)
            SDL_Log("? Khong the gan texture cho VatCan loai %d (%s)", loai, duongDanAnh.c_str());
    }

    void capNhat() override {
        x -= tocDo;
        if (x + w < 0) daXoa = true;
    }

    void ve(SDL_Renderer* renderer) override {
        if (!texVatCan) return;
        SDL_Rect rect = { x, y, w, h };
        SDL_RenderCopy(renderer, texVatCan, nullptr, &rect);
    }
	
    void datLai(int xMoi = 0, int yMoi = 0, int loaiMoi = 0) {
        x = xMoi;
        y = yMoi;
        loai = loaiMoi;
        daXoa = false;
        // tocDo duoc giu nguyen de theo toc do nen
    }

	
    SDL_Rect getRect() const { return SDL_Rect{ x, y, w, h }; }
    int getLoai() const { return loai; }
    bool daRaNgoai() const { return daXoa; }

    static void freeSharedTextures() {
        if (s_texXe)        { SDL_DestroyTexture(s_texXe); s_texXe = nullptr; }
        if (s_texRao)       { SDL_DestroyTexture(s_texRao); s_texRao = nullptr; }
        if (s_texContainer) { SDL_DestroyTexture(s_texContainer); s_texContainer = nullptr; }
        s_texturesLoaded = false;
    }

    ~VatCan() {}
};

SDL_Texture* VatCan::s_texXe = nullptr;
SDL_Texture* VatCan::s_texRao = nullptr;
SDL_Texture* VatCan::s_texContainer = nullptr;
bool VatCan::s_texturesLoaded = false;

#endif


