#ifndef NEN_H
#define NEN_H

#include "DoiTuongGame.h"
#include "DongHoGame.h"

#include <string>

class Nen : public DoiTuongGame {
private:
    DongHoGame& dongHo;        // tham chi?u d?ng h?
    SDL_Texture* texNen;       // texture n?n
    SDL_Renderer* renderer;    // renderer de ve
    std::string duongDanAnh;   // du?ng d?n tuy?t d?i ?nh BMP

public:
    // Constructor
    Nen(int x, int y, int w, int h, int tocDo, DongHoGame& dongHoGame,
        SDL_Renderer* renderer)
        : DoiTuongGame(x, y, w, h, tocDo),
          dongHo(dongHoGame),
          renderer(renderer)
    {

        duongDanAnh = "assets/nenduaxe.png";

        SDL_Surface* surface = SDL_LoadBMP(duongDanAnh.c_str());
        if (!surface) {
            SDL_Log("Khong load duoc anh nen: %s", SDL_GetError());
            texNen = nullptr;
        } else {
            texNen = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }


    void capNhat() override {

        Uint32 giay = dongHo.getThoiGianGiay();
        if (giay < 20) tocDo = 5;
        else if (giay < 40) tocDo = 7;
        else if (giay < 75) tocDo = 10;
        else  tocDo = 13;

        x -= tocDo;
        if (x <= -w) x = 0;
    }

    void ve(SDL_Renderer* renderer) override {
        if (!texNen) return;

        for (int k = 0; k < 2; ++k) {
            SDL_Rect dst = { x + k * w, y, w, h };
            SDL_RenderCopy(renderer, texNen, nullptr, &dst);
        }
    }
	

    void datLai() {
        // Dua nen ve vi tri ban dau va toc do mac dinh
        x = 0;
        tocDo = 5; // toc do khoi tao mac dinh
    }

	
    int getTocDo() { return tocDo; }

    ~Nen() {
        if (texNen) SDL_DestroyTexture(texNen);
    }
};

#endif


