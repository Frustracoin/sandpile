#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "all.h"

void pol::createpol() {
    kord = new uint64_t *[vis];
    for (int i = 0; i < vis; ++i){
        kord[i] = new uint64_t [shir];
        for (int j = 0; j < shir; ++j)
            kord[i][j] = 0;
    }}
void pol::iterate(int n, int freq){
    if (freq == 0){
        freq = n-1;
    }
    for (int iter = 0; iter <= n; ++iter){
        if (iter % freq == 0)
            createpic(*this, iter);
        int size = 2, k = 0;
        dot* s = new dot[size];
        for (int y = 0; y < vis; y++){
            for (int x = 0; x < shir; x++) {
                if (kord[y][x] >= 4){
                    if (k == size){
                        s = resize(s, size);
                    }
                    s[k++] = dot(x, y);
                }}}
        int leftest = 0, verhnee = 0, rightest = 0, nisnie = 0;
        for (int i = 0; i < size; i++){
            if (s[i].x == -1)
                break;
            leftest = 0, verhnee = 0, rightest = 0, nisnie = 0;
            if (s[i].x - 1 <= 0)
                leftest = 1;
            if (s[i].x >= shir-1)
                rightest = 1;
            if (s[i].y - 1 <= 0)
                verhnee = 1;
            if (s[i].y >= vis-1)
                nisnie = 1;
            if (leftest || rightest || verhnee || nisnie){
                int prevshir = shir, prevvis = vis;
                sizeup(leftest, verhnee, rightest, nisnie);
                for (int j = 0; j < size; j++){
                    if (s[j].x == -1)
                        break;
                    s[j].x = s[j].x + prevshir/2;
                    s[j].y = s[j].y + prevvis/2;
                }}
            kord[s[i].y][s[i].x] -= 4;
            kord[s[i].y][s[i].x-1]++;
            if (s[i].x-1 <= x0 && x0 != 0)
                x0--;
            kord[s[i].y][s[i].x+1]++;
            if (s[i].x+1 >= x1 && x1 != shir-1)
                x1++;
            kord[s[i].y-1][s[i].x]++;
            if (s[i].y-1 <= y0 && y0 != 0)
                y0--;
            kord[s[i].y+1][s[i].x]++;
            if (s[i].y+1 >= y1 && y1 != vis-1)
                y1++;
        }}}
dot* resize(dot *s, int& size){
    dot* temp = new dot[size * 2];
    for (int i = 0; i < size; ++i){
        temp[i].x = s[i].x;
        temp[i].y = s[i].y;
    }
    size *=2;
    delete [] s;
    return temp;
}
void readpol(const std::string& filename, pol& pol){
    std::ifstream in(filename);
    int tempx, tempy;
    if (in.is_open()){
        std::string line;
        while (in)
        {
            in >> tempx;
            in >> tempy;
            in >> pol.kord[pol.y1 - tempy][tempx - pol.x0];
        }
        pol.x0 = 0;
        pol.x1 = pol.shir - 1;
        pol.y0 = 0;
        pol.y1 = pol.vis - 1;
    }
}
void readcmd(int argc, char **argv, parser& data_set){
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--max-iter" || arg == "-m" ){
            data_set.max_iter = std::stoi(argv[++i]);
        } else if (arg == "--freq" || arg == "-f"){
            data_set.freq = std::stoi(argv[++i]);
        } else if (arg == "--input" || arg == "-i"){
            data_set.in_filename = argv[++i];
        } else if (arg == "--output" || arg == "-o"){
            data_set.out_filename = argv[++i];
        }}}
void dot_col(int shir, int pad, pol& ngopol, std::ofstream& file) {
    // white
    file.write("\xff\xff\xff\x00", 4);
    // green
    file.write("\x60\xad\x26\x00", 4);
    // yellow
    file.write("\x30\xa6\xab\x00", 4);
    // purple
    file.write("\x6a\x2c\x70\x00", 4);
    // black
    file.write("\x00\x00\x00\x00", 4);
    for (ssize_t i = ngopol.y1; i >= ngopol.y0; --i) {
        for (size_t j = 0; j < shir / 2; ++j) {
            uint8_t pix = 0;
            pix |= ((ngopol.kord[i][ngopol.x0 + 2 * j] > 3) ? 4 : ngopol.kord[i][ngopol.x0 + 2 * j]) << 4;
            pix |= ((ngopol.kord[i][ngopol.x0 + 2 * j + 1] > 3) ? 4 : ngopol.kord[i][ngopol.x0 + 2 * j + 1]);
            file.write(reinterpret_cast<char*>(&pix), 1);
        }
        if (shir % 2 == 1) {
            uint8_t pix = 0;
            pix |= ((ngopol.kord[i][ngopol.x1] > 3) ? 4 : ngopol.kord[i][ngopol.x1]) << 4;
            file.write(reinterpret_cast<char*>(&pix), 1);
        }
        for (size_t j = 0; j < pad; ++j) {
            file.write("\x00", 1);
        }}}
