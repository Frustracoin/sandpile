#include <cstdint>
#include <string>
#include <iostream>

class pol{
public:
    int shir = 1, vis = 1;
    mutable int16_t x0, y0, x1, y1;
    uint64_t** kord;
    void sizeup(int leftest, int verhnee, int rightest, int nisnie);
    void iterate(int n, int freq);
    void createpol();
};
class dot{
public:
    int16_t x = -1, y = -1;
    dot(int16_t x, int16_t y) : x(x), y(y){};
    dot() : x(-1), y(-1){};
};
dot* resize (dot * s, int& size);
class parser {
public:
    int max_iter, freq;
    std::string in_filename, out_filename;
};
void createpic(pol& ngopol, int iter);
void dot_col(int shir, int pad, pol& ngopol, std::ofstream& file);
void title(char* Path, int iter);
void readcmd(int argc, char **argv, parser& data_set);
void getsize(const std::string& filename, pol& pol);
void bmpsetting(pol& ngopol, std::ofstream& file);
void readpol(const std::string& filename, pol& pol);