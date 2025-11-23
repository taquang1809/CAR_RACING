#ifndef QUANLYMENU_H
#define QUANLYMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map> // [THÊM MỚI] Dùng map để lưu thông tin điểm
#include "Menu.h"
#include "DiemSo.h"

using namespace std;

// Cac trang thai cua game
enum TrangThaiGame {
    MENU_CHINH,
    MENU_CHON_CHE_DO,
    CHOI_1_NGUOI,
    CHOI_2_NGUOI,
    TAM_DUNG_1P,
    TAM_DUNG_2P,
    KET_THUC_1P,
    KET_THUC_2P,
    CAI_DAT,
    KY_LUC
};

class QuanLyMenu {
	private :
	    SDL_Renderer* renderer;
	    TTF_Font* font;
	    Mix_Chunk* amThanhClick;
	    Mix_Music* nhacNen;
	
	    Menu menuChinh;
	    Menu menuChonCheDo;
	    Menu menuPause1P;
	    Menu menuPause2P;
	    Menu menuKetThuc1P;
	    Menu menuKetThuc2P;
	    Menu menuCaiDat;
	    Menu menuKyLuc;
	
	    // Map luu diem
	    map<string, int> thongTinDiem = {
	        {"Score:", 0},
	        {"High score:", 0},
	        {"Player 1 Score:", 0},
	        {"Player 2 Score:", 0}
	    };
	
	    int ketQua = 0; // ket qua cua phan choi 2 nguoi

	public:
	    SDL_Texture* texKetThuc1P[3] = {nullptr, nullptr, nullptr};
	    SDL_Rect rectKetThuc1P[3];
	
	    SDL_Texture* texKetThuc2P[3] = {nullptr, nullptr, nullptr};
	    SDL_Rect rectKetThuc2P[3];
	

	    // --- Constructor ---
	    QuanLyMenu(SDL_Renderer* rend, TTF_Font* f)
	        : renderer(rend), font(f),
	          amThanhClick(nullptr), nhacNen(nullptr)
	    {
	        amThanhClick = Mix_LoadWAV("assets/click.wav");
	        if (!amThanhClick) {
	            cout << "Khong load duoc am thanh click: " << Mix_GetError() << endl;
	        }
	
	        nhacNen = Mix_LoadMUS("assets/nhacmenu.mp3");
	        if (!nhacNen) {
	            cout << "Khong load duoc nhac nen: " << Mix_GetError() << endl;
	        } else {
	            Mix_PlayMusic(nhacNen, -1);
	        }
	
	        menuChinh.taiAnhNen(renderer, "assets/menugame.jpg");
	        menuChinh.ThemNut(300, 280, 200, 50, "New Game");
	        menuChinh.ThemNut(300, 350, 200, 50, "Scoreboard");
	        menuChinh.ThemNut(300, 420, 200, 50, "Music");
	
	        menuChonCheDo.taiAnhNen(renderer, "assets/menu_bg.png");
	        menuChonCheDo.ThemNut(300, 250, 200, 50, "Single Player");
	        menuChonCheDo.ThemNut(300, 320, 200, 50, "Multiplayer");
	        menuChonCheDo.ThemNut(10, 5, 100, 30, "Back");
	
	        menuPause1P.taiAnhNen(renderer, "assets/menu_bg.png");
	        menuPause1P.ThemNut(300, 250, 200, 50, "Resume");
	        menuPause2P = menuPause1P;
	
	        menuKetThuc1P.taiAnhNen(renderer, "assets/menu_bg.png");
	        menuKetThuc1P.ThemNut(300, 320, 200, 50, "Restart");
	        menuKetThuc1P.ThemNut(300, 390, 200, 50, "Main Menu");
	
	        menuKetThuc2P.taiAnhNen(renderer, "assets/menu_bg.png");
	        menuKetThuc2P.ThemNut(300, 320, 200, 50, "Restart");
	        menuKetThuc2P.ThemNut(300, 390, 200, 50, "Main Menu");
	
	        menuCaiDat.taiAnhNen(renderer, "assets/menu_bg.png");
	        menuCaiDat.ThemNut(300, 260, 200, 50, "ON");
	        menuCaiDat.ThemNut(300, 330, 200, 50, "OFF");
	        menuCaiDat.ThemNut(10, 5, 100, 30, "Back");
	
	        menuKyLuc.taiAnhNen(renderer, "assets/menu_bg.png");
	        menuKyLuc.ThemNut(10, 5, 100, 30, "Back");
	    }
	
