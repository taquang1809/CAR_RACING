#ifndef CHEDO2NGUOI_H
#define CHEDO2NGUOI_H

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

class CheDo2Nguoi {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    Mix_Chunk* amThanhAnCoin;
    DongHoGame dongHo;
    Nen nen;
    Xe xe1, xe2;

    std::vector<Coin> dsCoin;
    std::vector<Buff> dsBuff;
    std::vector<VatCan> dsVatCan;

    Uint32 mocSpawnCoin, mocSpawnBuff, mocSpawnVatCan;
    bool ketThuc;
    int ketQua = 0;

public:
    CheDo2Nguoi(SDL_Renderer* renderer)
        : renderer(renderer),
          nen(0, 0, 800, 600, 5, dongHo, renderer),
          xe1(100, 200, 60, 40, 5, SDL_SCANCODE_W, SDL_SCANCODE_S,
              SDL_Color{255, 255, 255, 255}, renderer, "assets/xexanh.png"),
          xe2(100, 400, 60, 40, 5, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN,
              SDL_Color{255, 255, 255, 255}, renderer, "assets/xedo.png"),
          ketThuc(false)
    {
        dongHo.batDauDem();
        mocSpawnCoin = 3000;
        mocSpawnBuff = 5000;
        mocSpawnVatCan = 2000;

        font = TTF_OpenFont("assets/font.ttf", 24);
        if (!font) {
            std::cout << "Khong mo duoc font: " << TTF_GetError() << std::endl;
        }
        amThanhAnCoin = Mix_LoadWAV("assets/coin.wav");
        if (!amThanhAnCoin) {
            cout << "Khong load duoc am thanh coin: " << Mix_GetError() << endl;
        }
    }

    ~CheDo2Nguoi() {
        if (font) TTF_CloseFont(font);
        if (amThanhAnCoin) Mix_FreeChunk(amThanhAnCoin);
    }
	
    bool daKetThuc() const { return ketThuc; }

    void capNhat(const Uint8* phim) {
        if (ketThuc) return;

        xe1.xuLySuKien(phim);
        xe2.xuLySuKien(phim);
        nen.capNhat();
        xe1.capNhat();
        xe2.capNhat();
        xe1.tangDiem(nen.getTocDo());
        xe2.tangDiem(nen.getTocDo());

        // Spawn coin
        if (dongHo.getThoiGianMS() >= mocSpawnCoin) {
            int yRand = random(100, 430);
            dsCoin.push_back(Coin(800, yRand, 60, 60, nen.getTocDo(), renderer));
            mocSpawnCoin += random(2500, 3500);
        }

        // Spawn buff
        if (dongHo.getThoiGianMS() >= mocSpawnBuff) {
            int yRand = random(100, 460);
            int loai = random(0, 2);
            dsBuff.push_back(Buff(800, yRand, 60, 60, nen.getTocDo(), loai, renderer));
            mocSpawnBuff += 7000;
        }

         // Spawn vat can
        int max = 3000, min = 2500;
        if (dongHo.getThoiGianGiay() > 60) { max = 2000; min = 1000; }
        if (dongHo.getThoiGianMS() >= mocSpawnVatCan) {
            int loai = random(0, 2);
            int w, h, yRand, toc = nen.getTocDo();
            if (loai == 0) { w = 60; h = 40; toc *= 2; yRand = random(100, 440); }
            else if (loai == 2) { w = 50; h = 100; yRand = random(100, 400);}
            else {w = h = 60; yRand = random(100, 440);}
            dsVatCan.push_back(VatCan(800, yRand, w, h, toc, loai, renderer));
            mocSpawnVatCan += random(min, max);
        }

        // Coin
        for (auto &xu : dsCoin) {
            xu.capNhat();
            SDL_Rect rXe1 = {xe1.getX(), xe1.getY(), xe1.getW(), xe1.getH()};
            SDL_Rect rXe2 = {xe2.getX(), xe2.getY(), xe2.getW(), xe2.getH()};
            SDL_Rect rCoin = xu.getRect();

            if (!xu.daAn()) {
                if (SDL_HasIntersection(&rXe1, &rCoin)) {
                    xe1.tangDiem(150);
                    xu.an();
                    Mix_PlayChannel(-1, amThanhAnCoin, 0);
                } else if (SDL_HasIntersection(&rXe2, &rCoin)) {
                    xe2.tangDiem(150);
                    xu.an();
                    Mix_PlayChannel(-1, amThanhAnCoin, 0);
                }
            }
        }

        // Buff
        for (auto it = dsBuff.begin(); it != dsBuff.end();) {
            it->capNhat();
            bool biXoa = false;
            if (it->vaChamVoiXe(xe1)) {
                if (it->getLoai() == 0) xe1.kichHoatLaChan(7000);
                else if (it->getLoai() == 1) xe1.kichHoatDoubleDiem(5000);
                else if (it->getLoai() == 2) xe2.kichHoatLamCham(5000);
                biXoa = true;
            }
            else if (it->vaChamVoiXe(xe2)) {
                if (it->getLoai() == 0) xe2.kichHoatLaChan(7000);
                else if (it->getLoai() == 1) xe2.kichHoatDoubleDiem(5000);
                else if (it->getLoai() == 2) xe1.kichHoatLamCham(5000);
                biXoa = true;
            }
            if (it->daRaNgoai()) biXoa = true;
            if (biXoa) it = dsBuff.erase(it);
            else ++it;
        }

        // Vat can
        for (auto it = dsVatCan.begin(); it != dsVatCan.end();) {
            it->capNhat();
            bool biXoa = false;

            SDL_Rect rXe1 = {xe1.getX(), xe1.getY(), xe1.getW(), xe1.getH()};
            SDL_Rect rXe2 = {xe2.getX(), xe2.getY(), xe2.getW(), xe2.getH()};
            SDL_Rect rVat = it->getRect();

            bool thuaXe1 = false, thuaXe2 = false;

            if (SDL_HasIntersection(&rXe1, &rVat)) {
                if (xe1.xuLyVaCham(rVat)) {
                    ketQua = 2;
                }
                biXoa = true;
            }
            if (SDL_HasIntersection(&rXe2, &rVat)) {
                if (xe2.xuLyVaCham(rVat)) {
                    ketQua = 1;
                }
                biXoa = true;
            }

            if (it->daRaNgoai()) biXoa = true;
            if (biXoa) it = dsVatCan.erase(it);
            else ++it;

            if (ketQua) {
                ketThuc = true;
                break;
            }
        }

        if (dongHo.hetThoiGian(90)) {
            ketThuc = true;
            if (xe1.getDiem() > xe2.getDiem() && ketQua == 0) ketQua = 1;
            else if (xe1.getDiem() < xe2.getDiem() && ketQua == 0) ketQua = 2 ;
            else if (xe1.getDiem() == xe2.getDiem() && ketQua == 0) ketQua = 3;
        }
    }

