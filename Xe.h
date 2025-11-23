#ifndef XE_H
#define XE_H

#include "DoiTuongGame.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Xe : public DoiTuongGame {
private:
    SDL_Scancode phimLen;
    SDL_Scancode phimXuong;
    SDL_Color mau;
    SDL_Texture* texXe;
    SDL_Renderer* renderer;
    std::string duongDanAnh;

    bool coLaChan;
    Uint32 hetHanLaChan;

    bool doubleDiem;
    Uint32 hetHanDoubleDiem;

    bool biLamCham;
    Uint32 hetHanLamCham;

    int tocDoGoc;
    int diem;

    float vanTocY;
    float tangToc;
    float maSat;

public:
    Xe(int x, int y, int w, int h, int tocDo,
       SDL_Scancode len, SDL_Scancode xuong,
       SDL_Color mauXe,
       SDL_Renderer* renderer,
       const std::string& pathAnh)
        : DoiTuongGame(x, y, w, h, tocDo),
          phimLen(len), phimXuong(xuong), mau(mauXe),
          renderer(renderer), duongDanAnh(pathAnh)
    {
        coLaChan = false;
        doubleDiem = false;
        biLamCham = false;
        tocDoGoc = tocDo;
        diem = 0;
        hetHanLaChan = hetHanDoubleDiem = hetHanLamCham = 0;

        vanTocY = 0.0f;
        tangToc = 0.5f;
        maSat   = 0.93f;

        SDL_Surface* surface = IMG_Load(duongDanAnh.c_str());
        if (!surface) {
            SDL_Log("Kh?ng load ???c ?nh xe (IMG_Load): %s", IMG_GetError());
            texXe = nullptr;
        } else {
            texXe = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            if (!texXe) {
                SDL_Log("Kh?ng t?o ???c texture xe: %s", SDL_GetError());
            }
        }
    }

    void xuLySuKien(const Uint8* trangPhim) {
        float heSoCham = (biLamCham ? 0.5f : 1.0f);

        if (trangPhim[phimLen]) {
            vanTocY -= tangToc * heSoCham;
        }
        if (trangPhim[phimXuong]) {
            vanTocY += tangToc * heSoCham;
        }

        if (vanTocY > 7.0f)  vanTocY = 7.0f;
        if (vanTocY < -7.0f) vanTocY = -7.0f;
    }

    void capNhat() override {
        Uint32 now = SDL_GetTicks();

        if (coLaChan && now > hetHanLaChan)           coLaChan = false;
        if (doubleDiem && now > hetHanDoubleDiem)     doubleDiem = false;
        if (biLamCham && now > hetHanLamCham)         biLamCham = false;

        y += (int)vanTocY;

        vanTocY *= maSat;

        if (y < 100) {
            y = 100;
            vanTocY = 0;
        }
        if (y + h > 500) {
            y = 500 - h;
            vanTocY = 0;
        }
    }

    void tangDiem(int tocDoNen) {
        int diemCong = tocDoNen;
        if (doubleDiem) diemCong *= 2;
        diem += diemCong;
    }

    void ve(SDL_Renderer* renderer) override {
        SDL_Rect xeRect = {x, y, w, h};
        if (texXe) {
            SDL_SetTextureColorMod(texXe, mau.r, mau.g, mau.b);
            SDL_RenderCopy(renderer, texXe, nullptr, &xeRect);
        } else {
            SDL_SetRenderDrawColor(renderer, mau.r, mau.g, mau.b, 255);
            SDL_RenderFillRect(renderer, &xeRect);
        }
    }

    bool xuLyVaCham(const SDL_Rect& vat) {
        SDL_Rect xeRect = {x, y, w, h};
        if (SDL_HasIntersection(&xeRect, &vat)) {
            if (coLaChan) {
                coLaChan = false;
                return false;
            } else {
                return true;
            }
        }
        return false;
    }

    void kichHoatLaChan(Uint32 thoiGian) {
        coLaChan = true;
        hetHanLaChan = SDL_GetTicks() + thoiGian;
    }

    void kichHoatDoubleDiem(Uint32 thoiGian) {
        doubleDiem = true;
        hetHanDoubleDiem = SDL_GetTicks() + thoiGian;
    }

    void kichHoatLamCham(Uint32 thoiGian) {
        biLamCham = true;
        hetHanLamCham = SDL_GetTicks() + thoiGian;
    }
	

    void datLai(int xMoi = 100, int yMoi = 300) {
        // Dat lai vi tri
        x = xMoi;
        y = yMoi;

        vanTocY = 0.0f;
        tangToc = 0.5f;
        maSat   = 0.93f;

        coLaChan = false;
        doubleDiem = false;
        biLamCham = false;
        hetHanLaChan = 0;
        hetHanDoubleDiem = 0;
        hetHanLamCham = 0;

        diem = 0;
        tocDo = tocDoGoc;
    }

	
    int getDiem() const { return diem; }
    bool getLaChan() const { return coLaChan; }
    bool getDoubleDiem() const { return doubleDiem; }
    bool getLamCham() const { return biLamCham; }
    SDL_Color getMau() const { return mau; }

    int getX() const { return x; }
    int getY() const { return y; }
    int getW() const { return w; }
    int getH() const { return h; }
};

#endif


