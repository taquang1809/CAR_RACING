#ifndef DONGHOGAME_H
#define DONGHOGAME_H

#include <SDL2/SDL.h>

class DongHoGame {
private:
    Uint32 mocBatDau;  // moc bat dau dem
    Uint32 tamDung;    // thoi gian tich luy khi pause
    bool dangChay; 
    bool biTamDung;

public:
    DongHoGame() {
        reset();
    }

    // Bat dau dem tu dau
    void batDauDem() {
        dangChay = true;
        biTamDung = false;
        mocBatDau = SDL_GetTicks();
        tamDung = 0;
    }

    // Tam dung
    void pause() {
        if (dangChay && !biTamDung) {
            biTamDung = true;
            tamDung = SDL_GetTicks() - mocBatDau;
        }
    }

    // Tiep tuc
    void resume() {
        if (dangChay && biTamDung) {
            biTamDung = false;
            mocBatDau = SDL_GetTicks() - tamDung;
            tamDung = 0;
        }
    }

    // Reset : dung hoan toan
    void reset() {
        mocBatDau = 0;
        tamDung = 0;
        dangChay = false;
        biTamDung = false;
    }
	
	
    void datLai() { // bat dau van moi
        mocBatDau = SDL_GetTicks();
        tamDung = 0;
        dangChay = true;
        biTamDung = false;
    }

	
    Uint32 getThoiGianMS() const {
        if (!dangChay) return 0;
        if (biTamDung) return tamDung;
        return SDL_GetTicks() - mocBatDau;
    }

    // Lay thoi gian da troi qua (s)
    Uint32 getThoiGianGiay() const {
        return getThoiGianMS() / 1000;
    }

    // Game het gio?
    bool hetThoiGian(int gioiHanGiay) const {
        return getThoiGianGiay() >= (Uint32)gioiHanGiay;
    }
};

#endif


