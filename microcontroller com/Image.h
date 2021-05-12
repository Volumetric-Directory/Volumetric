#pragma once

#include <vector>
#include"SerialPort.h"

using namespace std;

struct Color
{
    float r,g,b;

    Color();
    Color(float r, float g, float b);
    ~Color();
};

class Image
{
public:
    Image(int width, int height);
    ~Image();

    Color GetColor(int x, int y) const;
    void SetColor(const Color& color, int x, int y);

    void Read(const char* path);
    void Export(const char* path)const;

    void send(SerialPort &arduino);
    void sendB(SerialPort &arduino);

private:
    int m_width;
    int m_height;
    std::vector<Color> m_colors;
};

//unsigned short Color_To_565(uint8_t r, uint8_t g, uint8_t b);