void getsize(const std::string& filename, pol& pol){
    std::ifstream in(filename);
    int16_t ymax=-32768, xmax=-32768, ymin=32767, xmin=32767, tempx, tempy;
    uint64_t val;
    if (!in.is_open()){ std::cerr << "no file" << "\n";
    } else {
        std::string line;
        while (in)
        {
            in >> tempx;
            in >> tempy;
            in >> val;
            xmin = std::min(tempx, xmin);
            xmax = std::max(tempx, xmax);
            ymax = std::max(tempy, ymax);
            ymin = std::min(tempy, ymin);
        }
    }
    pol.x0 = xmin;
    pol.x1 = xmax;
    pol.y0 = ymin;
    pol.y1 = ymax;
    pol.shir = abs(xmax - xmin) + 1;
    pol.vis = abs(ymax - ymin ) + 1;
    if (pol.shir == 1){
        pol.shir++;
        pol.x1++;
    }
    if (pol.vis == 1){
        pol.vis++;
        pol.y1++;
    }
}

void pol::sizeup(int leftest, int verhnee, int rightest, int nisnie){
    x0 = x0 - leftest + shir/2;
    x1 = x1 + rightest + shir/2;
    y0 = y0 - verhnee + vis/2;
    y1 = y1 + nisnie + vis/2;
    int nvis = vis/2 + vis/2 + vis;
    int nshir = shir/2 + shir/2 + shir;
    uint64_t **temp = new uint64_t *[nvis];
    for (int i = 0; i < nvis;i++){
        temp[i] = new uint64_t [nshir];
        for (int j = 0; j < nshir; j++){
            temp[i][j] = 0;
        }}
    for (int i = 0; i < vis; i++){
        for (int j = 0; j < shir; j++){
            temp[i+vis/2][j+shir/2] = kord[i][j];
        }}
    shir = shir + shir/2 + shir/2;
    vis = vis + vis/2 + vis/2;
    kord = new uint64_t *[vis];
    for (int i = 0; i < vis; i++){
        kord[i] = new uint64_t[shir];
        for (int j = 0; j < shir; j++){
            kord[i][j] = temp[i][j];
        }}
    delete [] temp;
}
void createpic(pol& ngopol, int iter) {
    char* Path = new char[100]{};
    title(Path, iter);
    int shir = ngopol.x1 - ngopol.x0 + 1;
    int vis = ngopol.y1 - ngopol.y0 + 1;
    std::ofstream file(Path, std::ios::binary);
    if (!file.is_open()) {
        std::cout << Path << std::endl;
        throw std::runtime_error("no such file or directory");
    }
    bmpsetting(ngopol, file);
    int pad = ((4 - ((shir) / 2 + shir % 2) % 4) % 4);
    dot_col(shir, pad, ngopol, file);
}
void bmpsetting(pol& ngopol, std::ofstream& file) {
    int vis = ngopol.y1 - ngopol.y0 + 1;
    int shir = ngopol.x1 - ngopol.x0 + 1;
    int pad = ((4 - (shir / 2 + shir % 2) % 4) % 4);
    int offset = 74;
    uint64_t file_size = offset + (shir / 2 + shir % 2) * vis + pad * vis;
    int header_size = 40;
    int zero = 0;
    int colors = 5;
    int planes = 1;
    int bits_per_pix = 4;
    file.write("BM", 2);
    file.write(reinterpret_cast<char*>(&file_size), 8);
    file.write(reinterpret_cast<char*>(&offset), 4);
    file.write(reinterpret_cast<char*>(&header_size), 4);
    file.write(reinterpret_cast<char*>(&shir), 4);
    file.write(reinterpret_cast<char*>(&vis), 4);
    file.write(reinterpret_cast<char*>(&planes), 2);
    file.write(reinterpret_cast<char*>(&bits_per_pix), 2);
    file.write(reinterpret_cast<const char*>(&zero), 4);
    file.write(reinterpret_cast<char*>(&zero), 4);
    file.write(reinterpret_cast<char*>(&zero), 4);
    file.write(reinterpret_cast<char*>(&zero), 4);
    file.write(reinterpret_cast<char*>(&colors), 4);
    file.write(reinterpret_cast<char*>(&zero), 4);
}
void title(char* Path, int iter) {
    char* number = new char[20]{};
    std::sprintf(number, "%zu", iter);
    std::strcat(Path, number);
    std::strcat(Path, ".bmp");
    delete[] number;
}
int main(int argc, char ** argv) {
    parser comand;
    readcmd(argc, argv, comand);
    pol ngopol;
    getsize(comand.in_filename, ngopol);
    ngopol.createpol();
    readpol(comand.in_filename, ngopol);
    ngopol.iterate(comand.max_iter, comand.freq);
    return 0;
}