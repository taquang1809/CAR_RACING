#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

struct NutBam {
    SDL_Rect khung;
    string ten;
    bool duocChon = false;
};

class Menu {
private:
    SDL_Texture* anhNen = nullptr;
    vector<NutBam> danhSachNut;
	int nutDuocChon;
public:
    bool taiAnhNen(SDL_Renderer* renderer, const string& fileAnh) {
        SDL_Surface* surface = IMG_Load(fileAnh.c_str());
        if (!surface) {
            cout << "Khong tai duoc anh: " << fileAnh << " - " << IMG_GetError() << endl;
            return false;
        }
        anhNen = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return anhNen != nullptr;
    }

    void ThemNut(int x, int y, int w, int h, const string& ten) {
        NutBam nut;
        nut.khung = {x, y, w, h};
        nut.ten = ten;
        danhSachNut.push_back(nut);
    }

    int xuLySuKien(SDL_Event& e) {
        if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);

            for (int i = 0; i < (int)danhSachNut.size(); i++) {
                auto& nut = danhSachNut[i];
                bool namTrongNut = (x > nut.khung.x && x < nut.khung.x + nut.khung.w &&
                                    y > nut.khung.y && y < nut.khung.y + nut.khung.h);
                nut.duocChon = namTrongNut;
                if (namTrongNut && e.type == SDL_MOUSEBUTTONDOWN){
                	nutDuocChon = i;
					return i;
				} 
            }
        }
        nutDuocChon = -1;
        return -1;
    }
	
    void ve(SDL_Renderer* renderer, TTF_Font* font) {
        if (anhNen) SDL_RenderCopy(renderer, anhNen, nullptr, nullptr);

        for (auto& nut : danhSachNut) {
            SDL_Color mauChu = nut.duocChon ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255};

            // Nen nut (Hieu ung sang )
            SDL_SetRenderDrawColor(renderer,
                nut.duocChon ? 100 : 40,
                nut.duocChon ? 100 : 40,
                nut.duocChon ? 100 : 40,
                255);
            SDL_RenderFillRect(renderer, &nut.khung);

            // Vien
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &nut.khung);

            // Chu
            SDL_Surface* surfaceText = TTF_RenderText_Solid(font, nut.ten.c_str(), mauChu);
            SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);

            int textW, textH;
            SDL_QueryTexture(textureText, nullptr, nullptr, &textW, &textH);
            SDL_Rect viTriChu = {
                nut.khung.x + (nut.khung.w - textW) / 2,
                nut.khung.y + (nut.khung.h - textH) / 2,
                textW, textH
            };

            SDL_RenderCopy(renderer, textureText, nullptr, &viTriChu);

            SDL_FreeSurface(surfaceText);
            SDL_DestroyTexture(textureText);
        }
    }

    void giaiPhong() {
        if (anhNen) SDL_DestroyTexture(anhNen);
        anhNen = nullptr;
    }
	
	int getNut(){
		return nutDuocChon;
	}
};

#endif


