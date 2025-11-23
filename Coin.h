#ifndef COIN_H
#define COIN_H

#include "DoiTuongGame.h"
#include <SDL2/SDL.h>

#include<string>

class Coin : public DoiTuongGame {
private:
    bool biAn; // coin da bi an chua
    SDL_Texture* texCoin;//texture xu
    SDL_Renderer* renderer; // ve anh
    std::string duongDanAnh; //duong dan anh

public:
    Coin(int x, int y, int w, int h, int tocDo,SDL_Renderer* renderer)
        : DoiTuongGame(x, y, w, h, tocDo),
		 biAn(false),
		 texCoin(nullptr),
		 renderer(renderer)
	{
		duongDanAnh="assets/coin.bmp";
		SDL_Surface* surface=SDL_LoadBMP(duongDanAnh.c_str());
		if(!surface){
			SDL_Log("Khong load duoc anh coin: %s",SDL_GetError());
			texCoin=nullptr;
		}
		else{
			Uint32 mauNen=SDL_MapRGB(surface->format,0,0,0);
			SDL_SetColorKey(surface,SDL_TRUE,mauNen);
			texCoin=SDL_CreateTextureFromSurface(renderer,surface);
			SDL_FreeSurface(surface);
			
		}
	}	 
	
    void capNhat() override {
        if (!biAn) {
            x -= tocDo; // coin di chuyen sang trai theo nen
            if (x + w < 0) { 
                biAn = true; // ra khoi man hinh thi bien mat
            }
        }
    }

    void ve(SDL_Renderer* renderer) override {
        if (!biAn && texCoin) {
            SDL_Rect coinRect={x,y,w,h};
            SDL_RenderCopy(renderer, texCoin,NULL, &coinRect);
        }
    }
	
    void datLai(int xMoi = 0, int yMoi = 0, int tocDoMoi = 5) {
        x = xMoi;
        y = yMoi;
        tocDo = tocDoMoi;
        biAn = false;
    }

	
    SDL_Rect getRect() const {
        return {x, y, w, h};
    }

    void an() { biAn = true; }  // danh dau coin bi an
    bool daAn() const { return biAn; }
};

#endif


