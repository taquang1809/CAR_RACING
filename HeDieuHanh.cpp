#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "QuanLyMenu.h"
#include "CheDo1Nguoi.h"
#include "CheDo2Nguoi.h"


SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
	
QuanLyMenu* quanLyMenu;
CheDo1Nguoi* cheDo1;
CheDo2Nguoi* cheDo2;
	
TrangThaiGame trangThai;


bool khoiTao() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "Khong the khoi tao SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Khong the khoi tao SDL_image: " << IMG_GetError() << std::endl;
    }

    if (TTF_Init() == -1) {
        std::cout << "Khong the khoi tao SDL_ttf: " << TTF_GetError() << std::endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Khong the khoi tao SDL_mixer: " << Mix_GetError() << std::endl;
    }

    window = SDL_CreateWindow("Game Dua Xe 2D",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Khong tao duoc cua so: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Khong tao duoc renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("assets/font.ttf", 28);
    if (!font) {
        std::cout << "Khong the mo font: " << TTF_GetError() << std::endl;
    }

    quanLyMenu = new QuanLyMenu(renderer, font);
    return true;
}

void giaiPhong() {
    if (cheDo1) { delete cheDo1; cheDo1 = nullptr; }
    if (cheDo2) { delete cheDo2; cheDo2 = nullptr; }
    if (quanLyMenu) { delete quanLyMenu; quanLyMenu = nullptr; }

    if (font) { TTF_CloseFont(font); font = nullptr; }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void chay() {
    bool thoat = false;
    SDL_Event e;
    const Uint8* phim = SDL_GetKeyboardState(NULL);

    while (!thoat) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) thoat = true;

            // Xu ly menu chung
            quanLyMenu->capNhat(e, trangThai);

            // Xu ly phim ESC
            if ((trangThai == CHOI_1_NGUOI || trangThai == CHOI_2_NGUOI) &&
                e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                if (trangThai == CHOI_1_NGUOI) trangThai = TAM_DUNG_1P;
                else trangThai = TAM_DUNG_2P;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vong lap cac trang thai
        switch (trangThai) {
            case MENU_CHINH:
            case MENU_CHON_CHE_DO:
            case TAM_DUNG_1P:
            case TAM_DUNG_2P:
            case KET_THUC_1P:
            case KET_THUC_2P:
            case CAI_DAT:
            case KY_LUC:
                quanLyMenu->ve(trangThai);
                break;
            case CHOI_1_NGUOI:
                if (!cheDo1) cheDo1 = new CheDo1Nguoi(renderer);
                cheDo1->capNhat(phim);
                cheDo1->ve();
                if (cheDo1->daThua()) {
                	quanLyMenu->capNhatKetQua1(cheDo1->getDiem());
                	cheDo1->datLai();
                    delete cheDo1; cheDo1 = nullptr;
                    trangThai = KET_THUC_1P;
                }
                break;

            case CHOI_2_NGUOI:{
                if (!cheDo2) cheDo2 = new CheDo2Nguoi(renderer);
                cheDo2->capNhat(phim);
                cheDo2->ve();
                if (cheDo2->daKetThuc()) {
                	quanLyMenu->capNhatKetQua2(cheDo2->getDiemXe1(), cheDo2->getDiemXe2(), cheDo2->getKetQua());
				    delete cheDo2; cheDo2 = nullptr;
				    trangThai = KET_THUC_2P;
				}
				break;
			}
								                                        	

            default:{
				break;
			}
		}

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
	
    giaiPhong();
}

int main(int argc, char* argv[]) {
    if (!khoiTao()) return -1;
    chay();
    return 0;
}