    void ve() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        nen.ve(renderer);
        for (auto &xu : dsCoin) xu.ve(renderer);
        for (auto &b : dsBuff) b.ve(renderer);
        for (auto &v : dsVatCan) v.ve(renderer);
        xe1.ve(renderer);
        xe2.ve(renderer);

        // Hien diem
        if (font) {
            SDL_Color mauTrang = {255, 255, 255, 255};
            std::string diem1 = "Player 1 : " + std::to_string(xe1.getDiem());
            std::string diem2 = "Player 2 : " + std::to_string(xe2.getDiem());

            SDL_Surface* s1 = TTF_RenderText_Solid(font, diem1.c_str(), mauTrang);
            SDL_Surface* s2 = TTF_RenderText_Solid(font, diem2.c_str(), mauTrang);

            SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);

            SDL_Rect r1 = {20, 20, s1->w, s1->h};
            SDL_Rect r2 = {20, 50, s2->w, s2->h};

            SDL_RenderCopy(renderer, t1, NULL, &r1);
            SDL_RenderCopy(renderer, t2, NULL, &r2);

            SDL_FreeSurface(s1); SDL_FreeSurface(s2);
            SDL_DestroyTexture(t1); SDL_DestroyTexture(t2);
        }
        
        // Thoi gian dem nguoc 90s
        int thoiGianConLai = 90 - dongHo.getThoiGianGiay();
        if (thoiGianConLai < 0) thoiGianConLai = 0;
		SDL_Color mauTrang = {255, 255, 255, 255};
        std::string textTG = "Time: " + std::to_string(thoiGianConLai) + "s";
        SDL_Surface* sTG = TTF_RenderText_Solid(font, textTG.c_str(), mauTrang);
        if (sTG) {
            SDL_Texture* tTG = SDL_CreateTextureFromSurface(renderer, sTG);
            SDL_Rect rTG = {650, 20, sTG->w, sTG->h}; // góc trên bên phải
            SDL_RenderCopy(renderer, tTG, NULL, &rTG);
            SDL_DestroyTexture(tTG);
            SDL_FreeSurface(sTG);
        }

        SDL_RenderPresent(renderer);
    }
	int getDiemXe1(){ return xe1.getDiem(); }
    int getDiemXe2(){ return xe2.getDiem(); }
    int getKetQua(){ return ketQua; }
};

#endif
