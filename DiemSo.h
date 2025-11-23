#ifndef DIEMSO_H
#define DIEMSO_H

#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

class DiemSo {
private:
    std::string duongDanFile;

public:
    DiemSo(const std::string& path = "data/diem.txt")
        : duongDanFile(path) {}

    // Luu diem moi vao file
    void luuDiem(int diemMoi) {
        std::ofstream file(duongDanFile, std::ios::app);
        if (!file) {
            std::cout << "Khong mo duoc file diem!\n";
            return;
        }
        file << diemMoi << "\n";
        file.close();
    }

    // Doc tat ca diem da luu
    std::vector<int> docDiem() {
        std::vector<int> ds;
        std::ifstream file(duongDanFile);
        if (!file) return ds;

        int diem;
        while (file >> diem) {
            ds.push_back(diem);
        }
        file.close();

        std::sort(ds.begin(), ds.end(), std::greater<int>()); // sap xep giam dan
        return ds;
    }
};

#endif