	    ~QuanLyMenu() {
	        if (amThanhClick) Mix_FreeChunk(amThanhClick);
	        if (nhacNen) Mix_FreeMusic(nhacNen);
	        for(int i=0;i<3;++i){
	            if(texKetThuc1P[i]) SDL_DestroyTexture(texKetThuc1P[i]);
	            if(texKetThuc2P[i]) SDL_DestroyTexture(texKetThuc2P[i]);
	        }
	    }
	
	    // --- Xu ly su kien ---
	    void capNhat(SDL_Event& e, TrangThaiGame& trangThai) {
	        int nut = -1;
	        switch (trangThai) {
	        case MENU_CHINH:
	            nut = menuChinh.xuLySuKien(e);
	            if (nut == 0) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = MENU_CHON_CHE_DO; }
	            if (nut == 1) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = KY_LUC; }
	            if (nut == 2) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = CAI_DAT; }
	            break;
	
	        case MENU_CHON_CHE_DO:
	            nut = menuChonCheDo.xuLySuKien(e);
	            if (nut == 0) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = CHOI_1_NGUOI; }
	            if (nut == 1) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = CHOI_2_NGUOI; }
	            if (nut == 2) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = MENU_CHINH; }
	            break;
	
	        case TAM_DUNG_1P:
	            nut = menuPause1P.xuLySuKien(e);
	            if (nut == 0) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = CHOI_1_NGUOI; }
	            break;
	
	        case TAM_DUNG_2P:
	            nut = menuPause2P.xuLySuKien(e);
	            if (nut == 0) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = CHOI_2_NGUOI; }
	            break;
	
	        case KET_THUC_1P:
	            nut = menuKetThuc1P.xuLySuKien(e);
	            if (nut == 0) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = CHOI_1_NGUOI; }
	            if (nut == 1) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = MENU_CHINH; }
	            break;
	
	        case KET_THUC_2P:
	            nut = menuKetThuc2P.xuLySuKien(e);
	            if (nut == 0) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = CHOI_2_NGUOI; }
	            if (nut == 1) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = MENU_CHINH; }
	            break;
	
	        case CAI_DAT:
	            nut = menuCaiDat.xuLySuKien(e);
	            if (nut == 0) { Mix_PlayChannel(-1, amThanhClick, 0); Mix_ResumeMusic(); }
	            if (nut == 1) {
	                Mix_PlayChannel(-1, amThanhClick, 0);
	                if (Mix_PlayingMusic()) Mix_PauseMusic();
	            }
	            if (nut == 2) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = MENU_CHINH; }
	            break;
	
	        case KY_LUC:
	            nut = menuKyLuc.xuLySuKien(e);
	            if (nut == 0) { Mix_PlayChannel(-1, amThanhClick, 0); trangThai = MENU_CHINH; }
	            break;
	
	        default: break;
	        }
	    }
	
	    // --- Ve menu ---
	    void ve(TrangThaiGame trangThai) {
	        switch (trangThai) {
	        case MENU_CHINH: menuChinh.ve(renderer, font); break;
	        case MENU_CHON_CHE_DO: menuChonCheDo.ve(renderer, font); break;
	        case TAM_DUNG_1P: menuPause1P.ve(renderer, font); break;
	        case TAM_DUNG_2P: menuPause2P.ve(renderer, font); break;
	        case KET_THUC_1P:
	            menuKetThuc1P.ve(renderer, font);
	            if (!texKetThuc1P[0]) taoTextureKetThuc1P();
	            for (int i = 0; i < 3; ++i) SDL_RenderCopy(renderer, texKetThuc1P[i], nullptr, &rectKetThuc1P[i]);
	            break;
	        case KET_THUC_2P:
	            menuKetThuc2P.ve(renderer, font);
	            if (!texKetThuc2P[0]) taoTextureKetThuc2P();
	            for (int i = 0; i < 3; ++i) SDL_RenderCopy(renderer, texKetThuc2P[i], nullptr, &rectKetThuc2P[i]);
	            break;
	        case CAI_DAT: menuCaiDat.ve(renderer, font); break;
	        case KY_LUC: {
	            menuKyLuc.ve(renderer, font);
	            DiemSo diem;
	            auto ds = diem.docDiem();
	            int y = 200;
	            SDL_Color mauChu = {255, 255, 255, 255};
	            for (int i = 0; i < (int)ds.size() && i < 5; ++i) {
	                string text = "Top " + to_string(i + 1) + ": " + to_string(ds[i]);
	                SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), mauChu);
	                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	                SDL_Rect vt = {310, y, surface->w, surface->h};
	                SDL_RenderCopy(renderer, texture, nullptr, &vt);
	                SDL_FreeSurface(surface);
	                SDL_DestroyTexture(texture);
	                y += 40;
	            }
	            break;
	        }
	        default: break;
	        }
	    }
	    
	    int docDiemCaoNhat(const string& path = "data/diem.txt") {
	        ifstream f(path);
	        if (!f.is_open()) return 0;
	        int val, maxD = 0;
	        while (f >> val) maxD = max(maxD, val);
	        f.close();
	        return maxD;
	    }
	
	    void capNhatKetQua1(int diemMoi) {
	        thongTinDiem["Score:"] = diemMoi;
	        thongTinDiem["High score:"] = docDiemCaoNhat("data/diem.txt"); // [THÊM MỚI]
	        for (int i = 0; i < 3; ++i) {
	            if (texKetThuc1P[i]) { SDL_DestroyTexture(texKetThuc1P[i]); texKetThuc1P[i] = nullptr; }
	        }
	    }
	
	    void capNhatKetQua2(int d1, int d2, int kq) {
	        ketQua = kq;
	        thongTinDiem["Player 1 Score:"] = d1; // [THÊM MỚI]
	        thongTinDiem["Player 2 Score:"] = d2; // [THÊM MỚI]
	        for (int i = 0; i < 3; ++i) {
	            if (texKetThuc2P[i]) { SDL_DestroyTexture(texKetThuc2P[i]); texKetThuc2P[i] = nullptr; }
	        }
	    }
	
	    void taoTextureKetThuc1P() {
	        for (int i = 0; i < 3; ++i) {
	            if (texKetThuc1P[i]) { SDL_DestroyTexture(texKetThuc1P[i]); texKetThuc1P[i] = nullptr; }
	        }
	
	        SDL_Color mauTrang = {255, 255, 255, 255};
	        string arr[3];
	        arr[0] = "GAME OVER";
	        arr[1] = "Score: " + to_string(thongTinDiem["Score:"]);
	        arr[2] = "High score: " + to_string(thongTinDiem["High score:"]);
	
	        int yPos[3] = {150, 200, 240};
	        for (int i = 0; i < 3; ++i) {
	            SDL_Surface* s = TTF_RenderText_Solid(font, arr[i].c_str(), mauTrang);
	            texKetThuc1P[i] = SDL_CreateTextureFromSurface(renderer, s);
	            rectKetThuc1P[i] = { (arr[i].find("High") != string::npos) ? 295 : 325, yPos[i], s->w, s->h };
	            SDL_FreeSurface(s);
	        }
	    }
	
	    void taoTextureKetThuc2P() {
	        for (int i = 0; i < 3; ++i) {
	            if (texKetThuc2P[i]) { SDL_DestroyTexture(texKetThuc2P[i]); texKetThuc2P[i] = nullptr; }
	        }
	
	        SDL_Color mauTrang = {255, 255, 255, 255};
	        string textKQ;
	        if (ketQua == 1) textKQ = "PLAYER 1 WINS!";
	        else if (ketQua == 2) textKQ = "PLAYER 2 WINS!";
	        else textKQ = "IT'S A DRAW!";
	
	        string arr[3];
	        arr[0] = textKQ;
	        arr[1] = "Player 1 Score: " + to_string(thongTinDiem["Player 1 Score:"]);
	        arr[2] = "Player 2 Score: " + to_string(thongTinDiem["Player 2 Score:"]);
	
	        int yPos[3] = {150, 210, 250};
	        int xPos[3] = {300, 280, 280};
	        for (int i = 0; i < 3; ++i) {
	            SDL_Surface* s = TTF_RenderText_Solid(font, arr[i].c_str(), mauTrang);
	            texKetThuc2P[i] = SDL_CreateTextureFromSurface(renderer, s);
	            rectKetThuc2P[i] = {xPos[i], yPos[i], s->w, s->h};
	            SDL_FreeSurface(s);
	        }
	    }
	
};

#endif
