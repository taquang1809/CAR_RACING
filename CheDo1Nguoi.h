#ifndef CHEDO1NGUOI_H
#define CHEDO1NGUOI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <iostream>
#include <string>
#include "Nen.h"
#include "Xe.h"
#include "Coin.h"
#include "Buff.h"
#include "VatCan.h"
#include "DongHoGame.h"
#include "random.h"
#include "DiemSo.h"

class CheDo1Nguoi {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    Mix_Chunk* amThanhAnCoin;
    DongHoGame dongHo;
    Nen nen;
    Xe xe;
    
    std::vector<Coin> dsCoin;
    std::vector<Buff> dsBuff;
    std::vector<VatCan> dsVatCan;

    Uint32 mocSpawnCoin, mocSpawnBuff, mocSpawnVatCan;
    bool thua;

public:
	
    CheDo1Nguoi(SDL_Renderer* renderer)
        : renderer(renderer),
          font(nullptr),
          dongHo(),
          nen(0, 0, 800, 600, 5, dongHo, renderer),
          xe(100, 300, 60, 40, 5,
             SDL_SCANCODE_W, SDL_SCANCODE_S,
             SDL_Color{255, 255, 255, 255},
             renderer,
             "assets/xexanhla.png"),
          mocSpawnCoin(0), mocSpawnBuff(0), mocSpawnVatCan(0),
          thua(false)
    {
        dongHo.batDauDem();
        mocSpawnCoin = 3500;
        mocSpawnBuff = 5000;
        mocSpawnVatCan = 3000;
		
        font = TTF_OpenFont("assets/font.ttf", 28);
        if (!font) {
            std::cout << "Khong mo duoc font: " << TTF_GetError() << std::endl;
        }
        
        amThanhAnCoin = Mix_LoadWAV("assets/coin.wav");
        if (!amThanhAnCoin) {
            cout << "Khong load duoc am thanh coin: " << Mix_GetError() << endl;
        }
    }

    ~CheDo1Nguoi() {
        if (font) TTF_CloseFont(font);
        if (amThanhAnCoin) Mix_FreeChunk(amThanhAnCoin);
    }
    
    bool daThua() const { return thua; }

    void capNhat(const Uint8* phim) {
        if (thua) return;

        xe.xuLySuKien(phim);
        nen.capNhat();
        xe.capNhat();
        xe.tangDiem(nen.getTocDo());

        // Spawn coin
        if (dongHo.getThoiGianMS() >= mocSpawnCoin) {
            int yRand = random(100, 440);
            dsCoin.push_back(Coin(800, yRand, 60, 60, nen.getTocDo(), renderer));
            mocSpawnCoin += random(2500, 3500);
        }

        // Spawn buff
        if (dongHo.getThoiGianMS() >= mocSpawnBuff) {
            int yRand = random(100, 460);
            int loai = random(0, 1);
            dsBuff.push_back(Buff(800, yRand, 60, 60, nen.getTocDo(), loai, renderer));
            mocSpawnBuff += 7000;
        }

        // Spawn vat can
        int max = 3000, min = 2500;
        if (dongHo.getThoiGianGiay() > 20) { max = 2500; min = 2000; }
        else if (dongHo.getThoiGianGiay() > 40) { max = 2000; min = 1500; }
        else if (dongHo.getThoiGianGiay() > 75) { max = 2000; min = 1000; }
        if (dongHo.getThoiGianMS() >= mocSpawnVatCan) {
            int loai = random(0, 2);
            int w, h, yRand, toc = nen.getTocDo();
            if (loai == 0) { w = 60; h = 40; toc *= 2; yRand = random(100, 440); }
            else if (loai == 2) { w = 50; h = 100; yRand = random(100, 400);}
            else {w = h = 60; yRand = random(100, 440);}
            dsVatCan.push_back(VatCan(800, yRand, w, h, toc, loai, renderer));
            mocSpawnVatCan += random(min, max);
        }

        // Cap nhat coin
        for (auto &xu : dsCoin) {
            xu.capNhat();
            SDL_Rect rXe = {xe.getX(), xe.getY(), xe.getW(), xe.getH()};
            SDL_Rect rCoin = xu.getRect();
            if (!xu.daAn() && SDL_HasIntersection(&rXe, &rCoin)) {
                xe.tangDiem(50);
                xu.an();
                Mix_PlayChannel(-1, amThanhAnCoin, 0);
            }
        }

        // Cap nhat buff
        for (auto it = dsBuff.begin(); it != dsBuff.end();) {
            it->capNhat();
            bool biXoa = false;
            if (it->vaChamVoiXe(xe)) {
                if (it->getLoai() == 0) xe.kichHoatLaChan(7000);
                else if (it->getLoai() == 1) xe.kichHoatDoubleDiem(5000);
                else if (it->getLoai() == 2) xe.kichHoatLamCham(5000);
                biXoa = true;
            }
            if (it->daRaNgoai()) biXoa = true;
            if (biXoa) it = dsBuff.erase(it); else ++it;
        }

        // Cap nhat vat can va xu ly va cham
        for (auto it = dsVatCan.begin(); it != dsVatCan.end();) {
            it->capNhat();
            bool biXoa = false;

            SDL_Rect rXe = {xe.getX(), xe.getY(), xe.getW(), xe.getH()};
            SDL_Rect rVat = it->getRect();

            if (SDL_HasIntersection(&rXe, &rVat)) {
                if (xe.xuLyVaCham(rVat)) {
                    thua = true;

                    // Luu diem vao file (chi luu cho che do 1 nguoi)
                    DiemSo ds;
                    ds.luuDiem(xe.getDiem());
                }
                biXoa = true;
            }
            if (it->daRaNgoai()) biXoa = true;
            if (biXoa) it = dsVatCan.erase(it);
            else ++it;
        }
	}

    void ve() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        nen.ve(renderer);
        for (auto &xu : dsCoin) xu.ve(renderer);
        for (auto &b : dsBuff) b.ve(renderer);
        for (auto &v : dsVatCan) v.ve(renderer);
        xe.ve(renderer);

        if (font) {
            std::string text = "Score : " + std::to_string(xe.getDiem());
            SDL_Color mauTrang = {255, 255, 255, 255};

            SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), mauTrang);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    SDL_Rect viTri = {5, 20, surface->w, surface->h};
                    SDL_RenderCopy(renderer, texture, NULL, &viTri);
                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }
        }

        SDL_RenderPresent(renderer);
    }
    
    getDiem(){
    	return xe.getDiem();
	}
	
	void datLai(){
		xe.datLai(); nen.datLai(); dongHo.datLai(); 
		for (auto xu : dsCoin) xu.datLai();
		for (auto v : dsVatCan) v.datLai();
		for (auto b : dsBuff) b.datLai();
	}
		
};

#endif
